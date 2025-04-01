#include "DataBase.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <Poco/Exception.h> 
#include <unordered_map> 
#include <algorithm>     



Database::Database(const std::string& dbName) : db(nullptr){
    std::cout << "Attempting to open database: " << dbName << std::endl;

    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        throw Poco::IOException("Failed to open database: " + dbName);
    }
    std::cout << "Database opened successfully." << std::endl;
}


Database::~Database() {
    sqlite3_close(db);
}

void Database::logQuery(const std::string& query, bool success) {
    std::cout << "Query: " << query << " | Success: " << (success ? "Yes" : "No") << std::endl;
}

sqlite3* Database::getDbHandle(){
    return db;
}