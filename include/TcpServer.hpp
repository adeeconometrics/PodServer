#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <arpa/inet.h>
#include <array>
#include <string>
#include <string_view>
#include <sys/socket.h>

struct TcpServer {
public:
  TcpServer(const std::size_t t_port = 8080, const int t_backlog = 3);
  ~TcpServer();
  
  auto upload(std::string_view t_msg) const -> ssize_t;
  auto download() -> const std::string;

private: 
    auto setup_server(const int t_backlog) -> void;
    auto accept_connection() -> void;

private: 
    static constexpr std::size_t buffer_size = 1024;
    std::array<char, buffer_size> m_buffer{};
    sockaddr_in m_server_addr;

    int m_client_socket{};
    int m_server_socket{};
};

#endif // __TCPSERVER_H__