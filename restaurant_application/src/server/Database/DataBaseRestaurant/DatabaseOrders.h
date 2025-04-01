#ifndef DATABASEORDERS_H
#define DATABASEORDERS_H

#include "../DataBase.h"
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <Poco/Exception.h> 
#include <unordered_map> 
#include <algorithm>  

class Database;

/**
 * @class DatabaseOrders
 * @brief Manages the restaurant's orders in the database.
 *
 * This class provides functionalities to add, update, retrieve, and archive orders.
 * It interacts with the database to perform these operations.
 */
class DatabaseOrders {
public:
    /**
     * @brief Constructs a DatabaseOrders object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the order management functionality with access to the database.
     */
    DatabaseOrders(Database& databaseInstance);

    /**
     * @brief Adds a new order to the system.
     * @param userId The ID of the user placing the order.
     * @param tableNumber The table number associated with the order.
     * @param items A vector of tuples where each tuple contains:
     *         - Menu item ID (int)
     *         - Quantity of the menu item (int)
     * @return The total cost of the order (double).
     */
    double addOrder(int userId, int tableNumber, const std::vector<std::tuple<int, int>>& items);

    /**
     * @brief Retrieves the price of a menu item.
     * @param menuItemId The ID of the menu item.
     * @return The price of the menu item (double).
     */
    double getMenuItemPrice(int menuItemId);

    /**
     * @brief Updates the status of an existing order.
     * @param orderId The ID of the order whose status needs to be updated.
     * @param status The new status for the order.
     * @return True if the order status was updated successfully, false otherwise.
     */
    bool updateOrderStatus(int orderId, const std::string& status);

    /**
     * @brief Retrieves all orders in the system.
     * @return A vector of tuples where each tuple contains:
     *         - Order ID (int)
     *         - Table number (int)
     *         - User ID (int)
     *         - A vector of pairs where each pair contains:
     *             - Item name (std::string)
     *             - Item quantity (int)
     *         - Order status (std::string)
     */
    std::vector<std::tuple<int, int, int, std::vector<std::pair<std::string, int>>, std::string>> getOrders();

    /**
     * @brief Retrieves orders for the kitchen.
     * @return A vector of tuples where each tuple contains:
     *         - Order ID (int)
     *         - Table number (int)
     *         - User ID (int)
     *         - A vector of tuples where each tuple contains:
     *             - Item name (std::string)
     *             - Item quantity (int)
     *             - Special instructions (char)
     *         - Order status (std::string)
     */
    std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> getOrders_kitchen();

    /**
     * @brief Retrieves orders for the bar.
     * @return A vector of tuples where each tuple contains:
     *         - Order ID (int)
     *         - Table number (int)
     *         - User ID (int)
     *         - A vector of tuples where each tuple contains:
     *             - Item name (std::string)
     *             - Item quantity (int)
     *             - Type of Item F = food, D = drink (char)
     *         - Order status (std::string)
     */
    std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> getOrders_bar();

    /**
     * @brief Updates the status of an order if it is ready.
     * @param orderId The ID of the order to check and update.
     * @return True if the order was successfully marked as "ready", false otherwise.
     */
    bool updateOrderStatusIfReady(int orderId);

    /**
     * @brief Archives a completed order.
     * @param orderId The ID of the order to archive.
     * @return True if the order was archived successfully, false otherwise.
     */
    bool archiveOrder(int orderId);

    /**
     * @brief Deletes items from an order by order ID.
     * @param orderId The ID of the order whose items need to be deleted.
     * @return True if the items were successfully deleted, false otherwise.
     */
    bool deleteOrderItemsByOrderId(int orderId);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEORDERS_H