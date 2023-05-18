
#include "input_sync.h"
#include <socket.hpp>

void InputSync::_bind_methods() {
    ClassDB::bind_method(D_METHOD("bind", "address"), &InputSync::bind, DEFVAL(IPC_ADDR));
}

void InputSync::bind(const String &address) {
	sock.bind(address.utf8().get_data());
    sock.send("Hello from server!");

    std::string msg;
    sock.receive(msg);
    print_line("Received: " + String(msg.c_str()));
}

void InputSync::connect(const String &address) {
    sock.connect(address.utf8().get_data());
    sock.send("Hello from client!");

    std::string msg;
    sock.receive(msg);
    print_line("Received: " + String(msg.c_str()));
}

InputSync::InputSync()
    : sock((zmqpp::socket&&)zmqpp::socket(ctx, zmqpp::socket_type::pair)) {
}

InputSync::~InputSync() {
    sock.close();
}