#include "lms/add_book_window.hpp"

namespace lms {
AddBookWindow::AddBookWindow(Database::SharedPtr database)
    : database_(database),
      title_label_("Title:"),
      author_label_("Author:"),
      isbn_label_("ISBN:") {
  this->Dialog::set_title("Add Book");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(400, 200);

  box_.set_spacing(10);
  this->Dialog::get_content_area()->pack_start(box_);

  box_.pack_start(title_label_);
  box_.pack_start(title_entry_);
  box_.pack_start(author_label_);
  box_.pack_start(author_entry_);
  box_.pack_start(isbn_label_);
  box_.pack_start(isbn_entry_);

  add_button("OK", Gtk::RESPONSE_OK);
  add_button("Cancel", Gtk::RESPONSE_CANCEL);

  this->Dialog::show_all_children();
}

void AddBookWindow::on_response(int response_id) {
  if (response_id == Gtk::RESPONSE_OK) {
    std::string title = title_entry_.get_text();
    std::string author = author_entry_.get_text();
    std::string isbn = isbn_entry_.get_text();

    if (title.empty() || author.empty() || isbn.empty()) {
      Gtk::MessageDialog dialog(*this, "Error: All fields must be filled.");
      dialog.run();
      return;
    }

    Book new_book(title, author, isbn);
    database_->add_book(new_book);
  }
}

}  // namespace lms