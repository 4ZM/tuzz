/**
 * Copyright (C) 2012 Anders Sundman <anders@4zm.org>
 *
 * This file is part of tuzz - the text fuzzer.
 *
 * tuzz is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tuzz is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tuzz.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void fatal(const char* msg);
void handle_client(int conn_fd, struct sockaddr_in* client_addr);
int start_server(int port);

int main(int argc, const char* argv[]) {

  int port = 0;
  if (argc == 2)
    port = atoi(argv[1]);

  if (argc != 2 || port < 1 || port > 0xffff) {
    std::cout << "Accepts 256 bytes on a network socket and copies to 128 byte buffer." << std::endl;
    std::cout << "Usage: " << argv[0] << " port" << std::endl;
    return 0;
  }

  int sock_fd = start_server(port);

  for(;;) {
    int conn_fd;
    struct sockaddr_in client_addr;
    socklen_t sin_size = sizeof(struct sockaddr);

    if ((conn_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &sin_size)) == -1)
      fatal("accepting connection");

    handle_client(conn_fd, &client_addr);

    close(conn_fd);
  }

  return 0;
}

int start_server(int port) {
  int sock_fd;
  socklen_t sin_size = sizeof(struct sockaddr);

  if ((sock_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    fatal("creating listening socket");

  int yes = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
    fatal("setting socket option");

  struct sockaddr_in host_addr;
  host_addr.sin_family = AF_INET;
  host_addr.sin_port = htons(port);
  host_addr.sin_addr.s_addr = 0;
  memset(&(host_addr.sin_zero), 0, 8);

  if (bind(sock_fd, (struct sockaddr*)&host_addr, sin_size) == -1)
    fatal("binding to socket");

  if (listen(sock_fd, 5) == -1)
    fatal("listening on socket");

  return sock_fd;
}

void handle_client(int conn_fd, struct sockaddr_in* client_addr) {
    // This is where the bug is! 256 -> 128 unchecked
    char buff[128];
    int recv_len = recv(conn_fd, &buff, 256, 0);

    send(conn_fd, "OK\n", 4, 0);
}

void fatal(const char* msg) {
  std::cout << "Fatal error: " << msg << std::endl;
  exit(1);
}
