#include "lms/database.hpp"
#include <ctime>

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
            "INSERT INTO books (title, author, isbn,"
            "category_id, publication_date) VALUES (?, ?, ?, ?, ?)"));
    prepared_statement->setString(1, book.get_title());
    prepared_statement->setString(2, book.get_author());
    prepared_statement->setString(3, book.get_isbn());
    prepared_statement->setInt(4, std::stoi(book.get_category_id()));
    // Convert std::tm to string
    std::tm publication_date = book.get_publication_date();
    std::string publication_date_str =
        std::to_string(publication_date.tm_year + 1900) + "-" +
        std::to_string(publication_date.tm_mon + 1) + "-" +
        std::to_string(publication_date.tm_mday);
    prepared_statement->setString(5, publication_date_str);
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
            "UPDATE books SET title = ?, author = ?, "
            "isbn = ?, category_id = ? WHERE id = ?"));
    prepared_statement->setString(1, book.get_title());
    prepared_statement->setString(2, book.get_author());
    prepared_statement->setString(3, book.get_isbn());
    prepared_statement->setInt(4, std::stoi(book.get_category_id()));
    prepared_statement->setInt(5, book.get_id());
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
      std::string author = result_set->getString("author");
      std::string isbn = result_set->getString("isbn");
      std::string category_id = result_set->getString("category_id");
      std::tm publication_date;

      books.emplace_back(id, title, category_id, author, isbn);
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
      book = std::move(
          Book(result_set->getInt("id"), result_set->getString("title"),
               result_set->getString("category_id"),
               result_set->getString("author"), result_set->getString("isbn")));
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

}  // namespace lms
