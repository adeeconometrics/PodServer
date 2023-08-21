#include "../include/tcpclient.hpp"

#include <arpa/inet.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

TcpClient::TcpClient(const char *t_ip, const std::size_t t_port)
    : m_client_socket(socket(AF_INET, SOCK_STREAM, 0)) {
  m_server_addr.sin_family = AF_INET;
  m_server_addr.sin_port = htons(t_port);
  inet_pton(AF_INET, t_ip, &m_server_addr.sin_addr);

  if (connect(m_client_socket,
              reinterpret_cast<sockaddr *>(&m_server_addr) /*NOLINT*/,
              sizeof(m_server_addr))) {
    throw std::runtime_error("Connection failed");
  }
}

TcpClient::~TcpClient() { close(m_client_socket); }

auto TcpClient::upload_file(const std::filesystem::path &f_path) const
    -> ssize_t {
  std::ifstream file_stream{f_path, std::ios::binary | std::ios::ate};

  if (!file_stream.is_open()) {
    throw std::runtime_error("failed to open file for uploading");
  }

  std::streamsize file_size = file_stream.tellg();
  std::vector<char> file_data(file_size);
  file_stream.seekg(0);
  file_stream.read(file_data.data(), file_size);

  return send(m_client_socket, file_data.data(), file_size, 0);
}

// auto TcpClient::download_file(const std::filesystem::path &t_path,
//                               std::string_view f_name) -> void {
//   ssize_t bytes_read = read(m_client_socket, m_buffer.data(), m_buffer.size());
//   if (bytes_read < 0) {
//     throw std::runtime_error("Error reading from client socket");
//   }
//   // return std::vector<char>(m_buffer.data(), m_buffer.data() + bytes_read);
// }
