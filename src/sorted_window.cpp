#include "lms/sorted_books_window.hpp"

#include <algorithm>

namespace lms {

SortedBooksWindow::SortedBooksWindow(Database::SharedPtr database)
    : database_(database) {
  this->Dialog::set_title("Sorted Books");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(800, 600);

  sort_options_.append("Publication Date");
  sort_options_.append("Category");
  sort_options_.append("Id");
  sort_options_.set_active(0);
  sort_options_.signal_changed().connect(
      sigc::mem_fun(*this, &SortedBooksWindow::on_sort_option_changed));
  this->Dialog::get_content_area()->pack_start(sort_options_, Gtk::PACK_SHRINK);

  sort_order_.append("Ascending");
  sort_order_.append("Descending");
  sort_order_.set_active(0);
  sort_order_.signal_changed().connect(
      sigc::mem_fun(*this, &SortedBooksWindow::on_sort_option_changed));
  this->Dialog::get_content_area()->pack_start(sort_order_, Gtk::PACK_SHRINK);

  list_store_ = Gtk::ListStore::create(columns_);
  tree_view_.set_model(list_store_);
  tree_view_.append_column("ID", columns_.id);
  tree_view_.append_column("Title", columns_.title);
  tree_view_.append_column("Category", columns_.category);
  tree_view_.append_column("Author", columns_.author);
  tree_view_.append_column("ISBN", columns_.isbn);
  tree_view_.append_column("Publication Date", columns_.publication_date);

  scrolled_window_.add(tree_view_);
  scrolled_window_.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  this->Dialog::get_content_area()->pack_start(scrolled_window_);

  this->refresh_books();

  this->Dialog::show_all_children();
}

void SortedBooksWindow::on_sort_option_changed() {
  refresh_books();
}

void SortedBooksWindow::refresh_books() {
  list_store_->clear();
  std::vector<Book> books = database_->get_books();
  const std::string sort_option_string = sort_options_.get_active_text();
  const std::string sort_order_string = sort_order_.get_active_text();
  if (sort_option_string == "Id") {
    if (sort_order_string == "Descending") {
      std::sort(books.begin(), books.end(), [](const Book &a, const Book &b) {
        return a.get_id() > b.get_id();
      });
    } else {
      std::sort(books.begin(), books.end(), [](const Book &a, const Book &b) {
        return a.get_id() < b.get_id();
      });
    }
  }
  if (sort_option_string == "Publication Date") {
    if (sort_order_string == "Descending") {
      std::sort(books.begin(), books.end(), [](const Book &a, const Book &b) {
        auto t1 = a.get_publication_date();
        auto t2 = b.get_publication_date();
        return std::mktime(&t1) > std::mktime(&t2);
      });
    } else {
      std::sort(books.begin(), books.end(), [](const Book &a, const Book &b) {
        auto t1 = a.get_publication_date();
        auto t2 = b.get_publication_date();
        return std::mktime(&t1) < std::mktime(&t2);
      });
    }
  }
  if (sort_option_string == "Category") {
    if (sort_order_string == "Descending") {
      std::sort(books.begin(), books.end(), [&](const Book &a, const Book &b) {
        std::string a_category =
            database_->get_category_name_by_id(a.get_category_id());
        std::string b_category =
            database_->get_category_name_by_id(b.get_category_id());
        return a_category > b_category;
      });
    } else {
      std::sort(books.begin(), books.end(), [&](const Book &a, const Book &b) {
        std::string a_category =
            database_->get_category_name_by_id(a.get_category_id());
        std::string b_category =
            database_->get_category_name_by_id(b.get_category_id());
        return a_category < b_category;
      });
    }
  }

  for (const auto &book : books) {
    auto row = *(list_store_->append());
    row[columns_.id] = book.get_id();
    row[columns_.title] = book.get_title();
    std::string category_name =
        database_->get_category_name_by_id(book.get_category_id());
    row[columns_.category] = category_name;
    std::string author_name =
        database_->get_author_name_by_book_id(book.get_id());
    row[columns_.author] = author_name;
    row[columns_.isbn] = book.get_isbn();
    std::string publication_date =
        std::to_string(book.get_publication_date().tm_year + 1900) + "-" +
        std::to_string(book.get_publication_date().tm_mon + 1) + "-" +
        std::to_string(book.get_publication_date().tm_mday);
    row[columns_.publication_date] = publication_date;
  }
}

}  // namespace lms