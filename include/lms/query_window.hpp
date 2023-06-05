#ifndef LMS__QUERY_WINDOW_HPP_
#define LMS__QUERY_WINDOW_HPP_

// GTKMM
#include <gtkmm.h>

// Project
#include "lms/database.hpp"

namespace lms {
class QueryWindow : public Gtk::Dialog {
 public:
  QueryWindow(Database::SharedPtr database);

 protected:
  void on_button_clicked();
  void update_tree_view(std::unique_ptr<sql::ResultSet> result_set);

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

  Gtk::VBox vbox_;
  Gtk::Label label_;
  Gtk::Button button_;
  Gtk::TreeView tree_view_;
  Glib::RefPtr<Gtk::ListStore> list_store_;
  Gtk::ScrolledWindow scrolled_window_;
  Gtk::Entry entry_;
  Database::SharedPtr database_;
};

}  // namespace lms
#endif  // LMS__QUERY_WINDOW_HPP_
