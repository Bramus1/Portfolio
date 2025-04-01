#include "HandleUser.h"

HandleUser::HandleUser(Database & database) : dbUser(database){}

void HandleUser::handleLogin(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
        std::string username = json->getValue<std::string>("username");
        std::string password = json->getValue<std::string>("password");
        std::string roleStr = json->getValue<std::string>("role");

        int roleId = std::stoi(roleStr);
        int userId = dbUser.authenticateUser(username, password, roleId);
        std::cout << "Authentication result: " << userId << std::endl;

        if (userId >= 0) {
            Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
            response->set("user_id", userId);
            response->set("role_id", roleId);

            std::ostringstream responseBody;
            Poco::JSON::Stringifier::stringify(response, responseBody);

            socketStream << "HTTP/1.1 200 OK\r\n"
                        << "Access-Control-Allow-Origin: *\r\n"
                        << "Content-Type: application/json\r\n"
                        << "Content-Length: " << responseBody.str().size() << "\r\n"
                        << "\r\n"
                        << responseBody.str();
        } else {
            //sendErrorResponse(socketStream, "Invalid username, password, or role", 401);
        }
}

void HandleUser::handleGetUsers(Poco::Net::SocketStream& socketStream) {
    auto users = dbUser.getUsers();

    Poco::JSON::Array::Ptr usersArray = new Poco::JSON::Array();
    for (const auto& user : users) {
        Poco::JSON::Object::Ptr userObject = new Poco::JSON::Object();
        userObject->set("user_id", std::to_string(std::get<0>(user)));
        userObject->set("username", std::get<1>(user));
        userObject->set("role_id", std::get<2>(user));
        usersArray->add(userObject);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("users", usersArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
}

void HandleUser::handleDeleteUser(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
    int userId = json->getValue<int>("user_id");
    dbUser.deleteUser(userId);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("message", "User deleted successfully");

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
}

void HandleUser::handleAddUser(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
    std::string username = json->getValue<std::string>("username");
    std::string password = json->getValue<std::string>("password");
    int roleId = std::stoi(json->getValue<std::string>("role_id"));

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (dbUser.addUser(username, password, roleId)) {
        response->set("message", "User added successfully");

        std::ostringstream responseBody;
        Poco::JSON::Stringifier::stringify(response, responseBody);

        socketStream << "HTTP/1.1 201 Created\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "Content-Length: " << responseBody.str().size() << "\r\n"
                     << "\r\n"
                     << responseBody.str();
    } else {
        response->set("error", "Failed to add user");

        std::ostringstream responseBody;
        Poco::JSON::Stringifier::stringify(response, responseBody);

        socketStream << "HTTP/1.1 500 Internal Server Error\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "Content-Length: " << responseBody.str().size() << "\r\n"
                     << "\r\n"
                     << responseBody.str();
    }
}