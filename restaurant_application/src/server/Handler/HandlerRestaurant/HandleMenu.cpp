#include "HandleMenu.h"

HandleMenu::HandleMenu(Database & database) : dbMenu(database){}

void HandleMenu::handleGetMenu(Poco::Net::SocketStream& socketStream) {
    auto menu = dbMenu.getMenu();

    Poco::JSON::Array::Ptr menuArray = new Poco::JSON::Array();
    for (const auto& item : menu) {
        Poco::JSON::Object::Ptr menuItem = new Poco::JSON::Object();
        menuItem->set("id", std::get<0>(item));
        menuItem->set("name", std::get<1>(item));
        menuItem->set("price", std::get<2>(item));
        menuItem->set("type", std::string(1, std::get<3>(item))); // Convert char to string
        menuArray->add(menuItem);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("menu", menuArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
}

void HandleMenu::handleGetMenu_waiter(Poco::Net::StreamSocket& socketStream) {
    Poco::Net::SocketStream stream(socketStream); 

    auto menuItems = dbMenu.getMenu_waiter(); 

    Poco::JSON::Array::Ptr menuArray = new Poco::JSON::Array();
    for (const auto& item : menuItems) {
        Poco::JSON::Object::Ptr menuItem = new Poco::JSON::Object();
        menuItem->set("id", std::get<0>(item));        // menu_item_id
        menuItem->set("name", std::get<1>(item));      // name
        menuItem->set("price", std::get<2>(item));     // price
        menuArray->add(menuItem);
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(menuArray, responseBody);

    stream << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: application/json\r\n"
           << "Content-Length: " << responseBody.str().size() << "\r\n"
           << "\r\n"
           << responseBody.str();
    stream.flush(); 
}

void HandleMenu::handleAddMenuItem(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
    if (json->has("name") && json->has("price") && json->has("type")) {
        std::string name = json->getValue<std::string>("name");
        double price = json->getValue<double>("price");
        std::string type = json->getValue<std::string>("type");

        bool success = dbMenu.addMenuItem(name, price, type[0]);

        Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
        if (success) {
            response->set("status", "success");
            response->set("message", "Menu item added successfully.");
        } else {
            response->set("status", "error");
            response->set("message", "Failed to add menu item.");
        }

        std::ostringstream responseBody;
        Poco::JSON::Stringifier::stringify(response, responseBody);

        socketStream << "HTTP/1.1 200 OK\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "Content-Length: " << responseBody.str().size() << "\r\n"
                     << "\r\n"
                     << responseBody.str();
    } else {
        socketStream << "HTTP/1.1 400 Bad Request\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "\r\n"
                     << R"({"status": "error", "message": "Missing 'name', 'price', or 'type' in request."})";
    }
}

void HandleMenu::handleDeleteMenuItem(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
    if (json->has("item_id")) {
        int itemId = std::stoi(json->getValue<std::string>("item_id"));

        bool success = dbMenu.deleteMenuItem(itemId);

        Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
        if (success) {
            response->set("status", "success");
            response->set("message", "Menu item deleted successfully.");
        } else {
            response->set("status", "error");
            response->set("message", "Failed to delete menu item.");
        }

        std::ostringstream responseBody;
        Poco::JSON::Stringifier::stringify(response, responseBody);

        socketStream << "HTTP/1.1 200 OK\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "Content-Length: " << responseBody.str().size() << "\r\n"
                     << "\r\n"
                     << responseBody.str();
    } else {
        socketStream << "HTTP/1.1 400 Bad Request\r\n"
                     << "Access-Control-Allow-Origin: *\r\n"
                     << "Content-Type: application/json\r\n"
                     << "\r\n"
                     << R"({"status": "error", "message": "Missing 'item_id' in request."})";
    }
}