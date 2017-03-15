src= net.cc poller.cc

all:
	cd src && g++ -Wall -Wextra -g $(src) server.cc -o freezes
	cd src && g++ -Wall -Wextra -g $(src) client.cc -o freezec
