#include "DatabaseOrders.h"
#include "../DataBase.h"

#include <cstring>

DatabaseOrders::DatabaseOrders(Database& databaseInstance) : database(databaseInstance) {}

double DatabaseOrders::addOrder(int userId, int tableNumber, const std::vector<std::tuple<int, int>>& items) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    double totalPrice = 0.0;

    
    const char* insertCommandeQuery = R"(
        INSERT INTO Commandes (user_id, table_id, order_status, total_price, created_at)
        VALUES (?, ?, 'coocking', 0, datetime('now'))
    )";

    if (sqlite3_prepare_v2(db, insertCommandeQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        sqlite3_bind_int(stmt, 2, tableNumber);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            int commandeId = sqlite3_last_insert_rowid(db); 
            sqlite3_finalize(stmt);

            
            const char* insertOrderItemQuery = R"(
                INSERT INTO OrderItems (order_id, menu_item_id, quantity, sub_total)
                VALUES (?, ?, ?, ?)
            )";

            if (sqlite3_prepare_v2(db, insertOrderItemQuery, -1, &stmt, nullptr) == SQLITE_OK) {
                for (const auto& item : items) {
                    int menuItemId = std::get<0>(item);
                    int quantity = std::get<1>(item);

                    
                    double price = getMenuItemPrice(menuItemId);
                    double subTotal = price * quantity;
                    totalPrice += subTotal;

                    
                    sqlite3_bind_int(stmt, 1, commandeId);
                    sqlite3_bind_int(stmt, 2, menuItemId);
                    sqlite3_bind_int(stmt, 3, quantity);
                    sqlite3_bind_double(stmt, 4, subTotal);

                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Failed to insert order item: " << sqlite3_errmsg(db) << std::endl;
                    }
                    sqlite3_reset(stmt); 
                }
                sqlite3_finalize(stmt);

                
                const char* updateTotalPriceQuery = "UPDATE Commandes SET total_price = ? WHERE commande_id = ?";
                if (sqlite3_prepare_v2(db, updateTotalPriceQuery, -1, &stmt, nullptr) == SQLITE_OK) {
                    sqlite3_bind_double(stmt, 1, totalPrice);
                    sqlite3_bind_int(stmt, 2, commandeId);
                    if (sqlite3_step(stmt) != SQLITE_DONE) {
                        std::cerr << "Failed to update total price: " << sqlite3_errmsg(db) << std::endl;
                    }
                    sqlite3_finalize(stmt);
                }
            } else {
                std::cerr << "Failed to prepare order item insert query: " << sqlite3_errmsg(db) << std::endl;
            }
        } else {
            std::cerr << "Failed to insert order: " << sqlite3_errmsg(db) << std::endl;
        }
    } else {
        std::cerr << "Failed to prepare order insert query: " << sqlite3_errmsg(db) << std::endl;
    }

    return totalPrice;
}


double DatabaseOrders::getMenuItemPrice(int menuItemId) {
    const char* sqlQuery = "SELECT price FROM MenuItems WHERE menu_item_id = ?";
    
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    double price = 0.0;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, menuItemId);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            price = sqlite3_column_double(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to retrieve menu item price: " << sqlite3_errmsg(db) << std::endl;
    }

    return price;
}

bool DatabaseOrders::updateOrderStatus(int orderId, const std::string& status) {
    const char* sqlQuery = "UPDATE Commandes SET order_status = ? WHERE commande_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, status.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, orderId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Failed to update order status: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare update query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}

std::vector<std::tuple<int, int, int, std::vector<std::pair<std::string, int>>, std::string>> DatabaseOrders::getOrders() {
    std::vector<std::tuple<int, int, int, std::vector<std::pair<std::string, int>>, std::string>> orders;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* query = R"(
        SELECT c.commande_id, c.user_id, c.table_id, mi.name, oi.quantity, c.order_status
        FROM Commandes c
        JOIN OrderItems oi ON c.commande_id = oi.order_id
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
    )";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        // Map to store dishes for each order ID
        std::unordered_map<int, std::vector<std::pair<std::string, int>>> dishesMap;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int commandeId = sqlite3_column_int(stmt, 0);
            int userId = sqlite3_column_int(stmt, 1);
            int tableId = sqlite3_column_int(stmt, 2);
            std::string dishName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int quantity = sqlite3_column_int(stmt, 4);
            std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

            // Add dish details to the map
            dishesMap[commandeId].emplace_back(dishName, quantity);

            // Check if the order is already in the vector
            auto it = std::find_if(orders.begin(), orders.end(),
                [commandeId](const auto& order) {
                    return std::get<0>(order) == commandeId;
                });

            if (it == orders.end()) {
                // Add a new order if it doesn't exist
                orders.emplace_back(commandeId, userId, tableId, dishesMap[commandeId], status);
            } else {
                // Update the dishes vector for the existing order
                std::get<3>(*it) = dishesMap[commandeId];
            }
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to fetch orders: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "Fetched " << orders.size() << " orders from database." << std::endl;

    return orders;
}

std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> DatabaseOrders::getOrders_kitchen() {
    std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> orders;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* query = R"(
        SELECT c.commande_id, c.user_id, c.table_id, mi.name, oi.quantity, mi.Type, c.order_status
        FROM Commandes c
        JOIN OrderItems oi ON c.commande_id = oi.order_id
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE mi.Type = 'f' AND oi.status = 'pending'
    )";


    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        std::unordered_map<int, std::vector<std::tuple<std::string, int, char>>> dishesMap;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int commandeId = sqlite3_column_int(stmt, 0);
            int userId = sqlite3_column_int(stmt, 1);
            int tableId = sqlite3_column_int(stmt, 2);
            std::string dishName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int quantity = sqlite3_column_int(stmt, 4);
            const char* typeText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            char type = (typeText != nullptr && strlen(typeText) > 0) ? typeText[0] : '\0';
            std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

            dishesMap[commandeId].emplace_back(dishName, quantity, type);

            auto it = std::find_if(orders.begin(), orders.end(),
                [commandeId](const auto& order) {
                    return std::get<0>(order) == commandeId;
                });

            if (it == orders.end()) {
                orders.emplace_back(commandeId, userId, tableId, dishesMap[commandeId], status);
            } else {
                std::get<3>(*it) = dishesMap[commandeId];
            }
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to fetch orders: " << sqlite3_errmsg(db) << std::endl;
    }

    std::cout << "Fetched " << orders.size() << " orders from database." << std::endl;

    return orders;
}

std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> DatabaseOrders::getOrders_bar() {
    std::vector<std::tuple<int, int, int, std::vector<std::tuple<std::string, int, char>>, std::string>> orders;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* query = R"(
        SELECT c.commande_id, c.user_id, c.table_id, mi.name, oi.quantity, mi.Type, c.order_status
        FROM Commandes c
        JOIN OrderItems oi ON c.commande_id = oi.order_id
        JOIN MenuItems mi ON oi.menu_item_id = mi.menu_item_id
        WHERE mi.Type = 'D' AND oi.status = 'pending'
    )";

    if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) == SQLITE_OK) {
        std::unordered_map<int, std::vector<std::tuple<std::string, int, char>>> drinksMap;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int commandeId = sqlite3_column_int(stmt, 0);
            int userId = sqlite3_column_int(stmt, 1);
            int tableId = sqlite3_column_int(stmt, 2);
            std::string drinkName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            int quantity = sqlite3_column_int(stmt, 4);
            const char* typeText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            char type = (typeText != nullptr && strlen(typeText) > 0) ? typeText[0] : '\0';
            std::string status = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

            drinksMap[commandeId].emplace_back(drinkName, quantity, type);

            auto it = std::find_if(orders.begin(), orders.end(),
                [commandeId](const auto& order) {
                    return std::get<0>(order) == commandeId;
                });

            if (it == orders.end()) {
                orders.emplace_back(commandeId, userId, tableId, drinksMap[commandeId], status);
            } else {
                std::get<3>(*it) = drinksMap[commandeId];
            }
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to fetch bar orders: " << sqlite3_errmsg(db) << std::endl;
    }

    return orders;
}

bool DatabaseOrders::updateOrderStatusIfReady(int orderId) {
    const char* checkQuery = R"(
        SELECT COUNT(*)
        FROM OrderItems
        WHERE order_id = ? AND status != 'ready'
    )";

    const char* updateQuery = R"(
        UPDATE Commandes
        SET order_status = 'ready'
        WHERE commande_id = ?
    )";

    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    int remainingItems = 0;

    
    if (sqlite3_prepare_v2(db, checkQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            remainingItems = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare check query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

   
    if (remainingItems == 0) {
        if (sqlite3_prepare_v2(db, updateQuery, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, orderId);

            if (sqlite3_step(stmt) == SQLITE_DONE) {
                sqlite3_finalize(stmt);
                return true;
            } else {
                std::cerr << "Failed to update order status: " << sqlite3_errmsg(db) << std::endl;
            }
            sqlite3_finalize(stmt);
        } else {
            std::cerr << "Failed to prepare update query: " << sqlite3_errmsg(db) << std::endl;
        }
    }

    return false;
}

bool DatabaseOrders::archiveOrder(int orderId) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    
    const char* insertQuery = R"(
        INSERT INTO OrderHistory (commande_id, user_id, numeroTable, commande_status, sub_total, created_at)
        SELECT commande_id, user_id, table_id, order_status, total_price, datetime('now')
        FROM Commandes
        WHERE commande_id = ?
    )";

    if (sqlite3_prepare_v2(db, insertQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to archive order: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare insert query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* deleteQuery = R"(
        DELETE FROM Commandes WHERE commande_id = ?
    )";

    if (sqlite3_prepare_v2(db, deleteQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Failed to delete order: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return false;
        }

        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare delete query: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    return true;
}

bool DatabaseOrders::deleteOrderItemsByOrderId(int orderId) {
    const char* sqlQuery = "DELETE FROM OrderItems WHERE order_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            std::cout << "Order items for order ID " << orderId << " deleted successfully." << std::endl;
            return true;
        } else {
            std::cerr << "Error deleting order items: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing delete query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}
