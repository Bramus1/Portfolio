#ifndef DATABASEINVENTORY_H
#define DATABASEINVENTORY_H

#include "../DataBase.h"
#include <string>
#include <vector>
#include <tuple>
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
 * @class DatabaseInventory
 * @brief Manages the restaurant's inventory in the database.
 *
 * This class provides functionalities to retrieve, add, update, and delete inventory items.
 * It interacts with the database to perform these operations.
 */
class DatabaseInventory {
public:
    /**
     * @brief Constructs a DatabaseInventory object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the inventory management functionality with access to the database.
     */
    DatabaseInventory(Database& databaseInstance);

    /**
     * @brief Retrieves the full inventory.
     * @return A vector of tuples where each tuple contains:
     *         - Inventory ID (int)
     *         - Item name (std::string)
     *         - Item quantity (int)
     *         - Unit of measurement (std::string)
     *         - Minimum required quantity (int)
     *         - Date when Item was added (std::string)
     */
    std::vector<std::tuple<int, std::string, int, std::string, int, std::string>> getInventory();

    /**
     * @brief Adds a new item to the inventory.
     * @param name The name of the new inventory item.
     * @param quantity The quantity of the new inventory item.
     * @param unit The unit of measurement for the item (e.g., "kg", "liters").
     * @param minRequired The minimum required quantity of the item.
     * @return True if the item was added successfully, false otherwise.
     */
    bool addInventoryItem(const std::string& name, int quantity, const std::string& unit, int minRequired);

    /**
     * @brief Updates an existing inventory item.
     * @param inventoryId The ID of the inventory item to update.
     * @param itemName The new name of the inventory item.
     * @param quantity The new quantity of the item.
     * @param unit The new unit of measurement for the item.
     * @param minRequired The new minimum required quantity of the item.
     * @return True if the item was updated successfully, false otherwise.
     */
    bool updateInventoryItem(int inventoryId, const std::string& itemName, int quantity, const std::string& unit, int minRequired);

    /**
     * @brief Deletes an item from the inventory.
     * @param inventoryId The ID of the inventory item to delete.
     * @return True if the item was deleted successfully, false otherwise.
     */
    bool deleteInventoryItem(int inventoryId);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEINVENTORY_H