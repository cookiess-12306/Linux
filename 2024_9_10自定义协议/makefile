.PHONY:all
all:tcpserver tcpclient

tcpserver:TcpServerMain.cc
	g++ -o $@ $^ -std=c++14
tcpclient:TcpClientMain.cc
	g++ -o $@ $^ -std=c++14 -lpthread

.PHONY:clean
clean:
	rm -f tcpclient tcpserver
