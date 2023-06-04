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
#include "author.hpp"
#include "book.hpp"
#include "sale.hpp"

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
  int add_author(const Author &author);
  int add_sale(const Sale &sale);
  int delete_book(int id);
  int update_book(const Book &book);
  int get_last_insert_book_id();
  int get_last_insert_author_id();
  double get_total_sales_by_id(int id);
  double get_unit_price_by_id(int id);
  int get_quantity_by_id(int id);
  double get_avg_price_from_sales();
  int get_sale_id_by_book_id(int id);
  Book get_book_by_id(int id);
  std::string get_category_name_by_id(int id);
  std::string get_author_name_by_book_id(int id);
  std::vector<std::pair<std::string, int>> get_book_count_by_category();
  std::vector<Book> get_books();
  std::vector<Author> get_authors();
  std::vector<Sale> get_sales();

 private:
  sql::Driver* driver_;
  std::unique_ptr<sql::Connection> connection_;
};

}  // namespace lms
#endif  // LMS__DATABASE_HPP_