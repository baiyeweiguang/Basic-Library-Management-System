#include "lms/author.hpp"

namespace lms {

Author::Author(int id, int book_id, const std::string &name,
               const std::string &nationality, const std::string &sex)
    : id_(id),
      book_id_(book_id),
      name_(name),
      nationality_(nationality),
      sex_(sex) {}

Author::Author(int book_id, const std::string &name,
               const std::string &nationality, const std::string &sex)
    : name_(name), book_id_(book_id), nationality_(nationality), sex_(sex) {}

Author::Author() {}

int Author::get_id() const {
  return id_;
}

const std::string &Author::get_name() const {
  return name_;
}

const std::string &Author::get_sex() const {
  return sex_;
}

const std::string &Author::get_nationality() const {
  return nationality_;
}

int Author::get_book_id() const {
  return book_id_;
}

}  // namespace lms