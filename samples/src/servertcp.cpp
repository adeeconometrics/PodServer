// server.cpp
#include <arpa/inet.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <unistd.h>

constexpr int PORT = 12345;
constexpr int BUFFER_SIZE = 1024;

auto main() -> int {
  // Create socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1) {
    perror("Socket creation failed");
    return 1;
  }

  // Bind socket
  sockaddr_in serverAddr{};
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = INADDR_ANY;
  serverAddr.sin_port = htons(PORT);

  if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddr),
           sizeof(serverAddr)) == -1) {
    perror("Bind failed");
    close(serverSocket);
    return 1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, 5) == -1) {
    perror("Listen failed");
    close(serverSocket);
    return 1;
  }

  // Accept a client connection
  sockaddr_in clientAddr{};
  socklen_t clientAddrLen = sizeof(clientAddr);
  int clientSocket = accept(
      serverSocket, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
  if (clientSocket == -1) {
    perror("Accept failed");
    close(serverSocket);
    return 1;
  }

  // Receive file type information
  char fileType[BUFFER_SIZE];
  int fileTypeBytes = recv(clientSocket, fileType, sizeof(fileType), 0);
  if (fileTypeBytes <= 0) {
    perror("File type receive failed");
    close(clientSocket);
    close(serverSocket);
    return 1;
  }

    // Out Path
  const auto parent_path = std::filesystem::current_path();
  const auto filepath = parent_path / "tcpserver" / "PodServerLogo.png";
  // Open a file for writing
//   std::string fileName = "received_file." + std::string(fileType);
  std::ofstream outputFile(filepath, std::ios::binary);
  if (!outputFile) {
    perror("File open failed");
    close(clientSocket);
    close(serverSocket);
    return 1;
  }

  // Receive and write file data
  char buffer[BUFFER_SIZE];
  int bytesRead;
  while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
    outputFile.write(buffer, bytesRead);
  }

  // Close sockets and file
  close(clientSocket);
  close(serverSocket);
  outputFile.close();

  std::cout << "File received and saved as: " << filepath << std::endl;

  return 0;
}
