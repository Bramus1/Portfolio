document.addEventListener("DOMContentLoaded", function () {

    const logoutButton = document.getElementById("logoutButton");
    const addUserButton = document.getElementById("addUserButton");
    const saveNewUserButton = document.getElementById("saveNewUserButton");
    const cancelAddUserButton = document.getElementById("cancelAddUserButton");

    const addUserForm = document.getElementById("addUserForm");
    const newUsernameInput = document.getElementById("newUsername");
    const newPasswordInput = document.getElementById("newPassword");
    const newRoleIdInput = document.getElementById("newRoleId");

    const addInventoryButton = document.getElementById("addInventoryButton");
    const saveInventoryButton = document.getElementById("saveInventoryButton");
    const cancelAddInventoryButton = document.getElementById("cancelAddInventoryButton");
    const addInventoryForm = document.getElementById("addInventoryForm");

    const inventoryTable = document.getElementById("inventoryTable").getElementsByTagName("tbody")[0];
    
    addInventoryButton.addEventListener("click", () => addInventoryForm.style.display = "block");
    cancelAddInventoryButton.addEventListener("click", () => addInventoryForm.style.display = "none");


    const usersTable = document.getElementById("usersTable").getElementsByTagName('tbody')[0];

    
    logoutButton.addEventListener("click", function () {
        window.location.href = "/index.html"; 
    });

    
    addUserButton.addEventListener("click", function () {
        addUserForm.style.display = "block"; 
    });

    
    saveNewUserButton.addEventListener("click", async function () {
        const username = newUsernameInput.value.trim();
        const password = newPasswordInput.value.trim();
        const roleId = parseInt(newRoleIdInput.value.trim());

        console.log('Saving user with data:', { username, password, roleId });

        
        if (!username || !password || isNaN(roleId) || roleId < 0) {
            alert("Please fill in all fields with valid data.");
            return;
        }

        try {
            const response = await fetch("http://localhost:9090/users", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "ADD_USER",
                    username: username,
                    password: password,
                    role_id: roleId
                })
            });

            if (!response.ok) {
                throw new Error("Failed to add user.");
            }

            alert("User added successfully!");
            addUserForm.style.display = "none"; 
            fetchUsers(); 
        } catch (error) {
            console.error("Error adding user:", error);
            alert("An error occurred while adding the user.");
        }
    });

    
    cancelAddUserButton.addEventListener("click", function () {
        addUserForm.style.display = "none"; 
    });



    
    async function fetchUsers() {
        try {
            const response = await fetch("http://localhost:9090/users", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_USERS" }),
            });

            if (!response.ok) {
                throw new Error("Failed to fetch users.");
            }

            const data = await response.json();
            console.log("Fetched users:", data);

            usersTable.innerHTML = ""; 

            data.users.forEach(user => {
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${user.user_id}</td>
                    <td>${user.username}</td>
                    <td>${user.role_id}</td>
                    <td><button class="deleteUserButton" data-id="${user.user_id}">Delete</button></td>
                `;
                usersTable.appendChild(row);
            });

            
            const deleteButtons = document.querySelectorAll(".deleteUserButton");
            deleteButtons.forEach(button => {
                button.addEventListener("click", async function () {
                    const userId = button.getAttribute("data-id");
                    await deleteUser(userId);
                    fetchUsers(); 
                });
            });
        } catch (error) {
            console.error("Error fetching users:", error);
            alert("Failed to load users.");
        }
    }

        

    async function deleteUser(userId) {
        try {
            const response = await fetch("http://localhost:9090/users", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "DELETE_USER", user_id: userId }),
            });

            if (!response.ok) {
                throw new Error("Failed to delete user.");
            }

            alert("User deleted successfully!");
        } catch (error) {
            console.error("Error deleting user:", error);
            alert("An error occurred while deleting the user.");
        }
    }

    
    addMenuItemButton.addEventListener("click", function () {
        const addMenuForm = document.getElementById("addMenuForm");
        addMenuForm.style.display = "block";
    });

    saveMenuItemButton.addEventListener("click", async function () {
        const name = document.getElementById("itemName").value.trim();
        const price = parseFloat(document.getElementById("itemPrice").value.trim());
        const type = document.getElementById("itemType").value;
    
        if (!name || isNaN(price) || !type) {
            alert("Please fill in all fields with valid data.");
            return;
        }
    
        try {
            const response = await fetch("http://localhost:9090/menu", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "ADD_MENU_ITEM",
                    name: name,
                    price: price,
                    type: type
                })
            });
    
            if (!response.ok) {
                throw new Error("Failed to add menu item.");
            }
    
            alert("Menu item added successfully!");
            addMenuForm.style.display = "none";
            fetchMenu(); 
        } catch (error) {
            console.error("Error adding menu item:", error);
            alert("An error occurred while adding the menu item.");
        }
    });
    

    async function fetchMenu() {
        try {
            const response = await fetch("http://localhost:9090/menu", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_MENU" }),
            });

            if (!response.ok) {
                throw new Error("Failed to fetch menu.");
            }

            const data = await response.json();
            console.log("Fetched menu:", data);

            menuItemsTable.innerHTML = ""; 

            data.menu.forEach(item => {
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${item.id}</td>
                    <td>${item.name}</td>
                    <td>$${item.price.toFixed(2)}</td>
                    <td>${item.type === "f" ? "Food" : "Drink"}</td
                    <td><button class="deleteMenuItemButton" data-id="${item.id}">Delete</button></td>
                `;
                menuItemsTable.appendChild(row);
            });

            
            const deleteMenuButtons = document.querySelectorAll(".deleteMenuItemButton");
            deleteMenuButtons.forEach(button => {
                button.addEventListener("click", async function () {
                    const menuItemId = button.getAttribute("data-id");
                    await deleteMenuItem(menuItemId);
                    fetchMenu(); 
                });
            });
        } catch (error) {
            console.error("Error fetching menu:", error);
            alert("Failed to load menu.");
        }
    }

    async function deleteMenuItem(menuItemId) {
        try {
            const response = await fetch("http://localhost:9090/menu", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "DELETE_MENU_ITEM", item_id: menuItemId }),
            });

            if (!response.ok) {
                throw new Error("Failed to delete menu item.");
            }

            alert("Menu item deleted successfully!");
        } catch (error) {
            console.error("Error deleting menu item:", error);
            alert("An error occurred while deleting the menu item.");
        }
    }


    saveInventoryButton.addEventListener("click", async function () {
        const name = document.getElementById("inventoryName").value.trim();
        const quantity = parseInt(document.getElementById("inventoryQuantity").value.trim());
        const unit = document.getElementById("inventoryUnit").value.trim();
        const minRequired = parseInt(document.getElementById("inventoryMinRequired").value.trim());

        if (!name || isNaN(quantity) || !unit || isNaN(minRequired)) {
            alert("Please fill in all fields with valid data.");
            return;
        }

        try {
            const response = await fetch("http://localhost:9090/inventory", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "ADD_INVENTORY",
                    name,
                    quantity,
                    unit,
                    min_required: minRequired
                })
            });

            if (!response.ok) throw new Error("Failed to add inventory item.");

            alert("Inventory item added successfully!");
            addInventoryForm.style.display = "none";
            fetchInventory();
        } catch (error) {
            console.error("Error adding inventory item:", error);
            alert("An error occurred while adding the inventory item.");
        }
    });

    async function fetchInventory() {
        try {
            const response = await fetch("http://localhost:9090/inventory", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_INVENTORY" }),
            });
    
            if (!response.ok) {
                throw new Error("Failed to fetch inventory.");
            }
    
            const data = await response.json();
            console.log("Fetched inventory data:", data);
    
            const inventoryTable = document.getElementById("inventoryTable").getElementsByTagName("tbody")[0];
            inventoryTable.innerHTML = ""; 
    
            data.inventory.forEach(item => {
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${item.inventory_id}</td>
                    <td>${item.item_name}</td>
                    <td>${item.quantity}</td>
                    <td>${item.unit}</td>
                    <td>${item.min_required}</td>
                    <td>${item.last_updated}</td>
                    <td>
                        <button class="editInventoryButton" data-id="${item.inventory_id}">Edit</button>
                        <button class="deleteInventoryButton" data-id="${item.inventory_id}">Delete</button>
                    </td>
                `;
                inventoryTable.appendChild(row);
            });
    
            document.querySelectorAll(".editInventoryButton").forEach(button => {
                button.addEventListener("click", function () {
                    const inventoryId = button.getAttribute("data-id");
                    const item = data.inventory.find(i => i.inventory_id == inventoryId);
                    if (item) {
                        showEditForm(item);
                    }
                });
            });
    
            document.querySelectorAll(".deleteInventoryButton").forEach(button => {
                button.addEventListener("click", async function () {
                    const inventoryId = button.getAttribute("data-id");
                    await deleteInventory(inventoryId);
                    fetchInventory();
                });
            });
        } catch (error) {
            console.error("Error fetching inventory:", error);
            alert("Failed to load inventory.");
        }
    }
    

    async function deleteInventory(itemId) {
        try {
            const response = await fetch("http://localhost:9090/inventory", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "DELETE_INVENTORY", inventory_id: itemId }),
            });

            if (!response.ok) throw new Error("Failed to delete inventory item.");

            alert("Inventory item deleted successfully!");
        } catch (error) {
            console.error("Error deleting inventory item:", error);
            alert("An error occurred while deleting the inventory item.");
        }
    }


    

    function showEditForm(item) {
        document.getElementById("inventoryName").value = item.item_name;
        document.getElementById("inventoryQuantity").value = item.quantity;
        document.getElementById("inventoryUnit").value = item.unit;
        document.getElementById("inventoryMinRequired").value = item.min_required;
        document.getElementById("editInventoryForm").setAttribute("data-id", item.inventory_id);
        document.getElementById("editInventoryForm").style.display = "block";
        document.getElementById("saveEditInventoryButton").addEventListener("click", updateInventory);
    }
    

    async function updateInventory() {
        const form = document.getElementById("editInventoryForm");
        const inventoryId = form.getAttribute("data-id");
    
        const updatedData = {
            command: "UPDATE_INVENTORY",
            inventory_id: parseInt(inventoryId),
            item_name: document.getElementById("editInventoryName").value.trim(),
            quantity: parseInt(document.getElementById("editInventoryQuantity").value.trim()),
            unit: document.getElementById("editInventoryUnit").value.trim(),
            min_required: parseInt(document.getElementById("editInventoryMinRequired").value.trim())
        };
    
        try {
            const response = await fetch("http://localhost:9090/inventory", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify(updatedData),
            });
    
            if (!response.ok) {
                throw new Error("Failed to update inventory item.");
            }
    
            const data = await response.json();
            if (data.status === "success") {
                alert("Inventory item updated successfully!");
                form.style.display = "none";
                fetchInventory(); 
            } else {
                alert(data.message || "Failed to update inventory item.");
            }
        } catch (error) {
            console.error("Error updating inventory item:", error);
            alert("An error occurred while updating the inventory item.");
        }
    }
    


    function editInventory(inventoryId) {
        const inventoryItem = getInventoryItemById(inventoryId); 
        
        document.getElementById("editInventoryName").value = inventoryItem.name;
        document.getElementById("editInventoryQuantity").value = inventoryItem.quantity;
        document.getElementById("editInventoryUnit").value = inventoryItem.unit;
        document.getElementById("editInventoryMinRequired").value = inventoryItem.min_required;
        document.getElementById("editInventoryForm").style.display = "block";
        document.getElementById("editInventoryForm").setAttribute("data-id", inventoryId);
    }
    


    document.getElementById("viewOrderHistoryButton").addEventListener("click", async function () {
        const startDate = document.getElementById("startDate").value;
        const endDate = document.getElementById("endDate").value;
    
        if (!startDate || !endDate) {
            alert("Please select both start and end dates.");
            return;
        }
    
        try {
            const response = await fetch("http://localhost:9090/orderHistory", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "GET_ORDER_HISTORY",
                    start_date: startDate,
                    end_date: endDate
                })
            });
    
            const data = await response.json();
            const tableBody = document.getElementById("orderHistoryTable").getElementsByTagName("tbody")[0];
            tableBody.innerHTML = "";
    
            data.order_history.forEach(entry => {
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${entry.history_id}</td>
                    <td>${entry.commande_id}</td>
                    <td>${entry.user_id}</td>
                    <td>${entry.numeroTable}</td>
                    <td>${entry.commande_status}</td>
                    <td>${entry.sub_total}</td>
                    <td>${entry.created_at}</td>
                `;
                tableBody.appendChild(row);
            });
        } catch (error) {
            console.error("Error fetching order history:", error);
        }
    });
    
    document.getElementById("generateRevenueButton").addEventListener("click", async function () {
        const startDate = document.getElementById("startDate").value;
        const endDate = document.getElementById("endDate").value;
    
        if (!startDate || !endDate) {
            alert("Please select both start and end dates.");
            return;
        }
    
        try {
            const response = await fetch("http://localhost:9090/revenue", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "GENERATE_REPORT",
                    start_date: startDate,
                    end_date: endDate
                })
            });
    
            const data = await response.json();
            document.getElementById("revenueDisplay").textContent = `Total Revenue: $${data.total_revenue}`;
        } catch (error) {
            console.error("Error generating revenue:", error);
        }
    });
    
    
    fetchUsers(); 
    fetchMenu(); 
    fetchInventory();
});
