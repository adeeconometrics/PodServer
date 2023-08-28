#include "../include/tcpclient.hpp"
#include <iostream>

auto main() -> int {
  const std::filesystem::path parent_path = std::filesystem::current_path();
  const std::filesystem::path filepath =
      parent_path / "tcpclient";

  if (std::filesystem::exists(filepath)) {
    std::cout << "file exists and detected\n";
  }

  TcpClient client{"127.0.0.1"};
  client.download_file(filepath, "SampleVideo.mp4");

  std::cout << "file uploaded\n";
  // client.download_file();

  return 0;
}