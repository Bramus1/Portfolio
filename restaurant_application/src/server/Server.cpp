#include "Server.h"


#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Stringifier.h>
#include <sstream> 

Server::Server(int port, const std::string& dbName, const std::string& staticFilesPath)
    : serverSocket(port), database(dbName), staticFilesPath(staticFilesPath), handleUser(database), dm(database), handleInventory(database), handleorder(database), handleMenu(database) {}

void Server::start() {
    std::cout << "Server running on port " << serverSocket.address().toString() << std::endl;

    while (true) {
        auto clientSocket = std::make_shared<Poco::Net::StreamSocket>(serverSocket.acceptConnection());
        std::thread([this, clientSocket]() {
            handleClient(*clientSocket);
        }).detach();
    }
}

void Server::handleClient(Poco::Net::StreamSocket& clientSocket) {
    Poco::Net::SocketStream socketStream(clientSocket);
    std::string requestLine;

    std::getline(socketStream, requestLine);
    std::cout << "Request: " << requestLine << std::endl;

    if (requestLine.find("GET") == 0) {
        handleGetRequest(clientSocket, requestLine);
    } else if (requestLine.find("POST") == 0) {
        handlePostRequest(clientSocket, socketStream);
    } else {
        socketStream << "HTTP/1.1 400 Bad Request\r\n"
                     << "Content-Type: text/plain\r\n"
                     << "\r\n"
                     << "400 Bad Request";
    }
}

void Server::handleGetRequest(Poco::Net::StreamSocket& clientSocket, const std::string& requestLine) {
    std::string requestedFile = requestLine.substr(4, requestLine.find(' ', 4) - 4);

    if (requestedFile == "/") {
        requestedFile = "/index.html";
    }
    serveStaticFile(clientSocket, staticFilesPath + requestedFile);
    
}

void Server::handlePostRequest(Poco::Net::StreamSocket& clientSocket, Poco::Net::SocketStream& socketStream) {
    size_t contentLength = 0;
    std::string line;

    while (std::getline(socketStream, line) && line != "\r") {
        if (line.find("Content-Length:") != std::string::npos) {
            contentLength = std::stoi(line.substr(16));
        }
    }

    std::string body(contentLength, '\0');
    socketStream.read(&body[0], contentLength);

    handleApiRequest(clientSocket, body);
}


void Server::sendJsonResponse(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Array::Ptr jsonArray) {
    std::ostringstream response;
    Poco::JSON::Stringifier::stringify(jsonArray, response);

    std::cout << "Sending JSON response: " << response.str() << std::endl;

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << response.str().size() << "\r\n"
                 << "\r\n"
                 << response.str();
    socketStream.flush();
}


void Server::serveStaticFile(Poco::Net::StreamSocket& clientSocket, const std::string& requestedFile) {


    
    std::string filePath =  requestedFile;
    std::replace(filePath.begin(), filePath.end(), '\\', '/'); // Normalize slashes
    std::cout << "Attempting to serve file: " << filePath << std::endl;

    std::ifstream file(filePath, std::ios::binary);
    Poco::Net::SocketStream socketStream(clientSocket);

    if (file) {
        std::ostringstream content;
        content << file.rdbuf();

        std::string contentType = "text/html";
        if (requestedFile.size() >= 4 && requestedFile.substr(requestedFile.size() - 4) == ".css") {
            contentType = "text/css";
        } else if (requestedFile.size() >= 3 && requestedFile.substr(requestedFile.size() - 3) == ".js") {
            contentType = "application/javascript";
        }

        socketStream << "HTTP/1.1 200 OK\r\n"
                     << "Content-Type: " << contentType << "\r\n"
                     << "Content-Length: " << content.str().size() << "\r\n"
                     << "\r\n"
                     << content.str();
    } else {
        std::cerr << "File not found: " << filePath << std::endl;
        socketStream << "HTTP/1.1 404 Not Found\r\n"
                     << "Content-Type: text/plain\r\n"
                     << "\r\n"
                     << "404 Not Found";
    }
    socketStream.flush();
}


void Server::sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code) {
    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("error", error);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    stream << "HTTP/1.1 " << code << " Error\r\n"
           << "Access-Control-Allow-Origin: *\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush();
}


void Server::handleApiRequest(Poco::Net::StreamSocket& clientSocket, const std::string& request) {
    Poco::Net::SocketStream socketStream(clientSocket);
    Poco::JSON::Parser parser;

    try {
        std::cout << "Incoming request: " << request << std::endl;

        Poco::Dynamic::Var parsed = parser.parse(request);
        auto json = parsed.extract<Poco::JSON::Object::Ptr>();
        std::string command = json->getValue<std::string>("command");

        if (command == "LOGIN") {
            handleUser.handleLogin(socketStream, json);
        } else if (command == "GET_MENU_WAITER") {
            handleMenu.handleGetMenu_waiter(clientSocket);
        }else if (command == "GET_MENU") {
            handleMenu.handleGetMenu(socketStream);
        } else if (command == "PLACE_ORDER") {
            handleorder.handlePlaceOrder(clientSocket, json);
        }else if (command == "UPDATE_ORDER_STATUS") {
            handleorder.handleUpdateOrderStatus(clientSocket, json);
        }else if (command == "GET_ORDERS_BAR") {
            handleorder.handleGetOrders_bar(clientSocket);
        } else if (command == "UPDATE_ORDER_STATUS_WAITER") {
            handleorder.handleUpdateOrderStatus_waiter(clientSocket, json);
        }else if (command == "GET_ORDERS") {
            handleorder.handleGetOrders(clientSocket);
        } else if (command == "GET_USERS") {
            handleUser.handleGetUsers(socketStream);
        } else if (command == "DELETE_USER") {
            handleUser.handleDeleteUser(socketStream, json);
        } else if (command == "ADD_USER") {
            handleUser.handleAddUser(socketStream, json);
        } else if (command == "ADD_MENU_ITEM") {
            handleMenu.handleAddMenuItem(socketStream, json);
        } else if (command == "DELETE_MENU_ITEM") {
            handleMenu.handleDeleteMenuItem(socketStream, json);
        }else if(command == "GET_INVENTORY"){
            std::cout << "Command GET_INVENTORY received" << std::endl;
            handleInventory.handleGetInventory(clientSocket);
        }else if(command == "ADD_INVENTORY"){
            handleInventory.handleAddInventory(socketStream, json);
        }else if(command == "DELETE_INVENTORY"){
            handleInventory.handleDeleteInventory(clientSocket, json);
        }else if(command == "UPDATE_INVENTORY"){
            handleInventory.handleUpdateInventory(clientSocket, json);
        }else if (command == "GET_ORDER_HISTORY") {
            handleorder.handleGetOrderHistory(clientSocket,json);
        }else if (command == "GENERATE_REPORT") {
            handleGenerateReport(clientSocket,json);
        }else if (command == "GET_ORDERS_KITCHEN") {
            handleorder.handleGetOrders_kitchen(clientSocket);
        }else if (command == "GET_NOTIFICATIONS") {
            handleorder.handleGetNotifications(clientSocket, json); 
        }else if (command == "DELETE_NOTIFICATION") {
            handleorder.handleDeleteNotification(clientSocket, json);
        }else if(command == "UPDATE_ORDER_STATUS_BAR"){
            handleorder.UpdateOrderStatusBar(clientSocket, json);
        }if (command == "ARCHIVE_ORDER") {
            handleorder.handleArchiveOrder(clientSocket, json);
        }else {
            sendErrorResponse(socketStream, "Invalid command", 400);
        }
    } catch (const Poco::Exception& ex) {
        sendErrorResponse(socketStream, "Server error: " + std::string(ex.what()), 500);
    }
}



void Server::handleGenerateReport(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {

    std::string startDate = json->getValue<std::string>("start_date");
    std::string endDate = json->getValue<std::string>("end_date");

    // Calculate revenue
    DatabaseRevenue dbr(database);
    int totalRevenue = dbr.calculateRevenue(startDate, endDate);

    // Create the response object
    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("total_revenue", totalRevenue);

    // Send response using socketStream directly
    Poco::Net::SocketStream socketStream(clientSocket);
    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}




int main() {
    try {
        Server server(9090, "restaurant.db", "../client");
        server.start();
    } catch (const std::exception& ex) {
        std::cerr << "Server failed to start: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}