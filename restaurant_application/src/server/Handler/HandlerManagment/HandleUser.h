#ifndef HANDLEUSER_H
#define HANDLEUSER_H

#include "../../Database/DataBaseManagement/DatabaseUserManagement.h"

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
 * @class HandleUser
 * @brief Manages user-related operations and communication with clients.
 *
 * This class provides methods to handle user-related API requests, including user login,
 * retrieval, addition, and deletion. It communicates with the `DatabaseUserManagement`
 * module for database interactions.
 */
class HandleUser {
public:
    /**
     * @brief Constructs a HandleUser object.
     * @param database Reference to the main Database instance.
     *
     * Initializes the `DatabaseUserManagement` object for user-specific operations.
     */
    HandleUser(Database & database);
    
    /**
     * @brief Handles a user login request.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing login credentials.
     *
     * This method validates the user's login information and responds with the login result.
     */
    void handleLogin(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to retrieve all users.
     * @param socketStream Stream for communicating with the client.
     *
     * This method fetches user data from the database and sends it back to the client in JSON format.
     */
    void handleGetUsers(Poco::Net::SocketStream& socketStream);

    /**
     * @brief Handles a request to delete a user.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the details of the user to be deleted.
     *
     * This method processes the deletion request and responds with the operation result.
     */
    void handleDeleteUser(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

    /**
     * @brief Handles a request to add a new user.
     * @param socketStream Stream for communicating with the client.
     * @param json JSON object containing the details of the new user.
     *
     * This method validates the input data, adds the user to the database, and responds with the operation result.
     */
    void handleAddUser(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json);

private:

    DatabaseUserManagement dbUser; ///< Instance of DatabaseUserManagement for user-related operations.
};

#endif // HANDLEUSER_H