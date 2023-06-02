#ifndef LMS__DATABASE_HPP_
#define LMS__DATABASE_HPP_

// STD
#include <memory>
#include <string>
#include <vector>

// MySQL Connector/C++
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

// Project
#include "book.hpp"

namespace lms {
struct DatabaseConfig {
  std::string host;
  std::string username;
  std::string password;
  std::string schema;
};

class Database {
 public:
  using SharedPtr = std::shared_ptr<Database>;
  Database(const DatabaseConfig &config);
  ~Database() = default;

  int add_book(const Book &book);
  int delete_book(int id);
  int update_book(const Book &book);
  Book get_book_by_id(int id);
  std::string get_category_name_by_id(int id);
  std::vector<std::pair<std::string, int>> get_book_count_by_category();
  std::vector<Book> get_books();

 private:
  sql::Driver* driver_;
  std::unique_ptr<sql::Connection> connection_;
};

}  // namespace lms
#endif  // LMS__DATABASE_HPP_