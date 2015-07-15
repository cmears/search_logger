#include <zmq.hpp>
#include <string>
#include <iostream>
#include <cstdio>
#include <iomanip>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "message.pb.hh"

int main() {

  using std::setw;

	zmq::context_t context(1);
	zmq::socket_t receiver (context, ZMQ_PULL);
	receiver.bind("tcp://*:6565");
  std::cout << "listening to port: 6565\n";
	int total = 0;

  char raw_data[80] = {0};

	while (true) {
	
    zmq::message_t msg;
  
		receiver.recv(&msg);

    message::Node node;
    node.ParseFromArray(msg.data(), msg.size());

    if (node.type() == message::Node::NODE) {
      std::cout << std::left
          << "Node: "      << setw(8) << node.sid() << " " << setw(8) << node.pid()
          << " " << setw(2) << node.alt() << " " << node.kids() << " " << node.status()
          << "  thread: "  << setw(2) << node.thread_id()
          << "  restart: " << setw(2) << node.restart_id()
          << "  time: "    << setw(9) << node.time()
          << "  domain: "  << setw(6) << std::setprecision(4) << node.domain_size()
          << "  label: "   << setw(14) << node.label()
          << "  nogood: "  << node.nogood() << std::endl;

      if (node.status() == 0) { /// solution!
        std::cout << "-----solution-----\n";
        std::cout << node.solution();
        std::cout << "------------------\n";


      }

      continue;
    }

    if (node.type() == message::Node::DONE) {
      std::cout << "Done receiving\n";
      continue;
    }

    if (node.type() == message::Node::START) {
      std::cout << "Start recieving, restart: " << node.restart_id() << " name: " << node.label() << " \n";
      continue;
    }
			
	}
	return 0;
}