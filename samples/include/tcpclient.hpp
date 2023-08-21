#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__

#include <arpa/inet.h>
#include <filesystem>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

struct TcpClient {
  TcpClient(const char *t_ip, const std::size_t t_port = 8080);
  ~TcpClient();

  auto upload_file(const std::filesystem::path &t_path) const -> ssize_t;
  // auto download_file(const std::filesystem::path &t_path,
  //                    std::string_view f_name) -> void;

private:
  std::vector<char> m_buffer;
  sockaddr_in m_server_addr;

  int m_client_socket{};
};

#endif // __TCPCLIENT_H__