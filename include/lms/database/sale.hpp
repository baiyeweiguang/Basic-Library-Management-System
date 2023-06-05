#ifndef LMS__SALE_HPP_
#define LMS__SALE_HPP_

// GTKMM
#include <gtkmm/treemodel.h>

namespace lms {
class Sale {
 public:
  class ColumnRecord : public Gtk::TreeModel::ColumnRecord {
   public:
    ColumnRecord() {
      this->ColumnRecord::add(id);
      this->ColumnRecord::add(book_id);
      this->ColumnRecord::add(quantity);
      this->ColumnRecord::add(unit_price);
    }
    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<int> book_id;
    Gtk::TreeModelColumn<int> quantity;
    Gtk::TreeModelColumn<double> unit_price;
  };
  Sale(int id, int book_id, int quantity, double unit_price);
  Sale(int book_id, int quantity, double unit_price);
  Sale();

  int get_id() const;
  int get_book_id() const;
  int get_quantity() const;
  double get_unit_price() const;

 private:
  int id_;
  int book_id_;
  int quantity_;
  double unit_price_;
};

}  // namespace lms
#endif