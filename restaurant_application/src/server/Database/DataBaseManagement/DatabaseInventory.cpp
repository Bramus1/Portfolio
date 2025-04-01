#include "DatabaseInventory.h"
#include "../DataBase.h"

DatabaseInventory::DatabaseInventory(Database& databaseInstance) : database(databaseInstance) {}

std::vector<std::tuple<int, std::string, int, std::string, int, std::string>> DatabaseInventory::getInventory() {
    std::vector<std::tuple<int, std::string, int, std::string, int, std::string>> inventory;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* sqlQuery = "SELECT inventory_id, item_name, quantity, unit, min_required, last_updated FROM Inventory";
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int quantity = sqlite3_column_int(stmt, 2);
            std::string unit = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int minRequired = sqlite3_column_int(stmt, 4);
            std::string lastUpdated = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

            inventory.emplace_back(id, name, quantity, unit, minRequired, lastUpdated);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return inventory;
}

bool DatabaseInventory::addInventoryItem(const std::string& name, int quantity, const std::string& unit, int minRequired) {
    const char* sqlQuery = "INSERT INTO Inventory (item_name, quantity, unit, min_required, last_updated) VALUES (?, ?, ?, ?, datetime('now'))";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, quantity);
        sqlite3_bind_text(stmt, 3, unit.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, minRequired);

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

bool DatabaseInventory::updateInventoryItem(int inventoryId, const std::string& itemName, int quantity,
                                   const std::string& unit, int minRequired) {
    const char* sqlQuery = R"(
        UPDATE Inventory
        SET item_name = ?, quantity = ?, unit = ?, min_required = ?, last_updated = datetime('now')
        WHERE inventory_id = ?
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, itemName.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, quantity);
        sqlite3_bind_text(stmt, 3, unit.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 4, minRequired);
        sqlite3_bind_int(stmt, 5, inventoryId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            std::cout << "Inventory item with ID " << inventoryId << " updated successfully." << std::endl;
            return true;
        } else {
            std::cerr << "Error updating inventory item: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing update query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}

bool DatabaseInventory::deleteInventoryItem(int inventoryId) {
    const char* sqlQuery = "DELETE FROM Inventory WHERE inventory_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, inventoryId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            std::cout << "Inventory item with ID " << inventoryId << " deleted successfully." << std::endl;
            return true;
        } else {
            std::cerr << "Error deleting inventory item: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing delete query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}