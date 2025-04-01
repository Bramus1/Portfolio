// Select DOM elements
const loginForm = document.getElementById("loginForm");
const errorMessage = document.getElementById("errorMessage");

loginForm.addEventListener("submit", async function (event) {
    event.preventDefault();

    // Get form input values
    const username = document.getElementById("username").value.trim();
    const password = document.getElementById("password").value.trim();
    const role = document.getElementById("role").value;

    // Validate input
    if (!username || !password) {
        displayError("Please enter both username and password.");
        return;
    }

    const roleId = parseInt(role);

    if (isNaN(roleId)) {
        displayError("Invalid role selected.");
        return;
    }

    try {
        // Send login request to server
        const response = await fetch("http://localhost:9090/login", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ command: "LOGIN", username, password, role: roleId }),
        });

        const responseText = await response.text();
        console.log("Response text:", responseText);

        if (!response.ok) {
            throw new Error("Login failed.");
        }

        // Parse JSON response
        let data;
        try {
            data = JSON.parse(responseText);
            console.log("Parsed JSON:", data);
        } catch (error) {
            throw new Error("Failed to parse response as JSON.");
        }

        // Handle successful login
        if (data.user_id && data.role_id !== undefined) {
            if (data.role_id === roleId) {
                localStorage.setItem("userId", data.user_id);
                console.log("Saving userId to localStorage:", data.user_id);

                alert("Login successful!");
                navigateToRolePage(roleId);
            } else {
                displayError("Role mismatch. Please check your role.");
            }
        } else {
            displayError("Login failed or invalid data.");
        }
    } catch (error) {
        console.error("Error logging in:", error);
        displayError("An error occurred during login. Please try again.");
    }
});

// Function to navigate to the appropriate role page
function navigateToRolePage(roleId) {
    switch (roleId) {
        case 0:
            window.location.href = "../waiter/waiter.html";
            break;
        case 1:
            window.location.href = "../bar/bar.html";
            break;
        case 2:
            window.location.href = "../admin_panel/admin_panel.html";
            break;
        case 3:
            window.location.href = "../kitchen/kitchen.html";
            break;
        default:
            displayError("Invalid role. Unable to navigate.");
    }
}

// Function to display error messages
function displayError(message) {
    errorMessage.textContent = message;
    errorMessage.style.display = "block";
    setTimeout(() => {
        errorMessage.style.display = "none";
    }, 5000); // Hide after 5 seconds
}
