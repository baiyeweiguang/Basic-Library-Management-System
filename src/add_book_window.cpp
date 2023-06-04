#include "lms/add_book_window.hpp"
#include "lms/author.hpp"
#include "lms/sale.hpp"

namespace lms {
AddBookWindow::AddBookWindow(Database::SharedPtr database)
    : database_(database),
      title_label_("Title:"),
      category_label_("Category:"),
      author_label_("Author:"),
      isbn_label_("ISBN:"),
      sex_label_("Author Sex"),
      country_label_("Author Country"),
      unit_price_label_("Unit Price:"),
      quantity_label_("Quantity:") {
  current_book_id_ = database_->get_last_insert_book_id() + 1;
  this->Dialog::set_title("Add Book");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(400, 200);

  grid_.attach(title_label_, 0, 0, 1, 1);
  grid_.attach(title_entry_, 0, 1, 1, 1);

  grid_.attach(isbn_label_, 0, 2, 1, 1);
  grid_.attach(isbn_entry_, 0, 3, 1, 1);

  grid_.attach(unit_price_label_, 0, 4, 1, 1);
  grid_.attach(unit_price_entry_, 0, 5, 1, 1);

  grid_.attach(quantity_label_, 0, 6, 1, 1);
  grid_.attach(quantity_entry_, 0, 7, 1, 1);

  grid_.attach(author_label_, 1, 0, 1, 1);
  grid_.attach(author_entry_, 1, 1, 1, 1);

  grid_.attach(sex_label_, 1, 2, 1, 1);
  grid_.attach(sex_combobox_, 1, 3, 1, 1);

  grid_.attach(country_label_, 1, 4, 1, 1);
  grid_.attach(country_entry_, 1, 5, 1, 1);

  grid_.attach(category_label_, 1, 6, 1, 1);
  grid_.attach(category_combobox_, 1, 7, 1, 1);

  grid_.attach(calendar_, 0, 8, 1, 1);

  this->Dialog::get_content_area()->pack_start(grid_);

  add_button("OK", Gtk::RESPONSE_OK);
  add_button("Cancel", Gtk::RESPONSE_CANCEL);

  this->Dialog::show_all_children();
  category_combobox_.append("1: 科学");
  category_combobox_.append("2: 言情");
  category_combobox_.append("3: 科幻");
  category_combobox_.append("4: 历史");
  category_combobox_.append("5: 生活");
  category_combobox_.append("6: 体育");
  sex_combobox_.append("Man");
  sex_combobox_.append("Female");
}

void AddBookWindow::on_response(int response_id) {
  if (response_id == Gtk::RESPONSE_OK) {
    std::string title = title_entry_.get_text();
    std::string category_id =
        std::to_string(category_combobox_.get_active_row_number());
    std::string author_name = author_entry_.get_text();
    std::string isbn = isbn_entry_.get_text();
    std::string unit_price = unit_price_entry_.get_text();
    std::string quantity = quantity_entry_.get_text();
    std::string sex = sex_combobox_.get_active_text();
    std::string country = country_entry_.get_text();

    // Get date from calendar
    std::tm publication_date = {};  // Initialize a std::tm struct
    unsigned int year, month, day;
    calendar_.get_date(year, month, day);
    publication_date.tm_year =
        static_cast<int>(year) - 1900;                      // Adjust the year
    publication_date.tm_mon = static_cast<int>(month) - 1;  // Adjust the month
    publication_date.tm_mday = static_cast<int>(day);

    if (title.empty() || author_name.empty() || isbn.empty() ||
        category_id.empty()) {
      Gtk::MessageDialog dialog(*this, "Error: All fields must be filled.");
      dialog.run();
      return;
    }
    Book new_book(current_book_id_, title, std::stoi(category_id), isbn);
    new_book.set_publication_date(publication_date);
    database_->add_book(new_book);
    Author author(current_book_id_, author_name, country, sex);
    database_->add_author(author);
    Sale sale(current_book_id_, std::stoi(quantity), std::stod(unit_price));
    database_->add_sale(sale);
    current_book_id_ += 1;
  }
}

}  // namespace lms