#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

struct TcpClient {

  TcpClient(const char *t_ip, const std::size_t t_port = 8080,
            const int t_backlog = 3)
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

  auto download() -> const std::string {
    const ssize_t bytesRead = read(m_client_socket, m_buffer.data(), m_buffer.size());
    if (bytesRead < 0) {
      throw std::runtime_error("Error reading from client socket");
    }
    return std::string(m_buffer.data(), static_cast<std::size_t>(bytesRead));
  }

private:
  static constexpr std::size_t buffer_size = 1024;
  std::array<char, buffer_size> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
};

auto main() -> int {
  TcpClient client{"127.0.0.1"};
  client.upload("Hello, Server!");
  std::cout << "Client Mesage: " << client.download() << '\n';

  return 0;
}