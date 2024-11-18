.PHONY:all
all:udp_server udp_client

udp_server:main.cc
	g++ -o $@ $^ -std=c++11
udp_client:UdpClient.cc
	g++ -o $@ $^ -std=c++11
	
.PHONY:clean
clean:
	rm -f udp_socket udp_client