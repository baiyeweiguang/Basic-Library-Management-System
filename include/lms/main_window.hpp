#ifndef LMS__MAIN_WINDOW_HPP_
#define LMS__MAIN_WINDOW_HPP_

// GTKMM
#include <gtkmm.h>

// Project
#include "book.hpp"
#include "database.hpp"

namespace lms {
class MainWindow : public Gtk::Window {
 public:
  explicit MainWindow(Database::SharedPtr database);
   ~MainWindow() = default;

 protected:
  void on_button_add();
  void refresh_books();
  void on_tree_view_row_activated(const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column);

 private:
  Gtk::Window window_;
  Gtk::Box box_;
  Gtk::Button button_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Book::ColumnRecord columns_;
  Database::SharedPtr database_;
};

}  // namespace lms
#endif  // LMS__MAIN_WINDOW_HPP_

