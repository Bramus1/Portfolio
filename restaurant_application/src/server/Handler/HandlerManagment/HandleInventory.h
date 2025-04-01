#ifndef HANDLEINVENTORY_H
#define HANDLEINVENTORY_H

#include "../../Database/DataBaseManagement/DatabaseInventory.h"

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
 * @class HandleInventory
 * @brief Manages inventory-related operations and communication with clients.
 *
 * This class provides methods to handle inventory-related API requests, including fetching,
 * adding, deleting, and updating inventory items. It communicates with the `DatabaseInventory`
 * module for database interactions.
 */
class HandleInventory {
public:
    /**
     * @brief Constructs a HandleInventory object.
     * @param database Reference to the main Database instance.
     *
     * Initializes the `DatabaseInventory` object for inventory-specific operations.
     */
    HandleInventory(Database & database);

    /**
     * @brief Handles a request to fetch the current inventory.
     * @param clientSocket Socket for the connected client.
     *
     * This method retrieves inventory data from the database and sends it back to the client in JSON format.
     */
    void handleGetInventory(Poco::Net::StreamSocket& clientSocket);

    /**
     * @brief Handles a request to add new items to the inventory.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the new inventory item details.
     *
     * This method validates the input data, adds the item to the database, and responds with the operation result.
     */
    void handleAddInventory(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to delete items from the inventory.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the item details to be deleted.
     *
     * This method processes the deletion request and returns the operation result to the client.
     */
    void handleDeleteInventory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to update inventory items.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the updated inventory details.
     *
     * This method updates the inventory database and responds to the client with the operation result.
     */
    void handleUpdateInventory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);

    /**
     * @brief Sends an error response to the client.
     * @param stream Socket stream to write the error response.
     * @param error Error message.
     * @param code HTTP status code of the error.
     */
    void sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code);

private:

    DatabaseInventory dbInventory; ///< Instance of DatabaseInventory for inventory-related operations.

};

#endif // HANDLEINVENTORY_H