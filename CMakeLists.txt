cmake_minimum_required(VERSION 3.10)

project(library_management_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(MYSQLCPP REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

find_path(MySQLCppConnector_INCLUDE_DIRS mysql_connection.h)
find_library(MySQLCppConnector_LIBRARIES mysqlcppconn)

set(SRC
        src/main.cpp
        src/add_book_window.cpp
        src/book.cpp
        src/database.cpp
        src/edit_book_window.cpp
        src/main_window.cpp)
add_executable(library_management_system ${SRC})
target_include_directories(library_management_system PUBLIC
        include
        ${GTKMM_INCLUDE_DIRS}
        ${MYSQLCPP_INCLUDE_DIRS}
        ${MySQLCppConnector_INCLUDE_DIRS})
target_link_libraries(library_management_system
        ${GTKMM_LIBRARIES}
        ${MYSQLCPP_LIBRARIES}
        ${MySQLCppConnector_LIBRARIES})
