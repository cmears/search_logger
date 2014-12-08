#include <zmq.hpp>
#include <string>
#include <iostream>
#include <iomanip>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

using std::setw;

enum MsgType {
  NODE_DATA = 1,
  DONE_SENDING = 2,
  START_SENDING = 3
};

struct Message {
  static const int LABEL_SIZE = 16;

  MsgType type;
  int sid;
  int parent;
  int alt;
  int kids;
  int status;
  int restart_id;
  unsigned long long time;
  char thread;
  char label[LABEL_SIZE];
};

int main() {

	zmq::context_t context(1);
	zmq::socket_t receiver (context, ZMQ_PULL);
	receiver.bind("tcp://*:6565");
  std::cout << "listening to port: 6565\n";
	int total = 0;

	while (true) {
		zmq::message_t msg;

		receiver.recv(&msg);
		Message *tr = reinterpret_cast<Message*>(msg.data());
		switch (tr->type) {
			case NODE_DATA:
				std::cout << std::left << "Node: " << setw(8) << tr->sid << " " << setw(8) << tr->parent
                  << " " << setw(2) << tr->alt << " " << tr->kids << " " << tr->status
                  << "  thread: " << setw(2) << (int)tr->thread 
                  << "  restart: " << setw(2) << static_cast<int>(tr->restart_id)
                  << "  time: " << setw(9) << tr->time
                  << "  label: " << tr->label << std::endl;
			break;
			case DONE_SENDING:
				std::cout << "Done receiving\n";
			break;
      case START_SENDING:
        std::cout << "Start recieving, restart: " << tr->restart_id << "\n";
      break;
		}
			
	}
	return 0;
}