#include "lms/book.hpp"

namespace lms {
Book::Book(int id, const std::string &title, const std::string category_id,
           const std::string &author, const std::string &isbn)
    : id_(id),
      category_id_(category_id),
      title_(title),
      author_(author),
      isbn_(isbn) {
  // Avoid invalid category ids
  int id_int = std::stoi(category_id_);
  if (id_int < 1 || id_int > 6) {
    category_id_ = std::to_string(1);
  }
}

Book::Book(const std::string &title, const std::string category_id,
           const std::string &author, const std::string &isbn)
    : id_(0),
      title_(title),
      category_id_(category_id),
      author_(author),
      isbn_(isbn) {  // Avoid invalid category ids
  int id_int = std::stoi(category_id_);
  if (id_int < 1 || id_int > 6) {
    category_id_ = std::to_string(1);
  }
}

Book::Book() : id_(-1), title_(""), category_id_("1"), author_(""), isbn_("") {}

void Book::set_publication_date(const std::tm &publication_date) {
  publication_date_ = publication_date;
}

int Book::get_id() const {
  return id_;
}

const std::string &Book::get_title() const {
  return title_;
}

const std::string &Book::get_category_id() const {
  return category_id_;
}

const std::string &Book::get_author() const {
  return author_;
}

const std::string &Book::get_isbn() const {
  return isbn_;
}

std::tm Book::get_publication_date() const {
  return publication_date_;
}

}  // namespace lms