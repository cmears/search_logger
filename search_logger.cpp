#include <string>
#include <iostream>
#include <cstdio>
#include <iomanip>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

// #include "zmq.hpp"
#include "message.pb.hh"

#include <nanomsg/nn.h>
#include <nanomsg/tcp.h>
#include <nanomsg/pipeline.h>

int main() {

  using std::setw;

  int nanosocket = nn_socket(AF_SP, NN_PULL);
  
	// zmq::context_t context(1);
	// zmq::socket_t receiver (context, ZMQ_PULL);
	// receiver.bind("tcp://*:6565");

  nn_bind(nanosocket, "tcp://*:6565");
  
  std::cout << "listening to port: 6565\n";

	while (true) {
	
    // zmq::message_t msg;
  
    //     	receiver.recv(&msg);

            void *buf = NULL;
            int msg_length = nn_recv(nanosocket, &buf, NN_MSG, 0);
            

                //usleep(10000);

    message::Node node;
    node.ParseFromArray(buf, msg_length);

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

      nn_freemsg(buf);

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
