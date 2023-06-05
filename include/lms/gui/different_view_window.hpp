#ifndef LMS__DIFFERENT_VIEW_WINDOW_HPP_
#define LMS__DIFFERENT_VIEW_WINDOW_HPP_

// STD
#include <string>

// GTKMM
#include <gtkmm.h>

// Project
#include "lms/database/book.hpp"
#include "lms/database/database.hpp"

namespace lms {
class DifferentViewWindow : public Gtk::Dialog {
 public:
  DifferentViewWindow(Database::SharedPtr database);
  ~DifferentViewWindow() = default;

 protected:
  void on_view_selector_changed();
  void refresh_view(const std::string &view_name);

 private:
  class SqlColumns : public Gtk::TreeModel::ColumnRecord {
   public:
    explicit SqlColumns(int size) {
      for (int i = 0; i < size; ++i) {
        sql_columns.emplace_back();
        this->Gtk::TreeModel::ColumnRecord::add(sql_columns.back());
      }
    }
    std::vector<Gtk::TreeModelColumn<Glib::ustring>> sql_columns;
  };

  Gtk::ComboBoxText view_selector_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Book::ColumnRecord book_columns_;
  Database::SharedPtr database_;
};

}  // namespace lms
#endif  // LMS_DIFFERENT_VIEW_WINDOW_HPP_