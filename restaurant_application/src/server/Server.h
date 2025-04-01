#ifndef SERVER_H
#define SERVER_H

#include "Database/DataBase.h"
#include "Handler/HandlerManagment/HandleUser.h"
#include "Handler/HandlerManagment/HandleInventory.h"
#include "Handler/HandlerRestaurant/HandleOrder.h"
#include "Handler/HandlerRestaurant/HandleMenu.h"

#include "Database/DataBaseUtility/DatabaseNotification.h"

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <string>
#include <thread>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Object.h>


/**
 * @class Server
 * @brief Implements the core server functionality for handling client connections, static file serving, and API requests.
 *
 * The Server class is responsible for managing client connections over sockets, handling API
 * requests (both GET and POST), serving static files, and interacting with the database and other
 * handler modules.
 */
class Server {
public:
    /**
     * @brief Constructs a Server object and initializes the server socket.
     * @param port The port number on which the server will listen for incoming connections.
     * @param dbName The name of the database file to use.
     * @param staticFilesPath Path to the directory containing static files to serve.
     */
    Server(int port, const std::string& dbName, const std::string& staticFilesPath);

    /**
     * @brief Starts the server to listen for incoming connections.
     *
     * This method runs the server loop to accept and process client connections.
     */
    void start();

private:
    Poco::Net::ServerSocket serverSocket; ///< Server socket for accepting connections.
    Database database; ///< Database instance for database interactions.
    std::string staticFilesPath; ///< Path to static files to be served.
    HandleUser handleUser; ///< Handler for user-related operations.
    DatabaseMenu dm; ///< Instance for menu-related database operations
    HandleInventory handleInventory;  ///< Handler for inventory-related operations
    HandleOrder handleorder; ///< Handler for order-related operations.
    HandleMenu handleMenu; ///< Handler for menu-related operations.

    /**
     * @brief Handles a client connection.
     * @param clientSocket Socket for the connected client.
     */
    void handleClient(Poco::Net::StreamSocket& clientSocket);

    /**
     * @brief Serves a static file to the client.
     * @param clientSocket Socket for the connected client.
     * @param filePath Path to the file to serve.
     */
    void serveStaticFile(Poco::Net::StreamSocket& clientSocket, const std::string& filePath);

    /**
     * @brief Processes an API request from the client.
     * @param clientSocket Socket for the connected client.
     * @param request The API request string.
     */
    void handleApiRequest(Poco::Net::StreamSocket& clientSocket, const std::string& request);

    /**
     * @brief Handles a GET API request.
     * @param clientSocket Socket for the connected client.
     * @param requestLine The first line of the GET request.
     */
    void handleGetRequest(Poco::Net::StreamSocket& clientSocket, const std::string& requestLine);

    /**
     * @brief Handles a POST API request.
     * @param clientSocket Socket for the connected client.
     * @param socketStream Socket stream for reading and writing data.
     */
    void handlePostRequest(Poco::Net::StreamSocket& clientSocket, Poco::Net::SocketStream& socketStream);

    /**
     * @brief Sends an error response to the client.
     * @param stream Socket stream to write the error response.
     * @param error Error message.
     * @param code HTTP status code of the error.
     */
    void sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code);

    /**
     * @brief Sends a JSON response to the client.
     * @param clientSocket Socket for the connected client.
     * @param jsonArray JSON array to send in the response.
     */
    void sendJsonResponse(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Array::Ptr jsonArray);

    /**
     * @brief Handles the generation of a report requested by the client.
     * @param clientSocket Socket for the connected client.
     * @param json JSON object containing the report parameters.
     */
    void handleGenerateReport(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json);
};

#endif // SERVER_H
