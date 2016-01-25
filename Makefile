#CXX_FLAGS = -std=c++11 -I /usr/include -L /usr/lib -lboost_system -Wno-c++11-extensions 
CXX_FLAGS = -std=c++11 -L /usr/lib/x86_64-linux-gnu/ -lpthread -lboost_system -Wno-c++11-extensions 
OBJECTS = server/session.o server/server.o server/simple_packet_router.o \
	  server/main.o

all: server
server: output/server_bin

%.o: %.cpp
	g++ -std=c++11 -c $< -o $@

output/server_bin: $(OBJECTS)
	g++ -o output/server_bin $(OBJECTS) $(CXX_FLAGS) 
clean:
	rm server/*.o
	rm -rf output/*
.PHONY: clean
