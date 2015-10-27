OBJS = console_gist.o message.pb.o
CC = clang++
DEBUG = -g
CFLAGS = -std=c++0x -W -Wall -c $(DEBUG)
LFLAGS = -lzmq `pkg-config --cflags --libs protobuf`

cgist: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cgist
console_gist.o: console_gist.cpp
	$(CC) $(CFLAGS) console_gist.cpp -o console_gist.o
message.pb.o: message.pb.cpp message.pb.hh
	g++ -c message.pb.cpp -o message.pb.o
clean:
	rm -f *.o cgist message.pb.cc