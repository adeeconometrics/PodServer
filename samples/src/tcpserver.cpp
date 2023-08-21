#include <arpa/inet.h>
#include <array>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

struct TcpServer {
  TcpServer(const std::size_t t_port = 8080, const int t_backlog = 3)
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

  ~TcpServer() { close(m_server_socket); }

  auto upload(std::string_view t_msg) const -> ssize_t {
    return send(m_client_socket, t_msg.data(), t_msg.size(), 0);
  }

  auto download() -> const std::string {
    ssize_t bytes_read =
        read(m_client_socket, m_buffer.data(), m_buffer.size());
    if (bytes_read < 0) {
      throw std::runtime_error("Error while reading data from client socket");
    }
    return std::string{m_buffer.data()};
  }

  auto upload_file(const std::vector<char>& f_data) const -> void {
    ssize_t bytesSent =
        send(m_client_socket, f_data.data(), f_data.size(), 0);
    if (bytesSent < 0) {
      throw std::runtime_error("Error sending file data");
    }
  }

// test
  auto download_file(const std::string& f_path) -> void {
    ssize_t bytes_read = recv(m_client_socket, m_buffer.data(), m_buffer.size(), 0);
    if (bytes_read < 0) {
      throw std::runtime_error("Failure during recieving the file type");
    }

    std::ofstream file_stream(f_path, std::ios::binary);
    if(!file_stream.is_open()){
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

private:
  auto setup_server(const int t_backlog) -> void {
    if (bind(m_server_socket,
             reinterpret_cast<sockaddr *>(&m_server_addr) /*NOLINT*/,
             sizeof(m_server_addr)) < 0) {
      throw std::runtime_error("Bind failed");
    }

    if (listen(m_server_socket, t_backlog) < 0) {
      throw std::runtime_error("Listen failed");
    }
  }

  auto accept_connection() -> void {
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

private:
  static constexpr std::size_t buffer_size = 1024;
  std::vector<char> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
  int m_server_socket{};
};

auto main() -> int {
  std::cout << "listening: .... ";
  const std::filesystem::path parent_path = std::filesystem::current_path();
  const std::filesystem::path filepath = parent_path / "tcpserver" / "SampleVideo.mp4";
  TcpServer server{};
  // server.upload("Hello, Client!");
  // std::cout << "Client Mesage: " << server.download() << '\n';
  server.download_file(filepath);

  return 0;
}