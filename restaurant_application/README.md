<h1 align="center">Restaurant Management Application</h1>

#  Introduction
 We have decided to develop a restaurant management application. The application aims to facilitate order management, communication between servers, the kitchen, and the bar, in addition to managing users, the menu, inventory, and stock management.


## Project Objectives
The main objective of this project is to create a client-server application for a restaurant that includes the following features:
- **Order Management**: Servers can create orders for each table.
- **Kitchen and Bar Interface**: Orders are sent to the server, which then forwards them to the kitchen or bar for preparation.
- **Notifications**: Servers receive notifications when dishes or drinks are ready.
- **User Role Management**: The application supports multiple roles such as server, cook, bartender, and administrator.
- **Stock Management**: Ingredients used for each dish are tracked in the inventory.


## Modules
Backend (Server Side)
 - **Handles API requests**
 - **Manages communication between clients (servers, kitchen, bar) and the database.**

Frontend (Client Side for Servers)
 - **Manages server orders through the interface.**
 - **Connects to the server for placing orders and updates.**

Frontend (Client Side for Kitchen and Bar)
 - **Displays and processes orders.**
 - **Updates order status and notifies servers.**

Database
  - **Stores data for users, roles, orders, menu items, and inventory.**
 - **Ensures data consistency and security.**

Admin Panel
- **Allows administrators to manage users, the menu, and inventory.**
- **Provides data for tracking stock levels and order history.**



## Technologies
- **Database**: SQLite for managing user, order, and stock information.
- **User Interface**: HTML, JAVASCRIPT, CSS.
- **Communication Protocol**: POCO, SOCKETS.


## Modules AND Project Division
- **Backend**: Server-side logic, API (Alexandre (58508)).
- **Client for Waiters**: Interface for taking orders (Giuliano (59333)).
- **Client for Kitchen and Bar**: Interface for kitchen and bar staff (Alec (58508)).
- **Database**: MySQL database with tables for users, orders, inventory (Lukasz (58434)).
- **Admin Panel**: Management of users, menu, and stock levels (Danila (58000)).


## Installation
1. Clone the repo: `git clone https://gitlab.com/58000/restaurant_application.git`
2. Run Makefile: `make requirements` for requirements for the project
3. Run make for creating the project: `make`
4. Run make for running server: `make run`
5. Run make for cleaning project: `make clean`
6. Run make for generation Doxyfile HTML: `make doc`
7. Visit localhost to test application: `http://localhost:9090/`
8. To enter as waiter: login: `Danila`, password: `123piano!`
9. To enter as admin: login: `admin`, password: `12345`
10. To enter as kitchen: login: `kitchen`, password: `12345`
11. To enter as bar: login: `bar`, password: `12345`
