#include "DatabaseMenu.h"
#include "../DataBase.h"

DatabaseMenu::DatabaseMenu(Database& databaseInstance) : database(databaseInstance) {}

std::vector<std::tuple<int, std::string, double, char>> DatabaseMenu::getMenu() {
    std::vector<std::tuple<int, std::string, double, char>> menu;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* sqlQuery = "SELECT menu_item_id, name, price, type FROM MenuItems";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            double price = sqlite3_column_double(stmt, 2);
            char type = static_cast<char>(sqlite3_column_text(stmt, 3)[0]); // Retrieve type as a char
            menu.emplace_back(id, name, price, type);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return menu;
}

std::vector<std::tuple<int, std::string, double>> DatabaseMenu::getMenu_waiter() {
    std::vector<std::tuple<int, std::string, double>> menuItems;
    const char* sqlQuery = "SELECT menu_item_id, name, price FROM MenuItems";

    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            double price = sqlite3_column_double(stmt, 2);
            menuItems.emplace_back(id, name, price);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to retrieve menu: " << sqlite3_errmsg(db) << std::endl;
    }

    return menuItems;
}

bool DatabaseMenu::addMenuItem(const std::string& name, double price, char type) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* sqlQuery = "INSERT INTO MenuItems (name, price, type) VALUES (?, ?, ?)";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, price);
        sqlite3_bind_text(stmt, 3, std::string(1, type).c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }
    return false;
}

bool DatabaseMenu::deleteMenuItem(int itemId) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    const char* sqlQuery = "DELETE FROM MenuItems WHERE menu_item_id = ?";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        
        sqlite3_bind_int(stmt, 1, itemId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true; 
        } else {
            std::cerr << "Error deleting menu item: " << sqlite3_errmsg(db) << std::endl;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing delete query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false; 
}