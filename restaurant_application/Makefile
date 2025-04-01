
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lPocoNet -lPocoJSON -lPocoFoundation -lsqlite3


SRC_DIR = src/server
SRC_DIR_DB_MNG =src/server/Database/DataBaseManagement
SRC_DIR_DB_Utility =src/server/Database/DataBaseUtility
SRC_DIR_DB_Resto =src/server/Database/DataBaseRestaurant
SRC_DIR_HAND_MNG=src/server/Handler/HandlerManagment
SRC_DIR_HAND_RESTO=src/server/Handler/HandlerRestaurant
SRC_DIR_DB=src/server/Database

SRC_FILES = $(SRC_DIR)/Server.cpp $(SRC_DIR_DB)/DataBase.cpp $(SRC_DIR_DB_MNG)/DatabaseInventory.cpp $(SRC_DIR_DB_Resto)/DatabaseMenu.cpp $(SRC_DIR_DB_Utility)/DatabaseNotification.cpp $(SRC_DIR_DB_Resto)/DatabaseOrders.cpp $(SRC_DIR_DB_MNG)/DatabaseRevenue.cpp $(SRC_DIR_DB_MNG)/DatabaseUserManagement.cpp $(SRC_DIR_DB_Utility)/DatabaseUtility.cpp $(SRC_DIR_HAND_MNG)/HandleUser.cpp $(SRC_DIR_HAND_MNG)/HandleInventory.cpp $(SRC_DIR_HAND_RESTO)/HandleOrder.cpp $(SRC_DIR_HAND_RESTO)/HandleMenu.cpp


TARGET = $(SRC_DIR)/server


all: $(TARGET)

$(TARGET): $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

doc:
	doxygen Doxyfile
	
run: all
	@echo "change directory $(SRC_DIR) and run $(TARGET)..."
	@cd $(SRC_DIR) && ./server

requirements:
	@echo "installing libraries"
	@xargs -a requirements.txt sudo apt install -y

.PHONY: all clean requirements
