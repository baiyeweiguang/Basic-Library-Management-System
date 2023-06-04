#ifndef LMS__ADD_BOOK_WINDOW_HPP_
#define LMS__ADD_BOOK_WINDOW_HPP_

// GTKMM
#include <gtkmm.h>
// Project
#include "database.hpp"

namespace lms {
class AddBookWindow : public Gtk::Dialog {
 public:
  explicit AddBookWindow(Database::SharedPtr database);
  ~AddBookWindow() = default;

 protected:
  void on_response(int response_id) override;

 private:
  Gtk::Grid grid_;
  Gtk::ComboBoxText category_combobox_;
  Gtk::ComboBoxText sex_combobox_;
  Gtk::Calendar calendar_;
  Gtk::Label title_label_, author_label_, isbn_label_, category_label_,
      unit_price_label_, quantity_label_, sex_label_, country_label_;
  Gtk::Entry title_entry_, author_entry_, isbn_entry_, unit_price_entry_,
      quantity_entry_, country_entry_;
  Database::SharedPtr database_;
  int current_book_id_;
};

}  // namespace lms
#endif  // LMS_ADD_BOOK_WINDOW_HPP_