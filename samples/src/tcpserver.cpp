#include "../include/tcpserver.hpp"

#include <arpa/inet.h>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

TcpServer::TcpServer(const std::size_t t_port, const int t_backlog)
    : m_server_socket(socket(AF_INET, SOCK_STREAM, 0)) {

  if (m_server_socket < 0) {
    throw std::runtime_error("Failed to create server socket");
  }

  m_server_addr.sin_family = AF_INET;
  m_server_addr.sin_addr.s_addr = INADDR_ANY;
  m_server_addr.sin_port = htons(t_port);

  setup_server(t_backlog);
  accept_connection();
}

TcpServer::~TcpServer() { close(m_server_socket); }

// auto TcpServer::upload_file(const std::filesystem::path &t_path) -> void {
//   if (std::filesystem::exists(t_path)){
//     throw std::runtime_error("file path does not exist: " + static_cast<std::string>(t_path));
//   }

//   ssize_t bytesSent = send(m_client_socket, m_buffer.data(), m_buffer.size(), 0);
//   if (bytesSent < 0) {
//     throw std::runtime_error("Error sending file data");
//   }
// }

// test
auto TcpServer::download_file(const std::filesystem::path &t_path) -> void {
  ssize_t bytes_read =
      recv(m_client_socket, m_buffer.data(), m_buffer.size(), 0);
  if (bytes_read < 0) {
    throw std::runtime_error("Failure during recieving the file type");
  }

  std::ofstream file_stream(t_path, std::ios::binary);
  if (!file_stream.is_open()) {
    throw std::runtime_error("failed to open for saving");
  }

  if (!file_stream) {
    throw std::runtime_error("failed to open file for writing");
  }

  std::vector<char> fetched_data(buffer_size);
  for (int bytes =
           recv(m_client_socket, fetched_data.data(), fetched_data.size(), 0);
       bytes > 0; bytes = recv(m_client_socket, fetched_data.data(),
                               fetched_data.size(), 0)) {
    file_stream.write(fetched_data.data(), bytes);
  }

  file_stream.close();
}

auto TcpServer::setup_server(const int t_backlog) -> void {
  if (bind(m_server_socket,
           reinterpret_cast<sockaddr *>(&m_server_addr) /*NOLINT*/,
           sizeof(m_server_addr)) < 0) {
    throw std::runtime_error("Bind failed");
  }

  if (listen(m_server_socket, t_backlog) < 0) {
    throw std::runtime_error("Listen failed");
  }
}

auto TcpServer::accept_connection() -> void {
  sockaddr_in client_addr{};
  socklen_t addrlen = sizeof(client_addr);
  int client_socket =
      accept(m_server_socket,
             reinterpret_cast<sockaddr *>(&client_addr) /*NOLINT*/, &addrlen);
  if (client_socket < 0) {
    throw std::runtime_error("Accept failed");
  }
  m_client_socket = client_socket;
}