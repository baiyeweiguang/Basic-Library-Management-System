#include "lms/main_window.hpp"
#include "lms/add_book_window.hpp"
#include "lms/edit_book_window.hpp"
#include "lms/sorted_books_window.hpp"

namespace lms {
MainWindow::MainWindow(Database::SharedPtr database)
    : database_(database), add_button_("Add Book"), sorted_books_button_("Sort") {
  this->Window::set_title("Library Management System");
  this->Window::set_border_width(10);
  this->Window::set_default_size(800, 600);

  add_button_.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_button_add));

  sorted_books_button_.signal_clicked().connect(
      sigc::mem_fun(*this, &MainWindow::on_sorted_books_button_clicked));

  main_box_.set_spacing(10);
  main_box_.pack_start(add_button_,Gtk::PACK_SHRINK);
  main_box_.pack_start(sorted_books_button_, Gtk::PACK_SHRINK);
  this->Window::add(main_box_);

  list_store_ = Gtk::ListStore::create(columns_);
  tree_view_.set_model(list_store_);
  tree_view_.append_column("ID", columns_.id);
  tree_view_.append_column("Title", columns_.title);
  tree_view_.append_column("Category", columns_.category);
  tree_view_.append_column("Author", columns_.author);
  tree_view_.append_column("ISBN", columns_.isbn);
  tree_view_.append_column("Publication Date", columns_.publication_date);
  tree_view_.signal_row_activated().connect(
      sigc::mem_fun(*this, &MainWindow::on_tree_view_row_activated));

  scrolled_window_.add(tree_view_);
  scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
//  main_box_.set_spacing(10);
  main_box_.pack_start(scrolled_window_);
//  this->Window::add(main_box_);

  this->refresh_books();

  this->Window::show_all_children();
}

void MainWindow::on_button_add() {
  AddBookWindow add_book_window(database_);
  auto result = add_book_window.run();

  if (result == Gtk::RESPONSE_OK) {
    this->refresh_books();
  }
}

void MainWindow::on_sorted_books_button_clicked() {
  SortedBooksWindow sorted_books_window(database_);
  sorted_books_window.run();
}

void MainWindow::on_tree_view_row_activated(const Gtk::TreeModel::Path &path,
                                            Gtk::TreeViewColumn* column) {
  using Gtk::TreeModel;
  TreeModel::iterator iter = list_store_->get_iter(path);
  if (iter) {
    TreeModel::Row row = *iter;
    int book_id = row[columns_.id];
    // Create a new EditBookWindow to edit or delete the book with the given ID.
    EditBookWindow edit_book_window(database_,
                                    database_->get_book_by_id(book_id));
    auto result = edit_book_window.run();
    if (result == Gtk::RESPONSE_OK) {
      this->refresh_books();
    }
  }
}

void MainWindow::refresh_books() {
  list_store_->clear();

  std::vector<Book> books = database_->get_books();
  for (const auto &book : books) {
    auto row = *(list_store_->append());
    row[columns_.id] = book.get_id();
    row[columns_.title] = book.get_title();
    std::string category_name =
        database_->get_category_name_by_id(std::stoi(book.get_category_id()));
    row[columns_.category] = category_name;
    row[columns_.author] = book.get_author();
    row[columns_.isbn] = book.get_isbn();
    std::string publication_date =
        std::to_string(book.get_publication_date().tm_year + 1900) + "-" +
        std::to_string(book.get_publication_date().tm_mon + 1) + "-" +
        std::to_string(book.get_publication_date().tm_mday);
    row[columns_.publication_date] = publication_date;
  }
}
}  // namespace lms