#include "DatabaseUserManagement.h"
#include "../DataBase.h"

DatabaseUserManagement::DatabaseUserManagement(Database& databaseInstance) : database(databaseInstance) {}

int DatabaseUserManagement::authenticateUser(const std::string& username, const std::string& password, int role) {
    std::cout << "Authenticating user: " << username << " with role: " << role << std::endl;

    const char* sqlQuery = R"(
        SELECT user_id 
        FROM Users 
        WHERE name = ? AND password = ? AND role_id = ?
    )";
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, role);

        
        std::cout << "Executing query..." << std::endl;

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            std::cout << "Authentication successful, userId: " << userId << std::endl;
            return userId;  
        } else {
            std::cerr << "No matching user found for given username, password, and role" << std::endl;
        }
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    return -1;  
}

std::vector<std::tuple<int, std::string, std::string>> DatabaseUserManagement::getUsers() {
    std::vector<std::tuple<int, std::string, std::string>> users;
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    
    const char* sqlQuery = "SELECT u.user_id, u.name, r.name FROM Users u JOIN Roles r ON u.role_id = r.role_id";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int userId = sqlite3_column_int(stmt, 0);  // user_id
            std::string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));  // username
            std::string roleName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));  // role name
            users.emplace_back(userId, username, roleName);  
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing query: " << sqlite3_errmsg(db) << std::endl;
    }

    return users;
}

bool DatabaseUserManagement::addUser(const std::string& username, const std::string& password, int roleId) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    const char* sqlQuery = "INSERT INTO Users (name, password, role_id) VALUES (?, ?, ?)";

    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 3, roleId);

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

void DatabaseUserManagement::deleteUser(int userId) {
    sqlite3* db = database.getDbHandle();
    sqlite3_stmt* stmt;

    
    const char* sqlQuery = "DELETE FROM Users WHERE user_id = ?";


    if (sqlite3_prepare_v2(db, sqlQuery, -1, &stmt, nullptr) == SQLITE_OK) {
        
        sqlite3_bind_int(stmt, 1, userId);

        
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "Error deleting user: " << sqlite3_errmsg(db) << std::endl;
        } else {
            std::cout << "User with ID " << userId << " deleted successfully." << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing delete query: " << sqlite3_errmsg(db) << std::endl;
    }
}