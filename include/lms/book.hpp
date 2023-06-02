#ifndef LMS__BOOK_HPP_
#define LMS__BOOK_HPP_

// STD
#include <ctime>
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
      this->ColumnRecord::add(category);
      this->ColumnRecord::add(title);
      this->ColumnRecord::add(author);
      this->ColumnRecord::add(isbn);
      this->ColumnRecord::add(publication_date);
    }
    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> category;
    Gtk::TreeModelColumn<Glib::ustring> title;
    Gtk::TreeModelColumn<Glib::ustring> author;
    Gtk::TreeModelColumn<Glib::ustring> isbn;
    Gtk::TreeModelColumn<Glib::ustring> publication_date;
  };

  Book(int id, const std::string &title, const std::string category_id,
       const std::string &author, const std::string &isbn);
  Book(const std::string &title, const std::string category_id,
       const std::string &author, const std::string &isbn);
  Book();

  void set_publication_date(const std::tm &publication_date);
  int get_id() const;
  const std::string &get_title() const;
  const std::string &get_author() const;
  const std::string &get_isbn() const;
  const std::string &get_category_id() const;
  std::tm get_publication_date() const;

 private:
  int id_;
  std::string category_id_;
  std::string title_;
  std::string author_;
  std::string isbn_;
  std::tm publication_date_;
};

}  // namespace lms
#endif  // LMS__BOOK_HPP_