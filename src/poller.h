#include <sys/epoll.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

namespace freeze {

const int maxevent = 1024;

class Poller {
 public:
  Poller();
  int addEvent(int sock_fd);
  int delEvent(int sock_fd);
  int poll(struct epoll_event* events, int max); 
  struct epoll_event *getPollEvents();
  int getMaxEvent();
 private:
  int epoll_fd_;
  struct epoll_event epoll_events_[maxevent];
};

}
