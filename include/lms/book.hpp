#ifndef LMS__BOOK_HPP_
#define LMS__BOOK_HPP_

// STD
#include <string>

// GTKMM
#include <gtkmm/treemodel.h>

namespace lms {
class Book {
 public:
  class ColumnRecord : public Gtk::TreeModel::ColumnRecord {
   public:
    ColumnRecord() {
      this->ColumnRecord::add(id);
      this->ColumnRecord::add(title);
      this->ColumnRecord::add(author);
      this->ColumnRecord::add(isbn);
    }

    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> title;
    Gtk::TreeModelColumn<Glib::ustring> author;
    Gtk::TreeModelColumn<Glib::ustring> isbn;
  };

  Book(int id, const std::string &title, const std::string &author,
       const std::string &isbn);
  Book(const std::string &title, const std::string &author,
       const std::string &isbn);
  Book();

  int get_id() const;
  const std::string &get_title() const;
  const std::string &get_author() const;
  const std::string &get_isbn() const;

 private:
  int id_;
  std::string title_;
  std::string author_;
  std::string isbn_;
};

}  // namespace lms
#endif  // LMS_BOOK_HPP_