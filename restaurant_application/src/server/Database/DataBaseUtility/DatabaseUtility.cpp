#include "DatabaseUtility.h"
#include "../DataBase.h"

DatabaseUtility::DatabaseUtility(Database& databaseInstance) : database(databaseInstance) {}

bool DatabaseUtility::isOrderFullyReady(int orderId, char type) {
    const char* sqlQuery = R"(
        SELECT COUNT(*) 
        FROM OrderItems oi
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE oi.order_id = ? AND mi.type = ? AND oi.status != 'ready'
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    int remainingItems = 0;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);
        sqlite3_bind_text(stmt, 2, std::string(1, type).c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            remainingItems = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return remainingItems == 0;
}

bool DatabaseUtility::updateOrderStatusForType(int orderId, char type, const std::string& status) {
    const char* sqlQuery = R"(
        UPDATE OrderItems
        SET status = ?
        WHERE order_id = ? AND menu_item_id IN (
            SELECT menu_item_id FROM MenuItems WHERE Type = ?
        )
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, orderId);
        sqlite3_bind_text(stmt, 3, std::string(1, type).c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Failed to update order items: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}

int DatabaseUtility::getOrderUserId(int orderId) {
    const char* sqlQuery = "SELECT user_id FROM Commandes WHERE commande_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    int userId = -1;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            userId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return userId;
}

int DatabaseUtility::getOrderTableId(int orderId) {
    const char* sqlQuery = "SELECT table_id FROM Commandes WHERE commande_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    int tableId = -1;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            tableId = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error fetching table ID: " << sqlite3_errmsg(db) << std::endl;
    }

    return tableId;
}

std::vector<std::pair<std::string, int>> DatabaseUtility::getOrderDishes(int orderId) {
    const char* sqlQuery = R"(
        SELECT mi.name, oi.quantity
        FROM OrderItems oi
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE oi.order_id = ? AND mi.Type = 'f' AND oi.status = 'pending'
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    std::vector<std::pair<std::string, int>> dishes;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string dishName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            dishes.emplace_back(dishName, quantity);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error fetching order dishes: " << sqlite3_errmsg(db) << std::endl;
    }

    return dishes;
}

std::vector<std::pair<std::string, int>> DatabaseUtility::getBarOrderDishes(int orderId) {
    const char* sqlQuery = R"(
        SELECT mi.name, oi.quantity
        FROM OrderItems oi
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE oi.order_id = ? AND mi.Type = 'D'
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    std::vector<std::pair<std::string, int>> drinks;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string drinkName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            drinks.emplace_back(drinkName, quantity);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error fetching bar order dishes: " << sqlite3_errmsg(db) << std::endl;
    }

    return drinks;
}

std::vector<std::pair<std::string, int>> DatabaseUtility::getOrderDishesForNotificationKitchen(int orderId) {
    const char* sqlQuery = R"(
        SELECT mi.name, oi.quantity
        FROM OrderItems oi
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE oi.order_id = ? AND mi.Type = 'f' 
    )";
    sqlite3_stmt* stmt;
    sqlite3* db = database.getDbHandle();
    std::vector<std::pair<std::string, int>> dishes;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string dishName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            int quantity = sqlite3_column_int(stmt, 1);
            dishes.emplace_back(dishName, quantity);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error fetching order dishes: " << sqlite3_errmsg(db) << std::endl;
    }

    return dishes;
}