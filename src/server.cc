#include "net.h"

using namespace freeze;

int main() {
  TcpServer ts;
  ts.bind("", 6688);
  return 0;
}
