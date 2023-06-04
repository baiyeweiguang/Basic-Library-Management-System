#include "lms/database.hpp"
#include <iomanip>
#include <sstream>

namespace lms {
Database::Database(const DatabaseConfig &config) {
  driver_ = get_driver_instance();
  connection_.reset(
      driver_->connect(config.host, config.username, config.password));
  connection_->setSchema(config.schema);
}

int Database::add_book(const Book &book) {
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "INSERT INTO books (title, isbn,"
            "category_id, publication_date) VALUES (?, ?, ?, ?)"));
    prepared_statement->setString(1, book.get_title());
    prepared_statement->setString(2, book.get_isbn());
    prepared_statement->setInt(3, book.get_category_id());
    // Convert std::tm to string
    std::tm publication_date = book.get_publication_date();
    std::string publication_date_str =
        std::to_string(publication_date.tm_year + 1900) + "-" +
        std::to_string(publication_date.tm_mon + 1) + "-" +
        std::to_string(publication_date.tm_mday);
    prepared_statement->setString(4, publication_date_str);
    int ret = prepared_statement->executeUpdate();
    return ret;
  } catch (sql::SQLException &e) {
    std::cout << "Error executing add_book query: " << e.what() << std::endl;
    return -1;
  }
}

int Database::delete_book(int book_id) {
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement("DELETE FROM books WHERE id = ?"));
    prepared_statement->setInt(1, book_id);
    int ret = prepared_statement->executeUpdate();
    return ret;
  } catch (sql::SQLException &e) {
    std::cout << "Error executing delete_book query: " << e.what() << std::endl;
    return -1;
  }
}

int Database::update_book(const Book &book) {
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "UPDATE books SET title = ?, "
            "isbn = ?, category_id = ? WHERE id = ?"));
    prepared_statement->setString(1, book.get_title());
    prepared_statement->setString(2, book.get_isbn());
    prepared_statement->setInt(3, book.get_category_id());
    prepared_statement->setInt(4, book.get_id());
    int ret = prepared_statement->executeUpdate();
    return ret;
  } catch (sql::SQLException &e) {
    std::cout << "Error executing update_book query: " << e.what() << std::endl;
    return -1;
  }
}

std::vector<Book> Database::get_books() {
  std::vector<Book> books;
  try {
    std::unique_ptr<sql::Statement> statement(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> result_set(
        statement->executeQuery("SELECT * FROM books"));

    while (result_set->next()) {
      int id = result_set->getInt("id");
      std::string title = result_set->getString("title");
      std::string isbn = result_set->getString("isbn");
      int category_id = result_set->getInt("category_id");
      // Convert string to std::tm
      std::string publication_date_str =
          result_set->getString("publication_date");
      std::tm publication_date = {};
      std::istringstream iss(publication_date_str);
      iss >> std::get_time(&publication_date, "%Y-%m-%d");

      Book book(id, title, category_id, isbn);
      book.set_publication_date(publication_date);
      books.emplace_back(book);
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_books query: " << e.what() << std::endl;
  }
  return books;
}

Book Database::get_book_by_id(int book_id) {
  Book book;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement("SELECT * FROM books WHERE id = ?"));
    prepared_statement->setInt(1, book_id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      book = std::move(Book(
          result_set->getInt("id"), result_set->getString("title"),
          result_set->getInt("category_id"), result_set->getString("isbn")));
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_book_by_id query: " << e.what()
              << std::endl;
  }
  return book;
}

std::vector<std::pair<std::string, int>>
Database::get_book_count_by_category() {
  std::vector<std::pair<std::string, int>> book_count_by_category;

  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT c.name, COUNT(b.id) as book_count "
                           "FROM categories c "
                           "LEFT JOIN books b ON c.id = b.category_id "
                           "GROUP BY c.id, c.name "
                           "ORDER BY book_count DESC"));

    while (res->next()) {
      std::string category_name = res->getString("name");
      int book_count = res->getInt("book_count");
      book_count_by_category.emplace_back(category_name, book_count);
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_book_count_by_category query: "
              << e.what() << std::endl;
  }

  return book_count_by_category;
}

std::string Database::get_category_name_by_id(int id) {
  std::string category_name;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement("SELECT name FROM categories WHERE id = "
                                      "?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      category_name = result_set->getString("name");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_category_name_by_id query: " << e.what()
              << std::endl;
  }
  return category_name;
}

std::string Database::get_author_name_by_book_id(int id) {
  std::string author_name = "NULL";
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "SELECT name FROM book_authors WHERE book_id = ?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      author_name = result_set->getString("name");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_author_name_by_id query: " << e.what()
              << std::endl;
  }
  return author_name;
}

int Database::get_sale_id_by_book_id(int id) {
  int sale_id = -1;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "SELECT id FROM sales WHERE book_id = ?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      sale_id = result_set->getInt("id");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_sale_id_by_book_id query: " << e.what()
              << std::endl;
  }
  return sale_id;
}

double Database::get_unit_price_by_id(int id) {
  double unit_price = 0;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "SELECT unit_price FROM sales WHERE id = ?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      unit_price = result_set->getDouble("unit_price");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_unit_price_by_id query: " << e.what()
              << std::endl;
  }
  return unit_price;
}

double Database::get_total_sales_by_id(int id) {
  double total_sales = 0;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement("SELECT quantity*unit_price AS "
                                      "total_sales FROM sales WHERE id = ?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      total_sales = result_set->getDouble("total_sales");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_total_sales_by_id query: " << e.what()
              << std::endl;
  }
  return total_sales;
}

double Database::get_avg_price_from_sales() {
  double avg_price = 0;
  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT AVG(unit_price) AS avg_price FROM sales"));

    if (res->next()) {
      avg_price = res->getDouble("avg_price");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_avg_price_from_sales query: " << e.what()
              << std::endl;
  }
  return avg_price;
}

int Database::get_quantity_by_id(int id) {
  int quantity = 0;
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement("SELECT quantity FROM sales WHERE id = "
                                      "?"));
    prepared_statement->setInt(1, id);

    std::unique_ptr<sql::ResultSet> result_set(
        prepared_statement->executeQuery());
    if (result_set->next()) {
      quantity = result_set->getInt("quantity");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_quantity_by_id query: " << e.what()
              << std::endl;
  }
  return quantity;
}

int Database::get_last_insert_book_id() {
  int book_id = -1;
  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT id FROM books ORDER BY id DESC LIMIT 1;"));

    if (res->next()) {
      book_id = res->getInt("id");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_last_insert_book_id query: " << e.what()
              << std::endl;
  }
  return book_id;
}

std::vector<Sale> Database::get_sales() {
  std::vector<Sale> sales;
  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT * from sales"));

    while (res->next()) {
      sales.emplace_back(res->getInt("id"), res->getInt("book_id"),
                         res->getInt("quantity"), res->getDouble("unit_price"));
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_sales query: " << e.what() << std::endl;
  }
  return sales;
}

std::vector<Author> Database::get_authors() {
  std::vector<Author> authors;
  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(
        stmt->executeQuery("SELECT * from book_authors"));

    while (res->next()) {
      authors.emplace_back(res->getInt("id"), res->getString("name"),
                           res->getString("nationality"),
                           res->getString("sex"));
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_authors query: " << e.what() << std::endl;
  }
  return authors;
}

int Database::add_author(const Author &author) {
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "INSERT INTO book_authors (book_id, name, nationality, sex) VALUES "
            "(?, ?, ?, ?)"));
    prepared_statement->setInt(1, author.get_book_id());
    prepared_statement->setString(2, author.get_name());
    prepared_statement->setString(3, author.get_nationality());
    prepared_statement->setString(4, author.get_sex());
    int ret = prepared_statement->executeUpdate();
    return ret;
  } catch (sql::SQLException &e) {
    std::cout << "Error executing add_author query: " << e.what() << std::endl;
    return -1;
  }
}

int Database::add_sale(const Sale &sale) {
  try {
    std::unique_ptr<sql::PreparedStatement> prepared_statement(
        connection_->prepareStatement(
            "INSERT INTO sales (book_id, quantity, unit_price) VALUES "
            "(?, ?, ?"));
    prepared_statement->setInt(1, sale.get_book_id());
    prepared_statement->setInt(2, sale.get_quantity());
    prepared_statement->setDouble(3, sale.get_unit_price());
    int ret = prepared_statement->executeUpdate();
    return ret;
  } catch (sql::SQLException &e) {
    std::cout << "Error executing add_sales query: " << e.what() << std::endl;
    return -1;
  }
}

int Database::get_last_insert_author_id() {
  int id = -1;
  try {
    std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
    std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(
        "SELECT id FROM book_authors ORDER BY id DESC LIMIT 1;"));

    if (res->next()) {
      id = res->getInt("id");
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_last_insert_book_id query: " << e.what()
              << std::endl;
  }
  return id;
}

}  // namespace lms
