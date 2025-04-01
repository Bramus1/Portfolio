#ifndef HANDLEMENU_H
#define HANDLEMENU_H

#include "../../Database/DataBaseRestaurant/DatabaseOrders.h"
#include "../../Database/DataBaseUtility/DatabaseUtility.h"
#include "../../Database/DataBaseUtility/DatabaseNotification.h"

#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <fstream>
#include <iostream>
#include <Poco/JSON/Array.h>
#include <sstream> 
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <string>
#include <Poco/JSON/Object.h>

/**
 * @class HandleMenu
 * @brief Manages menu-related operations and communication with clients.
 *
 * This class provides methods to handle API requests related to the menu, including retrieving,
 * adding, and deleting menu items. It interacts with the `DatabaseMenu` for database operations.
 */
class HandleMenu {
public:
    /**
     * @brief Constructs a HandleMenu object.
     * @param database Reference to the main Database instance.
     *
     * Initializes the `DatabaseMenu` object for menu-specific operations.
     */
    HandleMenu(Database & database);

    /**
     * @brief Handles a request to retrieve the menu.
     * @param socketStream Stream for communicating with the client.
     *
     * This method fetches the menu data from the database and sends it to the client in JSON format.
     */
    void handleGetMenu(Poco::Net::SocketStream& socketStream);

    /**
     * @brief Handles a request to retrieve the menu for a waiter.
     * @param clientSocket Socket for the connected client.
     *
     * This method retrieves a menu formatted specifically for waiter-related operations.
     */
    void handleGetMenu_waiter(Poco::Net::StreamSocket& socketStream);

    /**
     * @brief Handles a request to add a new item to the menu.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the details of the new menu item.
     *
     * This method validates the input data, adds the menu item to the database, and sends the result to the client.
     */
    void handleAddMenuItem(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to delete a menu item.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the details of the menu item to delete.
     *
     * This method processes the deletion request and responds with the operation result.
     */
    void handleDeleteMenuItem(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

private:

    DatabaseMenu dbMenu; ///< Instance of DatabaseMenu for menu-related database operations.
};

#endif // HANDLEMENU_H