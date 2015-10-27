#include <string>
#include <iostream>
#include <cstdio>
#include <iomanip>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "zmq.hpp"
#include "message.pb.hh"

int main() {

  using std::setw;

	zmq::context_t context(1);
	zmq::socket_t receiver (context, ZMQ_PULL);
	receiver.bind("tcp://*:6565");
  std::cout << "listening to port: 6565\n";

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
          // << "  time: "    << setw(9) << node.time()
          << "  label: "   << setw(14) << node.label();
      if (node.has_domain_size() && node.domain_size() > 0) {
          std::cout << "  domain: "  << setw(6) << std::setprecision(4) << node.domain_size();
      }
      if (node.has_nogood() && node.nogood().length() > 0) {
          std::cout << "  nogood: "  << node.nogood();
      }
      if (node.has_info() && node.info().length() > 0) {
          std::cout << "info:\n"    << node.info() << std::endl;
      }

      std::cout << std::endl;

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