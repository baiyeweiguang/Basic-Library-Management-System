#ifndef LMS__SORTED_BOOKS_WINDOW_HPP_
#define LMS__SORTED_BOOKS_WINDOW_HPP_

// GTKMM
#include <gtkmm.h>

// Project
#include "lms/database.hpp"

namespace lms {

class SortedBooksWindow : public Gtk::Dialog {
 public:
  SortedBooksWindow(Database::SharedPtr database);

 private:
  void refresh_books();
  void on_sort_option_changed();

  Database::SharedPtr database_;
  Gtk::ComboBoxText sort_options_;
  Gtk::ComboBoxText sort_order_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Book::ColumnRecord columns_;
};

}  // namespace lms
#endif  // LMS_SORTED_BOOKS_WINDOW_HPP