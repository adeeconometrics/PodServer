#include <arpa/inet.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

struct TcpClient {

  TcpClient(const char *t_ip, const std::size_t t_port = 8080)
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

  ~TcpClient() {
    close(m_client_socket);
  } // check is this one is closed when the server dies

  auto upload(const std::string &t_msg) const -> ssize_t {
    return send(m_client_socket, t_msg.data(), t_msg.size(), 0);
  }

  auto upload_file(const std::filesystem::path &f_path) const -> ssize_t {
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

  auto download_file() -> std::vector<char> {
    ssize_t bytes_read =
        read(m_client_socket, m_buffer.data(), m_buffer.size());
    if (bytes_read < 0) {
      throw std::runtime_error("Error reading from client socket");
    }
    return std::vector<char>(m_buffer.data(), m_buffer.data() + bytes_read);
  }

  auto download() -> const std::string {
    const ssize_t bytes_read =
        read(m_client_socket, m_buffer.data(), m_buffer.size());
    if (bytes_read < 0) {
      throw std::runtime_error("Error reading from client socket");
    }
    return std::string(m_buffer.data(), static_cast<std::size_t>(bytes_read));
  }

private:
  // static constexpr std::size_t buffer_size = 1024;
  std::vector<char> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
};

auto main() -> int {
  const std::filesystem::path parent_path = std::filesystem::current_path();
  const std::filesystem::path filepath = parent_path / "tcpclient" / "TestVideo.mp4";

  if (std::filesystem::exists(filepath)) {
    std::cout << "file exists and detected\n";
  }

  TcpClient client{"127.0.0.1"};
  client.upload_file(filepath);

  std::cout << "file uploaded\n";
  // client.download_file();

  return 0;
}