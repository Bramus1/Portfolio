document.addEventListener("DOMContentLoaded", () => {
    const ordersTable = document.getElementById("ordersTable").querySelector("tbody");

    async function fetchBarOrders() {
        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_ORDERS_BAR" }), // Command to fetch bar orders
            });

            if (!response.ok) throw new Error("Failed to fetch bar orders.");

            const data = await response.json();
            if (!data.orders || !Array.isArray(data.orders)) {
                throw new Error("Invalid orders data received.");
            }

            renderBarOrders(data.orders);
        } catch (error) {
            console.error("Error fetching bar orders:", error);
            alert("Failed to load bar orders. Please try again.");
        }
    }

    function renderBarOrders(orders) {
        ordersTable.innerHTML = ""; // Clear existing rows
    
        orders
            .filter(order => 
                order.items.some(item => item.type === "D" && item.status !== "ready") // Exclude ready items
            )
            .forEach(order => {
                const row = document.createElement("tr");
    
                const drinks = order.items
                    .filter(item => item.type === "D" && item.status !== "ready") // Filter items for drinks that are not ready
                    .map(item => `${item.name} x${item.quantity}`)
                    .join(", ");
    
                row.innerHTML = `
                    <td>${order.commande_id}</td>
                    <td>${order.table_id}</td>
                    <td>${drinks}</td>
                    <td>
                        <button class="readyButton" data-id="${order.commande_id}">Ready</button>
                    </td>
                `;
    
                ordersTable.appendChild(row);
            });
    
        // Add event listeners for "Ready" buttons
        document.querySelectorAll(".readyButton").forEach(button => {
            button.addEventListener("click", () => {
                const orderId = button.getAttribute("data-id");
                markOrderAsReady(orderId);
            });
        });
    }
    

    async function markOrderAsReady(orderId) {
        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    command: "UPDATE_ORDER_STATUS_BAR",
                    order_id: orderId,
                    type: "bar", // Specify this is for the bar
                    status: "ready",
                }),
            });
    
            if (!response.ok) throw new Error("Failed to update order status.");
    
            const data = await response.json();
            if (data.status === "success") {
                alert(`Order ${orderId} marked as ready. Notification sent to waiter.`);
                fetchBarOrders(); 
            } else {
                alert(data.message || "Failed to update order status.");
            }
        } catch (error) {
            console.error("Error updating order status:", error);
            alert("An error occurred while updating the order status. Please try again.");
        }
    }
    
    setInterval(() => {
        fetchBarOrders();
    }, 5000); 

    // Initial fetch
    fetchBarOrders();
});
