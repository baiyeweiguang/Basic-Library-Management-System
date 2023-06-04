# Library Management System
This is a course design for the "Database Principles" class in the Artificial Intelligence major at Central South University. The project consists of a simple library management system (LMS) implemented in C++ using the `gtkmm` library for the graphical user interface and MySQL for the database.


## Dependencies

- [gtkmm](https://www.gtkmm.org/en/) (C++ interface for GTK+)
- [MySQL](https://www.mysql.com/)

## Tested Environment

This project has been tested and confirmed to work on Ubuntu 20.04.

## Setup and Installation

Follow these steps to set up the development environment and install the necessary dependencies:

1. Update the system package list:

```bash
sudo apt-get update
```

2. Install the required dependencies:

```bash
sudo apt-get install libgtkmm-3.0-dev mysql-server libmysqlcppconn-dev
```

3. Clone the project repository:

4. Compile the project:

```bash
mkdir build && cd build
cmake ..
make
```

5. Run the application:

```bash
./library_management_system
```

## Features

The library management system includes the following features:

- Add books to the library
- Search for books by title, author, or category
- Edit book information
- Delete books from the library
- Manage book categories

## License

This project is available under the [MIT License](https://opensource.org/licenses/MIT). See the [LICENSE](./LICENSE) file for more information.

## Acknowledgements

This project was not completed.