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

  class SalesColumnRecord : public Gtk::TreeModel::ColumnRecord {
   public:
    SalesColumnRecord() {
      this->ColumnRecord::add(id);
      this->ColumnRecord::add(title);
      this->ColumnRecord::add(quantity);
      this->ColumnRecord::add(unit_price);
      this->ColumnRecord::add(total_price);
      this->ColumnRecord::add(avg_price);
    }
    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> title;
    Gtk::TreeModelColumn<int> quantity;
    Gtk::TreeModelColumn<double> unit_price;
    Gtk::TreeModelColumn<double> total_price;
    Gtk::TreeModelColumn<double> avg_price;
  };

  Book(int id, const std::string &title, int category_id,
       const std::string &isbn);
  Book(const std::string &title, int category_id, const std::string &isbn);
  Book();

  void set_publication_date(const std::tm &publication_date);
  int get_id() const;
  const std::string &get_title() const;
  const std::string &get_isbn() const;
  int get_category_id() const;
  std::tm get_publication_date() const;

 private:
  int id_;
  int category_id_;
  std::string title_;
  std::string isbn_;
  std::tm publication_date_;
};

}  // namespace lms
#endif  // LMS__BOOK_HPP_