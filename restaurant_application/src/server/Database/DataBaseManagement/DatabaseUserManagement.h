#ifndef DATABASEUSERMANAGEMENT_H
#define DATABASEUSERMANAGEMENT_H

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
 * @class DatabaseUserManagement
 * @brief Manages user authentication and operations related to users.
 *
 * This class provides functionalities to authenticate users, retrieve a list of users,
 * add new users, and delete users. It interacts with the database to perform these operations.
 */
class DatabaseUserManagement {
public:
    /**
     * @brief Constructs a DatabaseUserManagement object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the user management functionality with access to the database.
     */
    DatabaseUserManagement(Database& databaseInstance);

    /**
     * @brief Authenticates a user by verifying the username, password, and role.
     * @param username The username of the user to authenticate.
     * @param password The password of the user to authenticate.
     * @param role The role of the user (e.g., 1 for admin, 2 for waiter).
     * @return The user ID if authentication is successful, or -1 if the authentication fails.
     */
    int authenticateUser(const std::string& username, const std::string& password, int role);

    /**
     * @brief Retrieves a list of all users.
     * @return A vector of tuples where each tuple contains:
     *         - User ID (int)
     *         - Username (std::string)
     *         - User role (std::string)
     */
    std::vector<std::tuple<int, std::string, std::string>> getUsers();

    /**
     * @brief Adds a new user to the system.
     * @param username The username of the new user.
     * @param password The password of the new user.
     * @param roleId The role ID of the new user (e.g., 1 for admin, 2 for waiter).
     * @return True if the user was added successfully, false otherwise.
     */
    bool addUser(const std::string& username, const std::string& password, int roleId);

     /**
     * @brief Deletes an existing user from the system.
     * @param userId The ID of the user to delete.
     */
    void deleteUser(int userId); 

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEUSERMANAGEMENT_H