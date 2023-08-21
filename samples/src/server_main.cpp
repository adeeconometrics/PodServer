#include "../include/tcpserver.hpp"
#include <iostream>

auto main() -> int {
  std::cout << "listening: .... ";
  const std::filesystem::path parent_path = std::filesystem::current_path();
  const std::filesystem::path filepath =
      parent_path / "tcpserver" / "TestMp4.mkv";
  TcpServer server{};
  server.download_file(filepath);

  return 0;
}