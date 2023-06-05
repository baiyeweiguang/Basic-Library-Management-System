#include "lms/gui/different_view_window.hpp"

namespace lms {
DifferentViewWindow::DifferentViewWindow(Database::SharedPtr database)
    : database_(database) {
  // Set the title of the window
  this->Dialog::set_title("Different View");

  // Set the border width of the window.
  this->Dialog::set_border_width(10);

  // Set the window's default size.
  this->Dialog::set_default_size(800, 600);

  // Set the window's position.
  this->Dialog::set_position(Gtk::WIN_POS_CENTER);

  // Set up the view selector
  view_selector_.append("Book Details");
  view_selector_.append("Sales");
  view_selector_.append("Category Details");
  view_selector_.set_active(1);
  view_selector_.signal_changed().connect(
      sigc::mem_fun(*this, &DifferentViewWindow::on_view_selector_changed));
  this->Dialog::get_content_area()->pack_start(view_selector_,
                                               Gtk::PACK_SHRINK);

  // Set up the scrolled window
  scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  // Set up the tree view
  tree_view_.set_headers_visible(true);
  this->refresh_view("Sales");

  scrolled_window_.add(tree_view_);
  this->Dialog::get_content_area()->pack_start(scrolled_window_);
  // Show all children
  this->Dialog::show_all_children();
}

void DifferentViewWindow::on_view_selector_changed() {
  refresh_view(view_selector_.get_active_text());
}

void DifferentViewWindow::refresh_view(const std::string &view_name) {
  std::string query;
  if (view_name == "Book Details") {
    query = "SELECT * FROM book_details";
  } else if (view_name == "Sales") {
    query = "SELECT * FROM book_prices";
  } else if (view_name == "Category Details") {
    query = "SELECT * FROM category_details";
  }
  auto result_set = database_->execute_query(query);

  tree_view_.remove_all_columns();
  if (result_set == nullptr) {
    return;
  }

  // Get the metadata for the result set
  sql::ResultSetMetaData* metadata = result_set->getMetaData();
  int num_columns = metadata->getColumnCount();
  SqlColumns columns(num_columns);
  list_store_ = Gtk::ListStore::create(columns);

  // Add the columns to the tree view
  for (int i = 0; i < num_columns; i++) {
    std::string title = metadata->getColumnLabel(i + 1);
    tree_view_.append_column(title, columns.sql_columns[i]);
  }

  // Add the rows to the list store
  while (result_set->next()) {
    Gtk::TreeModel::Row row = *(list_store_->append());
    for (int i = 0; i < num_columns; i++) {
      row[columns.sql_columns[i]] =
          static_cast<std::string>(result_set->getString(i + 1));
    }
  }

  // Attach the list store to the tree view
  tree_view_.set_model(list_store_);
}

}  // namespace lms