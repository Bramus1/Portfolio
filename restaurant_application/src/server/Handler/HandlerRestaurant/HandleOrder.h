#ifndef HANDLEORDER_H
#define HANDLEORDER_H

#include "../../Database/DataBaseRestaurant/DatabaseOrders.h"
#include "../../Database/DataBaseUtility/DatabaseUtility.h"
#include "../../Database/DataBaseUtility/DatabaseNotification.h"

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <fstream>
#include <iostream>
#include <Poco/JSON/Array.h>
#include <sstream> 
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <string>
#include <Poco/JSON/Object.h>

/**
 * @class HandleOrder
 * @brief Manages order-related operations and communication with clients.
 *
 * This class provides methods to handle API requests for placing, updating, retrieving, 
 * and archiving orders, as well as managing notifications. It interacts with the relevant 
 * database modules for these operations.
 */
class HandleOrder {
public:
    /**
     * @brief Constructs a HandleOrder object.
     * @param database Reference to the main Database instance.
     *
     * Initializes the required database objects for order management operations.
     */
    HandleOrder(Database & database);

    /**
     * @brief Handles a request to place a new order.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the order details.
     */
    void handlePlaceOrder(Poco::Net::StreamSocket& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to update the status of an order.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the updated status details.
     */
    void handleUpdateOrderStatus(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to retrieve current orders.
     * @param clientSocket Socket for the connected client.
     *
     * Sends a list of active orders to the client in JSON format.
     */
    void handleGetOrders(Poco::Net::StreamSocket& clientSocket);

    /**
     * @brief Handles a request to retrieve the order history.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing filters for retrieving the order history.
     */
    void handleGetOrderHistory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to retrieve orders for the kitchen.
     * @param clientSocket Socket for the connected client.
     */
    void handleGetOrders_kitchen(Poco::Net::StreamSocket& clientSocket);

    /**
     * @brief Handles a request to retrieve orders for the bar.
     * @param clientSocket Socket for the connected client.
     */
    void handleGetOrders_bar(Poco::Net::StreamSocket& clientSocket);

    /**
     * @brief Updates the status of an order at the bar.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the updated status details.
     */
    void UpdateOrderStatusBar(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to update an order's status for a waiter.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the updated status details.
     */
    void handleUpdateOrderStatus_waiter(Poco::Net::StreamSocket& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to archive an order.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the order details to be archived.
     */
    void handleArchiveOrder(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Sends an error response to the client.
     * @param stream Socket stream to write the error response.
     * @param error Error message.
     * @param code HTTP status code of the error.
     */
    void sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code);

    /**
     * @brief Handles a request to delete a notification.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the notification details to be deleted.
     */
    void handleDeleteNotification(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to retrieve notifications.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing filters for retrieving notifications.
     */
    void handleGetNotifications(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Formats a list of dishes for display.
     * @param dishes Vector of pairs containing dish names and quantities.
     * @return A formatted string representing the dishes and their quantities.
     */
    std::string formatDishes(const std::vector<std::pair<std::string, int>>& dishes);

    /**
     * @brief Formats a list of dishes for the kitchen.
     * @param dishes Vector of tuples containing dish names, quantities, and statuses.
     * @return A formatted string representing the dishes, quantities, and statuses.
     */
    std::string formatDishes_kitchen(const std::vector<std::tuple<std::string, int, char>>& dishes);

private:

    DatabaseOrders dbOrder; ///< Instance of DatabaseOrders for order-related database operations.
    DatabaseUtility dbUtility; ///< Instance of DatabaseUtility for utility operations.
    DatabaseNotifications dbNotification; ///< Instance of DatabaseNotifications for notification-related operations.
    DatabaseRevenue dbRevenue; ///< Instance of DatabaseRevenue for revenue-related operations.
};

#endif // HANDLEORDER_H