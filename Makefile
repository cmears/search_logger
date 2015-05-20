OBJS = console_gist.o message.pb.o
CC = clang++
DEBUG = -g
CFLAGS = -std=c++0x -W -Wall -c $(DEBUG)
LFLAGS = -lzmq `pkg-config --cflags --libs protobuf`

cgist: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o cgist
console_gist.o: console_gist.cpp
	$(CC) $(CFLAGS) console_gist.cpp -o console_gist.o
message.pb.o: message.pb.cc message.pb.h
	g++ -c message.pb.cc -o message.pb.o
message.pb.cc: message.proto
	protoc -I=. --cpp_out=. message.proto

clean:
	rm -f *.o cgist message.pb.cc