#include <sys/epoll.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

namespace freeze {

class Poller {
 public:
  Poller();
  int addEvent(int sock_fd);
  int delEvent(int sock_fd);
  int poll(struct epoll_event* events, int max); 
 private:
  int epoll_fd_;
};

}
