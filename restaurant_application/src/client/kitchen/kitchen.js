document.addEventListener("DOMContentLoaded", () => {
    const ordersTable = document.getElementById("ordersTable").querySelector("tbody");

    async function fetchOrders() {
        try {
            const response = await fetch("http://localhost:9090/orders", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ command: "GET_ORDERS_KITCHEN" }),
            });
    
            if (!response.ok) throw new Error("Failed to fetch orders.");
    
            const data = await response.json();
            console.log("Orders fetched:", data.orders); 
            if (!data.orders || !Array.isArray(data.orders)) {
                throw new Error("Invalid orders data received.");
            }
    
            renderOrders(data.orders);
        } catch (error) {
            console.error("Error fetching orders:", error);
            alert("Failed to load orders. Please try again.");
        }
    }
    
    function renderOrders(orders) {
        ordersTable.innerHTML = ""; // Clear existing rows

        orders
            .filter(order =>
                order.items.some(item => item.type === "f") // Only orders with type 'f'
            )
            .forEach(order => {
                const row = document.createElement("tr");

                const dishes = order.items
                    .filter(item => item.type === "f") // Filter items with type 'f'
                    .map(item => `${item.name} x${item.quantity}`)
                    .join(", ");

                row.innerHTML = `
                    <td>${order.commande_id}</td>
                    <td>${order.table_id}</td>
                    <td>${dishes}</td>
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
                    command: "UPDATE_ORDER_STATUS",
                    order_id: orderId,
                    type: "kitchen", // Specify that this is for the kitchen
                    status: "ready",
                }),
            });

            if (!response.ok) throw new Error("Failed to update order status.");

            const data = await response.json();
            if (data.status === "success") {
                alert(`Order ${orderId} marked as ready. Notification sent to waiter.`);
                fetchOrders();
            } else {
                alert(data.message || "Failed to update order status.");
            }
            
        } catch (error) {
            console.error("Error updating order status:", error);
            alert("An error occurred while updating the order status. Please try again.");
        }
    }


    setInterval(() => {
        fetchOrders();
    }, 5000); 

    // Initial fetch
    fetchOrders();
});
