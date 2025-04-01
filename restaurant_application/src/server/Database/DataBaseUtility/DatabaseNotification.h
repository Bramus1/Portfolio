#ifndef DATABASENOTIFICATIONS_H
#define DATABASENOTIFICATIONS_H

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

class Database;

/**
 * @class DatabaseNotifications
 * @brief Manages notification-related operations in the database.
 *
 * This class provides methods to add, retrieve, and delete notifications.
 * Notifications are associated with specific users and orders.
 */
class DatabaseNotifications {
public:
    /**
     * @brief Constructs a DatabaseNotifications object.
     * @param databaseInstance Reference to the main Database instance.
     *
     * This constructor initializes the notification manager with access to the database.
     */
    DatabaseNotifications(Database& databaseInstance);

    /**
     * @brief Adds a new notification to the database.
     * @param orderId The ID of the associated order.
     * @param message The notification message.
     * @param userId The ID of the user associated with the notification.
     * @return True if the notification was added successfully, false otherwise.
     */
    bool addNotification(int orderId, const std::string& message, int userId);

    /**
     * @brief Retrieves notifications for a specific user.
     * @param userId The ID of the user whose notifications are to be retrieved.
     * @return A vector of tuples containing notification details:
     *         - Notification ID (int)
     *         - Order ID (int)
     *         - Notification message (std::string)
     *         - Timestamp (std::string)
     */
    std::vector<std::tuple<int, int, std::string, std::string>> getNotificationsForUser(int userId);
    
    /**
     * @brief Deletes a notification from the database.
     * @param notificationId The ID of the notification to be deleted.
     * @return True if the notification was deleted successfully, false otherwise.
     */
    bool deleteNotification(int notificationId);

private:
    Database& database; ///< Reference to the main database instance.
};

#endif // DATABASENOTIFICATIONS_H