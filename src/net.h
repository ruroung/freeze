#include <string>
#include <sys/types.h>  // socket()
#include <sys/socket.h> // socket()
#include <arpa/inet.h>  // htons()

#include <errno.h>
#include <stdio.h> // perror
#include <stdlib.h> // exit EXIT_SUCCESS EXIT_FAILURE

#include <unistd.h> // fcntl()
#include <fcntl.h> // fcntl()

namespace freeze {

int setNonBlock(int fd, bool value);

class TcpConn {
 public:
  TcpConn();
  int connect(const std::string& host, int port);
 private:
  std::string dest_host_;
  int dest_port_;    
};

class TcpServer{
 public:
  TcpServer();
  int bind(const std::string& host, int port);
  //int startServer(const std::string& host, int port);
  int accept();
 private:
  int fd_;
};

}
