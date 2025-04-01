#include "HandleOrder.h"

HandleOrder::HandleOrder(Database & database) : dbOrder(database), dbUtility(database), dbNotification(database), dbRevenue(database) {}


void HandleOrder::handlePlaceOrder(Poco::Net::StreamSocket& socketStream, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(socketStream);

    if (!json->has("table_number") || !json->has("items") || !json->has("user_id")) {
        sendErrorResponse(stream, "Missing required fields: 'table_number', 'items', 'user_id'", 400);
        return;
    }

    int tableNumber = json->getValue<int>("table_number");
    int userId = json->getValue<int>("user_id");
    Poco::JSON::Array::Ptr itemsArray = json->getArray("items");

    std::vector<std::tuple<int, int>> items; 
    for (size_t i = 0; i < itemsArray->size(); ++i) {
        Poco::JSON::Object::Ptr item = itemsArray->getObject(i);
        int menuItemId = item->getValue<int>("id");
        int quantity = item->getValue<int>("quantity");
        items.emplace_back(menuItemId, quantity);
    }

    
    double totalPrice = dbOrder.addOrder(userId, tableNumber, items);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("status", "success");
    response->set("message", "Order placed successfully.");
    response->set("total_price", totalPrice);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleUpdateOrderStatus(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(clientSocket);

    if (!json->has("order_id") || !json->has("status") || !json->has("type")) {
        sendErrorResponse(stream, "Missing required fields: 'order_id', 'status', or 'type'", 400);
        return;
    }

    int orderId = json->getValue<int>("order_id");
    std::string status = json->getValue<std::string>("status");
    std::string type = json->getValue<std::string>("type");
    char typeChar = (type == "kitchen") ? 'f' : (type == "bar" ? 'D' : '\0');

    if (typeChar == '\0') {
        sendErrorResponse(stream, "Invalid type provided", 400);
        return;
    }

    // Update the status of order items for the specified type (e.g., kitchen or bar)
    bool success = dbUtility.updateOrderStatusForType(orderId, typeChar, status); 

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        dbOrder.updateOrderStatusIfReady(orderId);
        // Check if all items of the specified type are ready
        if (dbUtility.isOrderFullyReady(orderId, typeChar)) {
            response->set("status", "success");
            response->set("message", "Order status updated successfully. All items are ready.");
            
            // Notify the user that their order is ready
            int userId = dbUtility.getOrderUserId(orderId);
            int tableId = dbUtility.getOrderTableId(orderId);
            auto dishes = dbUtility.getOrderDishes(orderId);
            // Fetch only the food items for the order
            auto foodDishes = dbUtility.getOrderDishesForNotificationKitchen(orderId);

            // Format the dishes into a readable string
            std::string dishDetails = formatDishes(foodDishes);

            // Construct the notification message
            std::string message = "Table " + std::to_string(tableId) + ": " + dishDetails + " are ready from kitchen.";

            if (userId > 0 && !message.empty()) {
                dbNotification.addNotification(orderId, message, userId);
            }

        } else {
            response->set("status", "success");
            response->set("message", "Order status updated successfully.");
        }
    } else {
        response->set("status", "error");
        response->set("message", "Failed to update order status.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleGetOrders(Poco::Net::StreamSocket& clientSocket) {
   auto orders = dbOrder.getOrders();

    std::cout << "Fetched " << orders.size() << " orders from database." << std::endl;

    for (const auto& order : orders) {
        std::cout << "Order ID: " << std::get<0>(order)
                  << ", User ID: " << std::get<1>(order)
                  << ", Table ID: " << std::get<2>(order)
                  << ", Dishes: " << formatDishes(std::get<3>(order))
                  << ", Status: " << std::get<4>(order) << std::endl;
    }

    Poco::JSON::Array::Ptr ordersArray = new Poco::JSON::Array();
    for (const auto& order : orders) {
        Poco::JSON::Object::Ptr orderObject = new Poco::JSON::Object();
        orderObject->set("commande_id", std::get<0>(order));
        orderObject->set("user_id", std::get<1>(order));
        orderObject->set("table_id", std::get<2>(order));
        orderObject->set("order_status", std::get<4>(order));

        
        Poco::JSON::Array::Ptr itemsArray = new Poco::JSON::Array();
        for (const auto& dish : std::get<3>(order)) {
            Poco::JSON::Object::Ptr dishObject = new Poco::JSON::Object();
            dishObject->set("name", dish.first);
            dishObject->set("quantity", dish.second);
            itemsArray->add(dishObject);
        }
        orderObject->set("items", itemsArray);
        ordersArray->add(orderObject);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("orders", ordersArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleOrder::handleGetOrderHistory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {

    std::string startDate = json->getValue<std::string>("start_date");
    std::string endDate = json->getValue<std::string>("end_date");

    // Fetch order history
    auto history = dbRevenue.getOrderHistory(startDate, endDate);
    Poco::JSON::Array::Ptr historyArray = new Poco::JSON::Array();

    // Populate the array
    for (const auto& entry : history) {
        Poco::JSON::Object::Ptr historyItem = new Poco::JSON::Object();
        historyItem->set("history_id", std::get<0>(entry));
        historyItem->set("commande_id", std::get<1>(entry));
        historyItem->set("user_id", std::get<2>(entry));
        historyItem->set("numeroTable", std::get<3>(entry));
        historyItem->set("commande_status", std::get<4>(entry));
        historyItem->set("sub_total", std::get<5>(entry));
        historyItem->set("created_at", std::get<6>(entry));
        historyArray->add(historyItem);
    }

    // Create the response object
    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    
    response->set("order_history", historyArray);

    // Send response using socketStream directly
    Poco::Net::SocketStream socketStream(clientSocket);
    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleOrder::handleGetOrders_kitchen(Poco::Net::StreamSocket& clientSocket) {
    auto orders = dbOrder.getOrders_kitchen();

    std::cout << "Fetched " << orders.size() << " orders from database." << std::endl;

    for (const auto& order : orders) {
        std::cout << "Order ID: " << std::get<0>(order)
                  << ", User ID: " << std::get<1>(order)
                  << ", Table ID: " << std::get<2>(order)
                  << ", Dishes: " << formatDishes_kitchen(std::get<3>(order))
                  << ", Status: " << std::get<4>(order) << std::endl;
    }

    Poco::JSON::Array::Ptr ordersArray = new Poco::JSON::Array();
    for (const auto& order : orders) {
        Poco::JSON::Object::Ptr orderObject = new Poco::JSON::Object();
        orderObject->set("commande_id", std::get<0>(order));
        orderObject->set("user_id", std::get<1>(order));
        orderObject->set("table_id", std::get<2>(order));
        orderObject->set("order_status", std::get<4>(order));

        Poco::JSON::Array::Ptr itemsArray = new Poco::JSON::Array();
        for (const auto& dish : std::get<3>(order)) {
            Poco::JSON::Object::Ptr dishObject = new Poco::JSON::Object();
            dishObject->set("name", std::get<0>(dish));
            dishObject->set("quantity", std::get<1>(dish));
            dishObject->set("type", std::string(1, std::get<2>(dish))); 
            itemsArray->add(dishObject);
        }
        orderObject->set("items", itemsArray);
        ordersArray->add(orderObject);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("orders", ordersArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleOrder::handleGetOrders_bar(Poco::Net::StreamSocket& clientSocket) {
    auto orders = dbOrder.getOrders_bar(); // Fetch bar-specific orders from the database

    Poco::JSON::Array::Ptr ordersArray = new Poco::JSON::Array();
    for (const auto& order : orders) {
        Poco::JSON::Object::Ptr orderObject = new Poco::JSON::Object();
        orderObject->set("commande_id", std::get<0>(order));
        orderObject->set("user_id", std::get<1>(order));
        orderObject->set("table_id", std::get<2>(order));
        orderObject->set("order_status", std::get<4>(order));

        // Add drinks to the items array
        Poco::JSON::Array::Ptr itemsArray = new Poco::JSON::Array();
        for (const auto& drink : std::get<3>(order)) {
            Poco::JSON::Object::Ptr drinkObject = new Poco::JSON::Object();
            drinkObject->set("name", std::get<0>(drink)); // Drink name
            drinkObject->set("quantity", std::get<1>(drink)); // Quantity
            drinkObject->set("type", std::string(1, std::get<2>(drink))); // Type (e.g., 'D')
            itemsArray->add(drinkObject);
        }
        orderObject->set("items", itemsArray);
        ordersArray->add(orderObject);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("orders", ordersArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleOrder::UpdateOrderStatusBar(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(clientSocket);

    int orderId = json->getValue<int>("order_id");
    std::string status = json->getValue<std::string>("status");

    // Update the status of bar-related order items (Type = 'D')
    bool success = dbUtility.updateOrderStatusForType(orderId, 'D', status);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        dbOrder.updateOrderStatusIfReady(orderId);

        // Check if all drink items are ready
        if (dbUtility.isOrderFullyReady(orderId, 'D')) {
            response->set("status", "success");
            response->set("message", "Order status updated successfully. All drink items are ready.");

            // Notify the waiter that the drink order is ready
            int userId = dbUtility.getOrderUserId(orderId);
            int tableId = dbUtility.getOrderTableId(orderId);
            auto drinks = dbUtility.getBarOrderDishes(orderId); // Get only drink-related items

            std::string message = "Table " + std::to_string(tableId) + ": " + formatDishes(drinks) + " are ready from bar.";

            if (userId > 0 && !message.empty()) {
                dbNotification.addNotification(orderId, message, userId);
            }
        } else {
            response->set("status", "success");
            response->set("message", "Order status updated successfully.");
        }
    } else {
        response->set("status", "error");
        response->set("message", "Failed to update order status.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleUpdateOrderStatus_waiter(Poco::Net::StreamSocket& socketStream, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(socketStream);

    if (!json->has("order_id") || !json->has("status")) {
        sendErrorResponse(stream, "Missing required fields: 'order_id', 'status'", 400);
        return;
    }

    int orderId = json->getValue<int>("order_id");
    std::string status = json->getValue<std::string>("status");

    bool success = dbOrder.updateOrderStatus(orderId, status);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        response->set("status", "success");
        response->set("message", "Order status updated successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to update order status.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleArchiveOrder(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(clientSocket);

    int orderId = json->getValue<int>("order_id");

    bool statusUpdated = dbOrder.updateOrderStatus(orderId, "served");
    if (!statusUpdated) {
        sendErrorResponse(stream, "Failed to update order status to 'served'", 500);
        return;
    }

    bool archived = dbOrder.archiveOrder(orderId);
    dbOrder.deleteOrderItemsByOrderId(orderId);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (archived) {
        response->set("status", "success");
        response->set("message", "Order archived successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to archive order.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
}

void HandleOrder::sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code) {
    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("error", error);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 " << code << " Error\r\n"
           << "Access-Control-Allow-Origin: *\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleDeleteNotification(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream stream(clientSocket);

    int notificationId = json->getValue<int>("notification_id");
    bool success = dbNotification.deleteNotification(notificationId);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        response->set("status", "success");
        response->set("message", "Notification deleted successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to delete notification.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

void HandleOrder::handleGetNotifications(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    int userId = json->getValue<int>("user_id");
    auto notifications = dbNotification.getNotificationsForUser(userId);

    Poco::JSON::Array::Ptr notificationsArray = new Poco::JSON::Array();
    for (const auto& notification : notifications) {
        Poco::JSON::Object::Ptr notif = new Poco::JSON::Object();
        notif->set("notification_id", std::get<0>(notification));
        notif->set("order_id", std::get<1>(notification));
        notif->set("message", std::get<2>(notification));
        notif->set("sent_at", std::get<3>(notification));
        notificationsArray->add(notif);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("notifications", notificationsArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream stream(clientSocket);
    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}

std::string HandleOrder::formatDishes(const std::vector<std::pair<std::string, int>>& dishes) {
    std::ostringstream oss;
    for (const auto& dish : dishes) {
        oss << dish.first << " x" << dish.second << ", ";
    }
    std::string result = oss.str();
    if (!result.empty()) {
        result.pop_back(); 
        result.pop_back(); 
    }
    return result;
}

std::string HandleOrder::formatDishes_kitchen(const std::vector<std::tuple<std::string, int, char>>& dishes) {
    std::ostringstream oss;
    for (const auto& dish : dishes) {
        oss << std::get<0>(dish) << " x" << std::get<1>(dish) 
            << " (" << (std::get<2>(dish) == 'f' ? "food" : "other") << "), ";
    }
    std::string result = oss.str();
    if (!result.empty()) {
        result.pop_back(); 
        result.pop_back(); 
    }
    return result;
}