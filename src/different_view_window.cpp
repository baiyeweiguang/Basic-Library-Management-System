#include "lms/different_view_window.hpp"

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
  view_selector_.append("Books with Information");
  view_selector_.append("Sales");
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
  this->Dialog::get_content_area()->pack_start(scrolled_window_,
                                               Gtk::PACK_SHRINK);
  // Show all children
  this->Dialog::show_all_children();
}

void DifferentViewWindow::on_view_selector_changed() {
  refresh_view(view_selector_.get_active_text());
}

void DifferentViewWindow::refresh_view(const std::string &view_name) {
  std::vector<Book> books = database_->get_books();
  if (view_name == "Books with Information") {
    list_store_.reset();
    tree_view_.remove_all_columns();
    list_store_ = Gtk::ListStore::create(book_columns_);
    tree_view_.set_model(list_store_);
    tree_view_.append_column("ID", book_columns_.id);
    tree_view_.append_column("Title", book_columns_.title);
    tree_view_.append_column("Category", book_columns_.category);
    tree_view_.append_column("Author", book_columns_.author);
    tree_view_.append_column("ISBN", book_columns_.isbn);
    tree_view_.append_column("Publication Date",
                             book_columns_.publication_date);
    for (const auto &book : books) {
      auto row = *(list_store_->append());
      row[book_columns_.id] = book.get_id();
      row[book_columns_.title] = book.get_title();
      row[book_columns_.category] =
          database_->get_category_name_by_id(book.get_category_id());
      row[book_columns_.author] =
          database_->get_author_name_by_book_id(book.get_id());
      row[book_columns_.isbn] = book.get_isbn();
      std::string publication_date =
          std::to_string(book.get_publication_date().tm_year + 1900) + "-" +
          std::to_string(book.get_publication_date().tm_mon + 1) + "-" +
          std::to_string(book.get_publication_date().tm_mday);
      row[book_columns_.publication_date] = publication_date;
    }
  }
  if (view_name == "Sales") {
    list_store_.reset();
    list_store_ = Gtk::ListStore::create(sale_columns_);
    tree_view_.set_model(list_store_);
    tree_view_.remove_all_columns();
    tree_view_.append_column("ID", sale_columns_.id);
    tree_view_.append_column("Title", sale_columns_.title);
    tree_view_.append_column("Quantity", sale_columns_.quantity);
    tree_view_.append_column("Unit Price", sale_columns_.unit_price);
    tree_view_.append_column("Total Price", sale_columns_.total_price);
    tree_view_.append_column("Average Price", sale_columns_.avg_price);
    for (const auto &book : books) {
      int sale_id = database_->get_sale_id_by_book_id(book.get_id());
      auto row = *(list_store_->append());
      row[sale_columns_.id] = book.get_id();
      row[sale_columns_.title] = book.get_title();
      row[sale_columns_.quantity] = database_->get_quantity_by_id(sale_id);
      row[sale_columns_.unit_price] = database_->get_unit_price_by_id(sale_id);
      row[sale_columns_.total_price] =
          database_->get_total_sales_by_id(sale_id);
      row[sale_columns_.avg_price] = database_->get_avg_price_from_sales();
    }
  }
}

}  // namespace lms