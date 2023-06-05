// GTKMM
#include <gtkmm/application.h>

// Project
#include "lms/database/database.hpp"
#include "lms/gui/main_window.hpp"

int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create(argc, argv, "com.example.library");
  using namespace lms;
  DatabaseConfig config = {.host = "127.0.0.1:3306",
                           .username = "root",
                           .password = "",
                           .schema = "book_management"};
  Database::SharedPtr database = std::make_shared<Database>(config);

  MainWindow main_window(database);

  return app->run(main_window);
}