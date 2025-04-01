
document.addEventListener("DOMContentLoaded", () => {
    const tableNumberInput = document.getElementById("tableNumber");
    const menuItemsContainer = document.getElementById("menuItemsContainer");
    const ordersTable = document.getElementById("ordersTable");
    const addedItemsContainer = document.getElementById("addedItemsContainer");
    let currentOrderItems = [];

    function showLoadingMessage(targetElement, message = "Loading...") {
        targetElement.innerHTML = `<tr><td colspan="6">${message}</td></tr>`;
    }

    async function fetchMenu() {
        try {
            const response = await fetch("http://localhost:9090", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_MENU" }),
            });
    
            if (!response.ok) throw new Error("Failed to fetch menu.");
    
            const data = await response.json();
            if (!data.menu || !Array.isArray(data.menu)) {
                throw new Error("Invalid menu data received.");
            }
    
            menuItemsContainer.innerHTML = ""; 
    
            data.menu.forEach(item => {
                const menuItemDiv = document.createElement("div");
                menuItemDiv.classList.add("menu-item");
    
                menuItemDiv.innerHTML = `
                    <div class="menu-item-details">
                        <span class="menu-item-name">${item.name}</span>
                        <span class="menu-item-price">$${item.price.toFixed(2)}</span>
                    </div>
                    <input 
                        type="number" 
                        class="menu-item-quantity" 
                        min="1" 
                        value="1" 
                        data-id="${item.id}" 
                        data-name="${item.name}" 
                        data-price="${item.price}" 
                    />
                    <button 
                        type="button" 
                        class="add-to-order-button" 
                        data-id="${item.id}">
                        Add
                    </button>
                `;
    
                menuItemsContainer.appendChild(menuItemDiv);
            });
    
            document.querySelectorAll(".add-to-order-button").forEach(button => {
                button.addEventListener("click", () => {
                    const input = button.previousElementSibling;
                    const itemId = parseInt(input.dataset.id, 10);
                    const itemName = input.dataset.name;
                    const itemPrice = parseFloat(input.dataset.price);
                    const quantity = parseInt(input.value, 10);
    
                    if (isNaN(quantity) || quantity <= 0) {
                        alert("Please enter a valid quantity.");
                        return;
                    }
    
                    const existingItem = currentOrderItems.find(item => item.id === itemId);
                    if (existingItem) {
                        existingItem.quantity += quantity;
                    } else {
                        currentOrderItems.push({ id: itemId, name: itemName, price: itemPrice, quantity });
                    }
    
                    renderCurrentOrderItems();
                });
            });
        } catch (error) {
            console.error("Error fetching menu:", error);
            alert("Failed to load menu. Please try again.");
        }
    }
    
    

    function renderCurrentOrderItems() {
        addedItemsContainer.innerHTML = "";
        currentOrderItems.forEach(item => {
            const itemDiv = document.createElement("div");
            itemDiv.classList.add("added-item");
            itemDiv.innerHTML = `
                <span>${item.name} x${item.quantity} - $${(item.price * item.quantity).toFixed(2)}</span>
                <button class="remove-item-button" data-id="${item.id}">Remove</button>
            `;
            addedItemsContainer.appendChild(itemDiv);

            itemDiv.querySelector(".remove-item-button").addEventListener("click", () => {
                currentOrderItems = currentOrderItems.filter(orderItem => orderItem.id !== item.id);
                renderCurrentOrderItems();
            });
        });
    }

    async function fetchOrders() {
        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_ORDERS" }),
            });

            if (!response.ok) throw new Error("Failed to fetch orders.");

            const data = await response.json();
            if (!data.orders || !Array.isArray(data.orders)) {
                throw new Error("Invalid orders data received.");
            }

            ordersTable.innerHTML = "";
            if (data.orders.length === 0) {
                showLoadingMessage(ordersTable, "No orders found.");
                return;
            }

            data.orders.forEach(order => {
                const items = Array.isArray(order.items) ? order.items : [];
                const row = document.createElement("tr");
                row.innerHTML = `
                    <td>${order.commande_id}</td>
                    <td>${order.user_id}</td>
                    <td>${items.map(i => `${i.name} x${i.quantity}`).join(", ")}</td> 
                    <td>${order.table_id}</td>
                    <td>${order.order_status || "Pending"}</td>
                    <td>
                        ${order.order_status !== "served" ? `<button class="serveOrderButton" data-id="${order.commande_id}">Serve</button>` : ""}
                    </td>
                `;
                ordersTable.appendChild(row);
            });
            

            document.querySelectorAll(".serveOrderButton").forEach(button => {
                button.addEventListener("click", async function () {
                    const orderId = button.getAttribute("data-id");
                    await serveOrder(orderId);
                    fetchOrders();
                });
            });
        } catch (error) {
            console.error("Error fetching orders:", error);
            alert("Failed to load Orders. Please try again.");
        }
    }

    async function serveOrder(orderId) {
        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "ARCHIVE_ORDER",
                    order_id: orderId,
                }),
            });
    
            if (!response.ok) throw new Error("Failed to archive order.");
    
            const data = await response.json();
            if (data.status === "success") {
                alert(`Order ${orderId} marked as served and archived!`);
                fetchOrders(); 
            } else {
                alert(data.message || "Failed to archive order.");
            }
        } catch (error) {
            console.error("Error archiving order:", error);
            alert("An error occurred while archiving the order. Please try again.");
        }
    }
    

    document.getElementById("orderForm").addEventListener("submit", async event => {
        event.preventDefault();
        const userId = localStorage.getItem("userId");

        const tableNumber = parseInt(tableNumberInput.value, 10);

        if (isNaN(tableNumber) || tableNumber <= 0) {
            alert("Please enter a valid table number.");
            return;
        }
        if (currentOrderItems.length === 0) {
            alert("Please add at least one item to the order.");
            return;
        }

        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "PLACE_ORDER",
                    table_number: tableNumber,
                    items: currentOrderItems,
                    user_id: userId,
                }),
            });

            if (!response.ok) throw new Error("Failed to place order.");

            const data = await response.json();
            alert(data.message || "Order placed successfully!");
            currentOrderItems = [];
            renderCurrentOrderItems();
            await fetchOrders();
        } catch (error) {
            console.error("Error placing order:", error);
            alert("An error occurred while placing the order. Please try again.");
        }
    });


    async function fetchNotifications(userId) {
        const notificationsContainer = document.getElementById("notificationsContainer");
        if (!notificationsContainer) {
            console.error("Notifications container not found!");
            return;
        }
    
        try {
            const response = await fetch("http://localhost:9090/notifications", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_NOTIFICATIONS", user_id: userId }),
            });
    
            if (response.ok) {
                const data = await response.json();
                console.log("Notifications:", data.notifications);
    
                notificationsContainer.innerHTML = "";
                if (data.notifications.length === 0) {
                    notificationsContainer.innerHTML = "<p>No notifications available.</p>";
                    return;
                }
    
                data.notifications
                    .filter(notification => notification.message.includes("from kitchen") || notification.message.includes("from bar"))
                    .forEach(notification => {
                        const notificationDiv = document.createElement("div");
                        notificationDiv.classList.add("notification-item");
                        notificationDiv.innerHTML = `
                            <p>${notification.message}</p>
                            <small>Received at: ${notification.sent_at}</small>
                            <button class="delete-notification-button" data-id="${notification.notification_id}">OK</button>
                        `;
                        notificationsContainer.appendChild(notificationDiv);
    
                        notificationDiv.querySelector(".delete-notification-button").addEventListener("click", async () => {
                            await deleteNotification(notification.notification_id);
                            fetchNotifications(userId);
                        });
                    });
            } else {
                console.error("Failed to fetch notifications.");
            }
        } catch (error) {
            console.error("Error fetching notifications:", error);
        }
    }
    
    
    async function deleteNotification(notificationId) {
        try {
            const response = await fetch("http://localhost:9090/notifications", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "DELETE_NOTIFICATION", notification_id: notificationId }),
            });
    
            if (response.ok) {
                console.log(`Notification ${notificationId} deleted successfully.`);
            } else {
                console.error(`Failed to delete notification ${notificationId}.`);
            }
        } catch (error) {
            console.error("Error deleting notification:", error);
        }
    }
    

   
    const userId = localStorage.getItem("userId"); 
    setInterval(() => {
        if (userId) {
            fetchOrders();
            fetchNotifications(userId);
        }
    }, 5000); 
    

    fetchMenu();
    fetchOrders();
});
