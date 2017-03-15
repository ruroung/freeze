#include "net.h"
#include <iostream>
#include <unistd.h>

using namespace freeze;
using namespace std;

int main() {
  TcpConn tc;
  tc.connect("127.0.0.1", 6688);
  sleep(20);
  return 0;
}
