cmake_minimum_required(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


add_executable(Flood_it_App
    main.cpp
    Model/GameBoard.h
    Model/GameBoard.cpp
    Model/Cell.cpp
    Model/Color.h
    Model/Cell.h
    Model/Observer.h
    Model/Observable.h
    Model/Observable.cpp
    Model/Game.h
    Model/Game.cpp
    View/MainWindow.h
    View/MainWindow.cpp
    View/mainwindow.ui
    View/GameBoardWidget.h
    View/GameBoardWidget.cpp
    Model/RecordManager.h
    Model/RecordManager.cpp
    Controller/GameController.h
    Controller/GameController.cpp






)


find_package (QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets )
find_package ( Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )
target_link_libraries (Flood_it_App PRIVATE
 Qt${QT_VERSION_MAJOR}::Widgets )



install(TARGETS Flood_it_App
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
