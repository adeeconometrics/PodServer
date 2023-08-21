#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

// #include "../include/Utils.hpp"

#include <arpa/inet.h>
#include <vector>
#include <string>
#include <string_view>
#include <filesystem>
#include <sys/socket.h>

struct TcpServer {
public:
  TcpServer(const std::size_t t_port = 8080, const int t_backlog = 3);
  ~TcpServer();
  
  auto upload(std::string_view t_msg) const -> ssize_t;
  auto download() -> const std::string;

  auto upload_file(const std::filesystem::path& t_file) -> void;
  auto download_file(const std::filesystem::path& t_file) -> void;

private: 
    auto setup_server(const int t_backlog) -> void;
    auto accept_connection() -> void;

private: 
    static constexpr std::size_t buffer_size = 1024;
    std::vector<char> m_buffer{}; // initialize upon decl, need to observe perf cost
    sockaddr_in m_server_addr;

    int m_client_socket{};
    int m_server_socket{};
};

#endif // __TCPSERVER_H__