#include <array>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 8080;

template <int BufferSize = 1024> struct ServerSocket {
  ServerSocket() : m_server_fd(socket(AF_INET, SOCK_STREAM, 0)) {
    if (m_server_fd < 0) {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }
  }

  ~ServerSocket() = default;

  auto attach(const std::size_t port) -> void {
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &m_opt,
                   sizeof(m_opt))) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(PORT);
  }

  auto accept() -> void;
  auto send() -> void;
  auto recv() -> void;

private:
  sockaddr_in m_address{};
  std::array<char, BufferSize> m_buffer{};
  std::size_t m_addr_len = sizeof(m_address);
  
  int m_server_fd{};
  int m_socket{};
  int m_opt{1};

};

auto main(int argc, char const *argv[]) -> int {
  int server_fd{};
  int new_socket{};
  int valread{};

  sockaddr_in address;

  int opt = 1;
  int addrlen = sizeof(address);

  std::array<char, 1024> buffer{};
  const char *hello = "Hello from server";

  // Creating socket file descriptor
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Forcefully attaching socket to port 8080
  if (bind(server_fd, reinterpret_cast<sockaddr *>(&address),
           sizeof(address) /*NOLINT*/) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if ((new_socket =
           accept(server_fd, reinterpret_cast<sockaddr *>(&address) /*NOLINT*/,
                  reinterpret_cast<socklen_t *>(&addrlen)) /*NOLINT*/) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  valread = read(new_socket, buffer.data(), buffer.size());
  printf("%s\n", buffer.data());
  send(new_socket, hello, strlen(hello), 0);
  printf("Hello message sent\n");

  // closing the connected socket
  close(new_socket);
  // closing the listening socket
  shutdown(server_fd, SHUT_RDWR);
  return 0;
}