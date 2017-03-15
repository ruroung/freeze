#include "poller.h"

namespace freeze {

Poller::Poller() {
  epoll_fd_ = epoll_create(1024);
  if (epoll_fd_ == -1) {
    perror("Poller epoll_create error.");
    exit(EXIT_FAILURE);
  }
}

int Poller::addEvent(int sock_fd) {
  struct epoll_event ee;
  ee.events = EPOLLIN;
  ee.data.fd = sock_fd; 
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, sock_fd, &ee) == -1) {
    perror("Poller epoll_ctl EPOLL_CTL_ADD error.");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int Poller::delEvent(int sock_fd) {
  struct epoll_event ee;
  ee.events = 0;
  ee.data.fd = sock_fd;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, sock_fd, &ee) == -1) {
    perror("Poller epoll_ctl EPOLL_CTL_DEL error.");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int Poller::poll(struct epoll_event *events, int max) {
  int retval, numevents = 0;
  retval = epoll_wait(epoll_fd_, events, max, -1); 
  if (retval > 0) {
    numevents = retval;
  }

  return numevents;
}

}
