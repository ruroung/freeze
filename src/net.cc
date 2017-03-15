#include "net.h"

namespace freeze {

int setNonBlock(int fd, bool value) {
  int flags = fcntl(fd, F_GETFL, 0); 
  if (flags < 0) {
    perror("setNonBlock flags < 0");
    exit(EXIT_FAILURE);
  }
  if (value)
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  return fcntl(fd, F_SETFL, flags | ~O_NONBLOCK);
}

TcpConn::TcpConn():dest_host_(""),dest_port_(0) {}

int TcpConn::connect(const std::string& host, int port) {
  dest_host_ = host;
  dest_port_ = port;
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("TcpConn socket error.");
    exit(EXIT_FAILURE);
  }
 
  //setNonBlock(fd, true);

  struct sockaddr_in addr;
  addr.sin_addr.s_addr = inet_addr(dest_host_.c_str());
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port); 

  int r = ::connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
  if (r == -1) {
    perror("TcpConn connect error.");
    exit(EXIT_FAILURE);
  }
  
  char buff[1024];
  while (std::cin.getline(buff, 1024)) {
    printf("%s\n", buff);
    write(fd, buff, strlen(buff));
  } 
  return 0;
}


TcpServer::TcpServer():fd_(0) {}

int TcpServer::bind(const std::string& host, int port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("TcpServer socket error.");
    exit(EXIT_FAILURE);
  }

  if (host.size() > 0) {}

  fd_ = fd;
  setNonBlock(fd, true);

  struct sockaddr_in addr;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);

  int r = ::bind(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr));
  if (r) {
    close(fd);
    perror("TcpServer bind error.");
    exit(EXIT_FAILURE);
  }

  r = listen(fd, 20);

  int maxevent = poller_.getMaxEvent();
  struct epoll_event *events = poller_.getPollEvents();
  poller_.addEvent(fd);
  struct epoll_event ee;
  int i, n;
  int buff_size = 1024;
  char buff[buff_size];
  int readn;

  for (;;) {
    if ((n = poller_.poll(events, maxevent)) > 0) {
      for (i = 0; i < n; i++) {
        ee = events[i];
        if (ee.data.fd == fd_) {
        //socket listen fd has a new client
          accept(); 
        } else {
          if (ee.events & EPOLLIN) {
            //receive client fd message
            //printf("receive client message\n");
            int total_read_num = 0;
            int read_round = 0;
            while (read_round == 0 || (read_round > 0 && readn != 0 && readn >= buff_size)) {
              read_round++;
              readn = read(ee.data.fd, buff, buff_size);
              total_read_num += readn;
              if (readn < 0) {
                perror("read error.");
                exit(EXIT_FAILURE);
              }
              if (!total_read_num) {
                //socket disconnect
                printf("delete epoll event fd\n");
                poller_.delEvent(ee.data.fd);
                close(ee.data.fd);
                printf("fd %d disconnect\n", ee.data.fd);
              } else if (readn > 0) {
                buff[readn] = '\0';
                printf("msg:%s\n", buff);
              }
            }
          }
        }
      }
    }
  }

  return 0;
}

//int TcpServer::startServer(const std::string& host, int port) {}
int TcpServer::accept() {
  struct sockaddr_in client_addr;
  socklen_t len = sizeof(client_addr);
  int cfd;
  
  if (fd_ > 0 && (cfd = ::accept(fd_, (struct sockaddr*)&client_addr, &len))) {
    setNonBlock(cfd, true);
    poller_.addEvent(cfd);
    printf("new client fd %d\n", cfd); 
  }
  /*
  while (fd_ > 0 && (cfd = ::accept(fd_, (struct sockaddr*)&client_addr, &len))) {
    printf("new client fd %d\n", cfd); 
  }
  */

  return 1;
}

}
