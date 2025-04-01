#include "HandleInventory.h"

HandleInventory::HandleInventory(Database & database) : dbInventory(database){}

void HandleInventory::handleGetInventory(Poco::Net::StreamSocket& clientSocket) {
    auto inventory = dbInventory.getInventory();

    Poco::JSON::Array::Ptr inventoryArray = new Poco::JSON::Array();
    for (const auto& item : inventory) {
        Poco::JSON::Object::Ptr inventoryItem = new Poco::JSON::Object();
        inventoryItem->set("inventory_id", std::get<0>(item));
        inventoryItem->set("item_name", std::get<1>(item));
        inventoryItem->set("quantity", std::get<2>(item));
        inventoryItem->set("unit", std::get<3>(item));
        inventoryItem->set("min_required", std::get<4>(item));
        inventoryItem->set("last_updated", std::get<5>(item));
        inventoryArray->add(inventoryItem);
    }

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    response->set("inventory", inventoryArray);

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleInventory::handleAddInventory(Poco::Net::SocketStream& socketStream, Poco::JSON::Object::Ptr json) {
    std::string name = json->getValue<std::string>("name");
    int quantity = json->getValue<int>("quantity");
    std::string unit = json->getValue<std::string>("unit");
    int minRequired = json->getValue<int>("min_required");

    bool success = dbInventory.addInventoryItem(name, quantity, unit, minRequired);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        response->set("status", "success");
        response->set("message", "Inventory item added successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to add inventory item.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Access-Control-Allow-Origin: *\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
}

void HandleInventory::handleDeleteInventory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    
    int inventoryId = json->getValue<int>("inventory_id");
    std::cout << "Deleting inventory item with ID: " << inventoryId << std::endl;

    bool success = dbInventory.deleteInventoryItem(inventoryId);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        response->set("status", "success");
        response->set("message", "Inventory item deleted successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to delete inventory item.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    Poco::Net::SocketStream socketStream(clientSocket);
    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleInventory::handleUpdateInventory(Poco::Net::StreamSocket& clientSocket, Poco::JSON::Object::Ptr json) {
    Poco::Net::SocketStream socketStream(clientSocket); 

    if (!json->has("inventory_id") || !json->has("item_name") || !json->has("quantity") ||
        !json->has("unit") || !json->has("min_required")) {
        sendErrorResponse(socketStream, "Missing required fields in request", 400);
        return;
    }

    int inventoryId = json->getValue<int>("inventory_id");
    std::string itemName = json->getValue<std::string>("item_name");
    int quantity = json->getValue<int>("quantity");
    std::string unit = json->getValue<std::string>("unit");
    int minRequired = json->getValue<int>("min_required");

    std::cout << "Updating inventory item ID " << inventoryId << " with new data: "
              << "Name=" << itemName << ", Quantity=" << quantity
              << ", Unit=" << unit << ", MinRequired=" << minRequired << std::endl;

    bool success = dbInventory.updateInventoryItem(inventoryId, itemName, quantity, unit, minRequired);

    Poco::JSON::Object::Ptr response = new Poco::JSON::Object();
    if (success) {
        response->set("status", "success");
        response->set("message", "Inventory item updated successfully.");
    } else {
        response->set("status", "error");
        response->set("message", "Failed to update inventory item.");
    }

    std::ostringstream responseBody;
    Poco::JSON::Stringifier::stringify(response, responseBody);

    socketStream << "HTTP/1.1 200 OK\r\n"
                 << "Content-Type: application/json\r\n"
                 << "Content-Length: " << responseBody.str().size() << "\r\n"
                 << "\r\n"
                 << responseBody.str();
    socketStream.flush();
}

void HandleInventory::sendErrorResponse(Poco::Net::SocketStream& stream, const std::string& error, int code) {
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