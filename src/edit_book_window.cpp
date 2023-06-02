#include "lms/edit_book_window.hpp"

namespace lms {

EditBookWindow::EditBookWindow(Database::SharedPtr database, const Book &book)
    : database_(database), book_(book) {
  this->Dialog::set_title("Edit Book");
  this->Dialog::set_border_width(10);
  this->Dialog::set_default_size(400, 200);

  box_.set_orientation(Gtk::ORIENTATION_VERTICAL);
  box_.set_spacing(10);

  title_label_.set_label("Title:");
  box_.pack_start(title_label_, Gtk::PACK_SHRINK);

  title_entry_.set_text(database_->get_book_by_id(book_.get_id()).get_title());
  box_.pack_start(title_entry_, Gtk::PACK_SHRINK);

  author_label_.set_label("Author:");
  box_.pack_start(author_label_, Gtk::PACK_SHRINK);

  author_entry_.set_text(
      database_->get_book_by_id(book_.get_id()).get_author());
  box_.pack_start(author_entry_, Gtk::PACK_SHRINK);

  isbn_label_.set_label("ISBN:");
  box_.pack_start(isbn_label_, Gtk::PACK_SHRINK);

  isbn_entry_.set_text(database_->get_book_by_id(book_.get_id()).get_isbn());
  box_.pack_start(isbn_entry_, Gtk::PACK_SHRINK);

  save_button_.set_label("Save");
  box_.pack_start(save_button_, Gtk::PACK_SHRINK);
  save_button_.signal_clicked().connect(
      sigc::mem_fun(*this, &EditBookWindow::on_button_save));

  delete_button_.set_label("Delete");
  box_.pack_start(delete_button_, Gtk::PACK_SHRINK);
  delete_button_.signal_clicked().connect(
      sigc::mem_fun(*this, &EditBookWindow::on_button_delete));

  get_content_area()->pack_start(box_);
  show_all_children();
}

void EditBookWindow::on_button_save() {
  Book updated_book(book_.get_id(), title_entry_.get_text(),
                    author_entry_.get_text(), isbn_entry_.get_text());
  database_->update_book(updated_book);
  response(Gtk::RESPONSE_OK);
}

void EditBookWindow::on_button_delete() {
  database_->delete_book(book_.get_id());
  response(Gtk::RESPONSE_OK);
}

}  // namespace lms