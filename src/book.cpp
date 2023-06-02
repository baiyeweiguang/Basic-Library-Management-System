#include "lms/book.hpp"

namespace lms {
Book::Book(int id, const std::string &title, const std::string &author,
           const std::string &isbn)
    : id_(id), title_(title), author_(author), isbn_(isbn) {}

Book::Book(const std::string &title, const std::string &author,
           const std::string &isbn)
    : id_(0), title_(title), author_(author), isbn_(isbn) {}

Book::Book() : id_(-1), title_(""), author_(""), isbn_("") {}

int Book::get_id() const {
  return id_;
}

const std::string &Book::get_title() const {
  return title_;
}

const std::string &Book::get_author() const {
  return author_;
}

const std::string &Book::get_isbn() const {
  return isbn_;
}

}  // namespace lms