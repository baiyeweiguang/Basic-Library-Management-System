#ifndef LMS__MAIN_WINDOW_HPP_
#define LMS__MAIN_WINDOW_HPP_

// GTKMM
#include <gtkmm.h>

// Project
#include "lms/database/book.hpp"
#include "lms/database/database.hpp"

namespace lms {
class MainWindow : public Gtk::Window {
 public:
  explicit MainWindow(Database::SharedPtr database);
  ~MainWindow() = default;

 protected:
  void on_button_add();
  void on_sorted_books_button_clicked();
  void refresh_books();
  void on_tree_view_row_activated(const Gtk::TreeModel::Path &path,
                                  Gtk::TreeViewColumn* column);
  void on_change_view_button_clicked();
  void on_query_button_clicked();

 private:
  Gtk::VBox main_box_;
  Gtk::Window window_;
  Gtk::ButtonBox button_box_;
  Gtk::Button query_button_;
  Gtk::Button change_view_button_;
  Gtk::Button sorted_books_button_;
  Gtk::Button add_button_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Book::ColumnRecord columns_;
  Database::SharedPtr database_;
};

}  // namespace lms
#endif  // LMS__MAIN_WINDOW_HPP_
