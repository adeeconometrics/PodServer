// client.cpp
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include <filesystem>

constexpr int PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

auto main() -> int {
  // Create socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1) {
    perror("Socket creation failed");
    return 1;
  }

  // Connect to server
  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
    perror("Invalid address");
    close(clientSocket);
    return 1;
  }

  if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddr),
              sizeof(serverAddr)) == -1) {
    perror("Connection failed");
    close(clientSocket);
    return 1;
  }

  // Send file type information
  std::string fileType = "jpg"; // Replace with your desired file type
  send(clientSocket, fileType.c_str(), fileType.size(), 0);

    // Define path
  const auto parent_path = std::filesystem::current_path();
  const auto filepath = parent_path / "tcpclient" / "PodServerLogo.png";

  // Open the file for reading
  std::ifstream inputFile(filepath, std::ios::binary);
  if (!inputFile) {
    perror("File open failed");
    close(clientSocket);
    return 1;
  }

  // Send file data
  char buffer[BUFFER_SIZE];
  while (inputFile.read(buffer, sizeof(buffer))) {
    send(clientSocket, buffer, inputFile.gcount(), 0);
  }

  // Close socket and file
  close(clientSocket);
  inputFile.close();

  std::cout << "File sent successfully." << std::endl;

  return 0;
}
