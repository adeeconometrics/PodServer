#include "../include/TcpServer.hpp"

#include <arpa/inet.h>
#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

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

auto TcpServer::upload(std::string_view t_msg) const -> ssize_t {
  return send(m_client_socket, t_msg.data(), t_msg.size(), 0);
}

auto TcpServer::download() -> const std::string {
  ssize_t bytes_read = read(m_client_socket, m_buffer.data(), m_buffer.size());
  if (bytes_read < 0) {
    throw std::runtime_error("Error while reading data from client socket");
  }
  return std::string{m_buffer.data()};
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