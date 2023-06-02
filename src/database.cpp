#include "lms/database.hpp"

namespace lms {
Database::Database(const DatabaseConfig &config) {
  driver_ = get_driver_instance();
  connection_.reset(
      driver_->connect(config.host, config.username, config.password));
  connection_->setSchema(config.schema);
}

int Database::add_book(const Book &book) {
  std::unique_ptr<sql::PreparedStatement> prepared_statement(
      connection_->prepareStatement(
          "INSERT INTO books (title, author, isbn) VALUES (?, ?, ?)"));
  prepared_statement->setString(1, book.get_title());
  prepared_statement->setString(2, book.get_author());
  prepared_statement->setString(3, book.get_isbn());
  int ret = prepared_statement->executeUpdate();
  return ret;
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
            "UPDATE books SET title = ?, author = ?, isbn = ? WHERE id = ?"));
    prepared_statement->setString(1, book.get_title());
    prepared_statement->setString(2, book.get_author());
    prepared_statement->setString(3, book.get_isbn());
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
      std::string author = result_set->getString("author");
      std::string isbn = result_set->getString("isbn");
      books.emplace_back(id, title, author, isbn);
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
               result_set->getString("author"), result_set->getString("isbn")));
    }
  } catch (const sql::SQLException &e) {
    std::cerr << "Error executing get_book_by_id query: " << e.what()
              << std::endl;
  }
  return book;
}

}  // namespace lms
