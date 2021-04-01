#include "util.h"

std::string ReadFile(std::string_view path) {
  std::ifstream file{path.data()};

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path.data() << '\n';
    throw std::runtime_error("Failed to open file");
  }

  std::stringstream ss;
  ss << file.rdbuf();

  return ss.str();
}
