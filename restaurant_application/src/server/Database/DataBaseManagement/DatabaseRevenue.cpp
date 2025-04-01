#include "DatabaseRevenue.h"
#include "../DataBase.h"

DatabaseRevenue::DatabaseRevenue(Database& databaseInstance) : database(databaseInstance) {}

std::vector<std::tuple<int, int, int, int, std::string, int, std::string>> DatabaseRevenue::getOrderHistory(const std::string& startDate, const std::string& endDate) {
    std::vector<std::tuple<int, int, int, int, std::string, int, std::string>> history;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* sqlQuery = R"(
        SELECT history_id, commande_id, user_id, numeroTable, commande_status, sub_total, created_at 
        FROM OrderHistory
        WHERE created_at BETWEEN ? AND ?
    )";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, startDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, endDate.c_str(), -1, SQLITE_STATIC);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            history.emplace_back(
                sqlite3_column_int(stmt, 0),
                sqlite3_column_int(stmt, 1),
                sqlite3_column_int(stmt, 2),
                sqlite3_column_int(stmt, 3),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)),
                sqlite3_column_int(stmt, 5),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))
            );
        }
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return history;
}

int DatabaseRevenue::calculateRevenue(const std::string& startDate, const std::string& endDate) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    int totalRevenue = 0;

    const char* sqlQuery = R"(
        SELECT SUM(sub_total) 
        FROM OrderHistory
        WHERE created_at BETWEEN ? AND ?
    )";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, startDate.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, endDate.c_str(), -1, SQLITE_STATIC);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            totalRevenue = sqlite3_column_int(stmt, 0);
        }
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return totalRevenue;
}