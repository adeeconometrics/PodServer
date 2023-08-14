#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

constexpr int PORT = 8080;

auto main(int argc, char const *argv[]) -> int {
  int sock {};
  int valread {};
  int client_fd{};

  sockaddr_in serv_addr;
  
  const char *hello = "Hello from client";
  char buffer[1024] = {0};
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary
  // form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  if ((client_fd = connect(sock, (sockaddr *)&serv_addr,
                           sizeof(serv_addr))) < 0) {
    printf("\nConnection Failed \n");
    return -1;
  }

  send(sock, hello, strlen(hello), 0);
  printf("Hello message sent\n");
  valread = read(sock, buffer, 1024);
  printf("%s\n", buffer);

  // closing the connected socket
  close(client_fd);
  return 0;
}