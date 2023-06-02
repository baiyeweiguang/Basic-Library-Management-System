#include "lms/add_book_window.hpp"

namespace lms {
AddBookWindow::AddBookWindow(Database::SharedPtr database)
    : database_(database),
      title_label_("Title:"),
      category_label_("Category:"),
      author_label_("Author:"),
      isbn_label_("ISBN:") {
  this->Dialog::set_title("Add Book");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(400, 200);

  box_.set_spacing(10);
  this->Dialog::get_content_area()->pack_start(box_);

  box_.pack_start(title_label_);
  box_.pack_start(title_entry_);
  box_.pack_start(category_label_);
  box_.pack_start(category_combobox_);
  box_.pack_start(author_label_);
  box_.pack_start(author_entry_);
  box_.pack_start(isbn_label_);
  box_.pack_start(isbn_entry_);
  box_.pack_start(calendar_);

  add_button("OK", Gtk::RESPONSE_OK);
  add_button("Cancel", Gtk::RESPONSE_CANCEL);

  this->Dialog::show_all_children();
  category_combobox_.append("1: 科学");
  category_combobox_.append("2: 言情");
  category_combobox_.append("3: 科幻");
  category_combobox_.append("4: 历史");
  category_combobox_.append("5: 生活");
  category_combobox_.append("6: 体育");
}

void AddBookWindow::on_response(int response_id) {
  if (response_id == Gtk::RESPONSE_OK) {
    std::string title = title_entry_.get_text();
    std::string category_id =
        std::to_string(category_combobox_.get_active_row_number());
    std::string author = author_entry_.get_text();
    std::string isbn = isbn_entry_.get_text();

    // Get date from calendar
    std::tm publication_date = {};  // Initialize a std::tm struct
    unsigned int year, month, day;
    calendar_.get_date(year, month, day);
    publication_date.tm_year =
        static_cast<int>(year) - 1900;                      // Adjust the year
    publication_date.tm_mon = static_cast<int>(month) - 1;  // Adjust the month
    publication_date.tm_mday = static_cast<int>(day);

    if (title.empty() || author.empty() || isbn.empty() ||
        category_id.empty()) {
      Gtk::MessageDialog dialog(*this, "Error: All fields must be filled.");
      dialog.run();
      return;
    }
    Book new_book(title, category_id, author, isbn);
    new_book.set_publication_date(publication_date);
    database_->add_book(new_book);
  }
}

}  // namespace lms