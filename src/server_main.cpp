#include "../include/tcpserver.hpp"
#include <iostream>

auto main() -> int {
  std::cout << "listening: .... ";
  const std::filesystem::path parent_path = std::filesystem::current_path();
  const std::filesystem::path filepath =
      parent_path / "tcpserver";
  TcpServer server{};
  server.upload_file(filepath);

  return 0;
}