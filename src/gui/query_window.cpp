#include "lms/gui/query_window.hpp"

namespace lms {
QueryWindow::QueryWindow(Database::SharedPtr database)
    : database_(database), button_("query") {
  this->Dialog::set_title("Do Query");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(800, 600);

  // Create the scrolled window
  scrolled_window_.set_size_request(400, 200);
  scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  scrolled_window_.add(tree_view_);

  label_.set_text("Enter a query:");
  // Create the entry
  entry_.set_size_request(400, 30);
  // Create the button
  button_.set_label("Execute");
  button_.signal_clicked().connect(
      sigc::mem_fun(*this, &QueryWindow::on_button_clicked));

  vbox_.pack_start(label_, Gtk::PACK_SHRINK);
  vbox_.pack_start(entry_, Gtk::PACK_SHRINK);
  vbox_.pack_start(button_, Gtk::PACK_SHRINK);
  this->Dialog::get_content_area()->pack_start(vbox_);
  this->Dialog::get_content_area()->pack_start(scrolled_window_);
  this->Dialog::show_all_children();
}

void QueryWindow::update_tree_view(std::unique_ptr<sql::ResultSet> result_set) {
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

void QueryWindow::on_button_clicked() {
  std::string query = entry_.get_text();
  auto result = database_->execute_query(query);
  this->update_tree_view(std::move(result));
}

}  // namespace lms