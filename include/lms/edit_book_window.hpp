#ifndef LMS__BOOK_WINDOW_HPP_
#define LMS__BOOK_WINDOW_HPP_

// STD
#include <string>

// GTKMM
#include <gtkmm.h>

// Project
#include "book.hpp"
#include "database.hpp"

namespace lms {

class EditBookWindow : public Gtk::Dialog {
 public:
  EditBookWindow(Database::SharedPtr database, const Book &book);
  ~EditBookWindow() = default;

 protected:
  void on_button_save();
  void on_button_delete();

 private:
  Database::SharedPtr database_;
  Book book_;
  Gtk::Box box_;
  Gtk::Label title_label_, author_label_, isbn_label_, category_label_;
  Gtk::Entry title_entry_, author_entry_, isbn_entry_, category_entry_;
  Gtk::Button save_button_, delete_button_;
};

}  // namespace lms
#endif  // LMS__BOOK_WINDOW_HPP_