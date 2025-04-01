#ifndef DATABASEMENU_H
#define DATABASEMENU_H

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

class Database; // Forward declaration to avoid circular inclusion

/**
 * @class DatabaseMenu
 * @brief Provides functionalities to manage the restaurant's menu.
 *
 * This class allows retrieval, addition, and deletion of menu items.
 * It interacts with the database to perform these operations.
 */
class DatabaseMenu {
public:
    /**
     * @brief Constructs a DatabaseMenu object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the menu management functionality with access to the database.
     */
    DatabaseMenu(Database& databaseInstance);

    /**
     * @brief Retrieves the full menu.
     * @return A vector of tuples, where each tuple contains:
     *         - Item ID (int)
     *         - Item name (std::string)
     *         - Item price (double)
     *         - Item type (char: e.g., 'F' for food, 'D' for drink)
     */
    std::vector<std::tuple<int, std::string, double, char>> getMenu();

    /**
     * @brief Retrieves the menu for waiters (without item types).
     * @return A vector of tuples, where each tuple contains:
     *         - Item ID (int)
     *         - Item name (std::string)
     *         - Item price (double)
     */
    std::vector<std::tuple<int, std::string, double>> getMenu_waiter();

    /**
     * @brief Adds a new item to the menu.
     * @param name The name of the new menu item.
     * @param price The price of the new menu item.
     * @param type The type of the menu item (e.g., 'F' for food, 'D' for drink).
     * @return True if the item was added successfully, false otherwise.
     */
    bool addMenuItem(const std::string& name, double price, char type);

    /**
     * @brief Deletes an item from the menu.
     * @param itemId The ID of the item to delete.
     * @return True if the item was deleted successfully, false otherwise.
     */
    bool deleteMenuItem(int itemId);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEMENU_H