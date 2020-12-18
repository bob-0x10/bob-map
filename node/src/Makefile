all: bob0x10_core

bob0x10_core: get_mac.o pkt.o main.o
	g++ -o bob0x10_core get_mac.o pkt.o main.o -std=c++11 -lpcap -lcurl -lpthread

get_mac.o: get_mac.cpp integrated.h
	g++ -c -o get_mac.o get_mac.cpp -std=c++11 -lpcap -lcurl -lpthread

pkt.o: pkt.cpp integrated.h
	g++ -c -o pkt.o pkt.cpp -std=c++11 -lpcap -lcurl -lpthread

main.o: main.cpp integrated.h
	g++ -c -o main.o main.cpp -std=c++11 -lpcap -lcurl -lpthread

clean:
	rm -f bob0x10_core *.o
