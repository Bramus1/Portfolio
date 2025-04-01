#include "DatabaseNotification.h"
#include "../DataBase.h"

DatabaseNotifications::DatabaseNotifications(Database& databaseInstance) : database(databaseInstance) {}

bool DatabaseNotifications::addNotification(int orderId, const std::string& message, int userId) {
    const char* sqlQuery = R"(
        INSERT INTO Notifications (order_id, message, sent_to)
        VALUES (?, ?, ?)
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, orderId);
        sqlite3_bind_text(stmt, 2, message.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, userId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Failed to insert notification: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare notification insert query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}

std::vector<std::tuple<int, int, std::string, std::string>> DatabaseNotifications::getNotificationsForUser(int userId) {
    std::vector<std::tuple<int, int, std::string, std::string>> notifications;
    const char* sqlQuery = R"(
        SELECT notification_id, order_id, message, sent_at
        FROM Notifications
        WHERE sent_to = ?
    )";
    
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, userId);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            notifications.emplace_back(
                sqlite3_column_int(stmt, 0),
                sqlite3_column_int(stmt, 1),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))
            );
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error fetching notifications: " << sqlite3_errmsg(db) << std::endl;
    }

    return notifications;
}

bool DatabaseNotifications::deleteNotification(int notificationId) {
    const char* sqlQuery = "DELETE FROM Notifications WHERE notification_id = ?";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, notificationId);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        } else {
            std::cerr << "Failed to delete notification: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing delete query: " << sqlite3_errmsg(db) << std::endl;
    }

    return false;
}