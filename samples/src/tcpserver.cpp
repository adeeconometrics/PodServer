#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

struct TcpSocket {
public:
  TcpSocket() : m_socket(-1) {}

  explicit TcpSocket(int socket) : m_socket(socket) {}

  TcpSocket(TcpSocket &&other) noexcept : m_socket(other.m_socket) {
    other.m_socket = -1;
  }

  auto operator=(TcpSocket &&other) noexcept -> TcpSocket & {
    if (this != &other) {
      close();
      m_socket = other.m_socket;
      other.m_socket = -1;
    }
    return *this;
  }

  ~TcpSocket() { close(); }

  operator int() const { return m_socket; }

  auto close() -> void {
    if (m_socket != -1) {
      // ::close(m_socket);
      m_socket = -1;
    }
  }

private:
  int m_socket;
};

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

  auto upload(std::string t_msg) const -> ssize_t {
    return send(m_client_socket, t_msg.data(), t_msg.size(), 0);
  }

  auto download() -> const std::string {
    ssize_t bytes_read = read(m_client_socket, m_buffer.data(), m_buffer.size());
    if(bytes_read < 0){
      throw std::runtime_error("Error while reading data from client socket");
    }
    return std::string{m_buffer.data()};
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
    m_client_socket = static_cast<int>(TcpSocket(client_socket));
  }

private:
  static constexpr std::size_t buffer_size = 1024;
  std::array<char, buffer_size> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
  int m_server_socket{};
};

auto main() -> int {
  TcpServer server{};
  server.upload("Hello, Client!");
  std::cout << "Client Mesage: " << server.download() << '\n';
  return 0;
}