#ifndef DATABASEUTILITY_H
#define DATABASEUTILITY_H

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
 * @class DatabaseUtility
 * @brief Provides utility functions for managing and querying order-related data in the database.
 *
 * This class includes methods to check order statuses, update statuses, retrieve user or table IDs 
 * associated with orders, and retrieve dish details for various purposes.
 */
class DatabaseUtility {
public:
    /**
     * @brief Constructs a DatabaseUtility object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the utility functions with access to the database.
     */
    DatabaseUtility(Database& databaseInstance);

    /**
     * @brief Checks if an order is fully ready.
     * @param orderId The ID of the order to check.
     * @param type The type of order (e.g., kitchen or bar).
     * @return True if the order is fully ready, false otherwise.
     */
    bool isOrderFullyReady(int orderId, char type);

    /**
     * @brief Updates the status of an order for a specific type.
     * @param orderId The ID of the order to update.
     * @param type The type of the order (e.g., kitchen or bar).
     * @param status The new status to set for the order.
     * @return True if the status was updated successfully, false otherwise.
     */
    bool updateOrderStatusForType(int orderId, char type, const std::string& status);

    /**
     * @brief Retrieves the user ID associated with a specific order.
     * @param orderId The ID of the order.
     * @return The user ID associated with the order.
     */
    int getOrderUserId(int orderId);

    /**
     * @brief Retrieves the table ID associated with a specific order.
     * @param orderId The ID of the order.
     * @return The table ID associated with the order.
     */
    int getOrderTableId(int orderId);

    /**
     * @brief Retrieves the list of dishes for a specific order.
     * @param orderId The ID of the order.
     * @return A vector of pairs, where each pair contains the dish name (std::string) and quantity (int).
     */
    std::vector<std::pair<std::string, int>> getOrderDishes(int orderId);

    /**
     * @brief Retrieves the list of bar-specific dishes for a specific order.
     * @param orderId The ID of the order.
     * @return A vector of pairs, where each pair contains the dish name (std::string) and quantity (int).
     */
    std::vector<std::pair<std::string, int>> getBarOrderDishes(int orderId);

    /**
     * @brief Retrieves the list of kitchen-specific dishes for notification purposes.
     * @param orderId The ID of the order.
     * @return A vector of pairs, where each pair contains the dish name (std::string) and quantity (int).
     */
    std::vector<std::pair<std::string, int>> getOrderDishesForNotificationKitchen(int orderId);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEUTILITY_H