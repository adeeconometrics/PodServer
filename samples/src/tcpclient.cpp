#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

template <std::size_t BufferSize = 1024> struct TcpClient {

  TcpClient(const char *t_ip, const std::size_t t_port = 8080,
            const int t_backlog = 3)
      : m_client_socket(socket(AF_INET, SOCK_STREAM, 0)) {
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_port = htons(t_port);
    inet_pton(AF_INET, t_ip, &m_server_addr.sin_addr);

    connect(m_client_socket, (sockaddr *)&m_server_addr, sizeof(m_server_addr));
  }

  ~TcpClient() { close(m_client_socket); }

  auto upload(const std::string &t_msg) const -> ssize_t {
    return send(m_client_socket, t_msg.data(), t_msg.size(), 0);
  }

  auto download() -> const std::string {
    read(m_client_socket, m_buffer.data(), m_buffer.size());
    return {m_buffer.data()};
  }

private:
  std::array<char, BufferSize> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
};

auto main() -> int {
  TcpClient<> client{"127.0.0.1"};
  client.upload("Hello, Server!");
  std::cout << "Client Mesage: " << client.download() << '\n';

  return 0;
}