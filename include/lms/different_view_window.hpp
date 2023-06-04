#ifndef LMS__DIFFERENT_VIEW_WINDOW_HPP_
#define LMS__DIFFERENT_VIEW_WINDOW_HPP_

// STD
#include <string>

// GTKMM
#include <gtkmm.h>

// Project
#include "book.hpp"
#include "database.hpp"

namespace lms {
class DifferentViewWindow : public Gtk::Dialog {
 public:
  DifferentViewWindow(Database::SharedPtr database);
  ~DifferentViewWindow() = default;

 protected:
  void on_view_selector_changed();
  void refresh_view(const std::string &view_name);

 private:
  Gtk::ComboBoxText view_selector_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Book::SalesColumnRecord sale_columns_;
  Book::ColumnRecord book_columns_;
  Database::SharedPtr database_;
};

}  // namespace lms
#endif  // LMS_DIFFERENT_VIEW_WINDOW_HPP_