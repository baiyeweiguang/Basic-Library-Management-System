#include "lms/database/sale.hpp"

namespace lms {
Sale::Sale(int id, int book_id, int quantity, double unit_price)
    : id_(id),
      book_id_(book_id),
      quantity_(quantity),
      unit_price_(unit_price) {}

Sale::Sale(int book_id, int quantity, double unit_price)
    : book_id_(book_id), quantity_(quantity), unit_price_(unit_price) {}

Sale::Sale() {}

int Sale::get_id() const {
  return id_;
}

int Sale::get_book_id() const {
  return book_id_;
}

int Sale::get_quantity() const {
  return quantity_;
}

double Sale::get_unit_price() const {
  return unit_price_;
}

}  // namespace lms