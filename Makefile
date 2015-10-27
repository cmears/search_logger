OBJS = search_logger.o message.pb.o
CC = clang++
DEBUG = -g
CFLAGS = -std=c++0x -W -Wall -c $(DEBUG)
LFLAGS = -lzmq `pkg-config --cflags --libs protobuf`

search_logger: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o search_logger
search_logger.o: search_logger.cpp
	$(CC) $(CFLAGS) search_logger.cpp -o search_logger.o
message.pb.o: message.pb.cpp message.pb.hh
	g++ -c message.pb.cpp -o message.pb.o
clean:
	rm -f *.o search_logger message.pb.cc