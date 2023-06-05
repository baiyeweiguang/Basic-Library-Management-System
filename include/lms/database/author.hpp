#ifndef LMS__AUTHOR_HPP_
#define LMS__AUTHOR_HPP_

// GTKMM
#include <gtkmm/treemodel.h>

namespace lms {
class Author {
 public:
  class ColumnRecord : public Gtk::TreeModel::ColumnRecord {
   public:
    ColumnRecord() {
      this->ColumnRecord::add(id);
      this->ColumnRecord::add(name);
      this->ColumnRecord::add(nationality);
      this->ColumnRecord::add(sex);
    }
    Gtk::TreeModelColumn<int> id;
    Gtk::TreeModelColumn<Glib::ustring> name;
    Gtk::TreeModelColumn<Glib::ustring> nationality;
    Gtk::TreeModelColumn<Glib::ustring> sex;
  };

  Author(int id, int book_id, const std::string &name, const std::string &nationality,
         const std::string &sex);
  Author(int book_id, const std::string &name, const std::string &nationality,
         const std::string &sex);
  Author();

  int get_id() const;
  int get_book_id() const;
  const std::string &get_name() const;
  const std::string &get_sex() const;
  const std::string &get_nationality() const;

 private:
  int id_;
  int book_id_;
  std::string name_;
  std::string nationality_;
  std::string sex_;
};

}  // namespace lms
#endif  // LMS__AUTHOR_HPP_