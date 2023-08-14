#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

template <std::size_t BufferSize = 1024> struct TcpServer {
  TcpServer(const std::size_t t_port = 8080, const int t_backlog = 3)
      : m_server_socket(socket(AF_INET, SOCK_STREAM, 0)) {
    m_server_addr.sin_family = AF_INET;
    m_server_addr.sin_addr.s_addr = INADDR_ANY;
    m_server_addr.sin_port = htons(t_port);

    bind(m_server_socket, (sockaddr *)&m_server_addr, sizeof(m_server_addr));
    listen(m_server_socket, t_backlog);

    sockaddr_in client_addr{};
    socklen_t addrlen = sizeof(client_addr);
    m_client_socket =
        accept(m_server_socket,
               reinterpret_cast<sockaddr *>(&client_addr) /*NOLINT*/, &addrlen);
  }

  ~TcpServer() { close(m_server_socket); }

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
  int m_server_socket{};
};

auto main() -> int {
  TcpServer<> server{};
  server.upload("Hello, Client!");
  std::cout << "Client Mesage: " << server.download() << '\n';
  return 0;
}