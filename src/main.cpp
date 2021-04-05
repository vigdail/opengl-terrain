#include "application.h"

int main() {
  try {
    Application app;
    app.run();
  } catch (const std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }
}
