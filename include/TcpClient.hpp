#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <arpa/inet.h>
#include <array>
#include <string>
#include <string_view>
#include <sys/socket.h>

struct TcpClient {
public:
  TcpClient(const char *t_ip, const std::size_t t_port = 8080,
            const int t_backlog = 3);
  ~TcpClient();

  auto upload(std::string_view t_msg) const -> ssize_t;
  auto download() -> const std::string;

private:
  static constexpr std::size_t buffer_size = 1024;
  std::array<char, buffer_size> m_buffer{};
  sockaddr_in m_server_addr;

  int m_client_socket{};
};

#endif // __TCPCLIENT_H__