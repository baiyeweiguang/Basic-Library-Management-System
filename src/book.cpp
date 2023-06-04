#include "lms/book.hpp"

namespace lms {
Book::Book(int id, const std::string &title, int category_id,
           const std::string &isbn)
    : id_(id),
      category_id_(category_id),
      title_(title),
      isbn_(isbn) {
  // Avoid invalid category ids
  if (category_id_ < 1 || category_id_ > 6) {
    category_id_ = 1;
  }
}

Book::Book(const std::string &title, int category_id,
           const std::string &isbn)
    : id_(0),
      title_(title),
      category_id_(category_id),
      isbn_(isbn) {
  // Avoid invalid category ids
  if (category_id_ < 1 || category_id_ > 6) {
    category_id_ = 1;
  }
}

Book::Book()
    : id_(-1), title_(""), category_id_(1), isbn_("") {}

void Book::set_publication_date(const std::tm &publication_date) {
  publication_date_ = publication_date;
}

int Book::get_id() const {
  return id_;
}

const std::string &Book::get_title() const {
  return title_;
}

int Book::get_category_id() const {
  return category_id_;
}

const std::string &Book::get_isbn() const {
  return isbn_;
}

std::tm Book::get_publication_date() const {
  return publication_date_;
}

}  // namespace lms