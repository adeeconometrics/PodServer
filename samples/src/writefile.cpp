#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string_view>

auto main() -> int {
  const auto parent_path = std::filesystem::current_path();
  const auto filepath = parent_path / "tcpclient" / "example.txt";
  std::ifstream file(filepath, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    throw std::runtime_error("failed to open file for uploading");
  } 

  std::streamsize file_size = file.tellg();
  std::vector<char> file_data(file_size);
  file.seekg(0);
  file.read(file_data.data(), file_size);

  std::cout << std::string_view(file_data.data(), file_data.size());
  return 0;
}