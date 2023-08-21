#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <filesystem>
#include <netinet/in.h>
#include <vector>

struct TcpServer {
public:
  TcpServer(const std::size_t t_port = 8080, const int t_backlog = 5);
  ~TcpServer();

  auto download_file(const std::filesystem::path &t_path) -> void;
  // auto upload_file(const std::filesystem::path &t_path) -> void;

private:
  auto setup_server(const int t_backlog) -> void;
  auto accept_connection() -> void;

private:
  static constexpr std::size_t buffer_size = 1024;
  std::vector<char> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
  int m_server_socket{};
};

#endif // __TCPSERVER_H__