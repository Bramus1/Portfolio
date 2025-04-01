#ifndef DATABASEREVENUE_H
#define DATABASEREVENUE_H

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
 * @class DatabaseRevenue
 * @brief Provides functionalities to calculate and retrieve revenue-related data.
 *
 * This class allows retrieval of order history and calculation of the total revenue
 * within a specified date range. It interacts with the database to fetch the necessary data.
 */
class DatabaseRevenue {
public:
    /**
     * @brief Constructs a DatabaseRevenue object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the revenue management functionality with access to the database.
     */
    DatabaseRevenue(Database& databaseInstance);
    
    /**
     * @brief Retrieves the order history within a specified date range.
     * @param startDate The start date for the range in the format "YYYY-MM-DD".
     * @param endDate The end date for the range in the format "YYYY-MM-DD".
     * @return A vector of tuples where each tuple contains:
     *         - Order ID (int)
     *         - User ID (int)
     *         - Table number (int)
     *         - Total price (int)
     *         - Order date (std::string)
     *         - Item count (int)
     *         - Data when order was marked as served (std::string)
     */
    std::vector<std::tuple<int, int, int, int, std::string, int, std::string>> getOrderHistory(const std::string& startDate, const std::string& endDate);

    /**
     * @brief Calculates the total revenue within a specified date range.
     * @param startDate The start date for the range in the format "YYYY-MM-DD".
     * @param endDate The end date for the range in the format "YYYY-MM-DD".
     * @return The total revenue (int) for the specified period.
     */
    int calculateRevenue(const std::string& startDate, const std::string& endDate);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASEREVENUE_H