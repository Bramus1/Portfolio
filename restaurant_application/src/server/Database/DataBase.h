#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <tuple>
#include <sqlite3.h>

#include "DataBaseRestaurant/DatabaseMenu.h"
#include "DataBaseRestaurant/DatabaseOrders.h"
#include "DataBaseManagement/DatabaseInventory.h"
#include "DataBaseUtility/DatabaseNotification.h"
#include "DataBaseManagement/DatabaseUserManagement.h"
#include "DataBaseManagement/DatabaseRevenue.h"
#include "DataBaseUtility/DatabaseUtility.h"


/**
 * @class Database
 * @brief Provides an interface to interact with an SQLite database for the application.
 *
 * This class encapsulates the logic for connecting to an SQLite database,
 * providing access to its handle, and logging database queries. It also integrates
 * functionality from various database modules such as menu, orders, inventory, notifications,
 * user management, and revenue.
 */
class Database {
public:
    /**
     * @brief Constructs a Database object and initializes a connection to the database.
     * @param dbName Name of the database file.
     */
    Database(const std::string& dbName);

    /**
     * @brief Destroys the Database object and closes the database connection.
     */
    ~Database();

    /**
     * @brief Retrieves the SQLite database handle.
     * @return Pointer to the SQLite database handle.
     */
    sqlite3* getDbHandle();
private:
    sqlite3* db; ///< SQLite database handle
    
    /**
     * @brief Logs the result of a database query.
     * @param query The SQL query being executed.
     * @param success True if the query was executed successfully; false otherwise.
     */
    void logQuery(const std::string& query, bool success);
};

#endif // DATABASE_H