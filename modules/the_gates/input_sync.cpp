
#include "input_sync.h"
#include "socket.hpp"
#include "zmq_context.h"
#include "variant_tools.h"
#include "core/input/input.h"
#include "core/core_string_names.h"

void InputSync::bind(const String &p_address) {
	sock.bind(p_address.utf8().get_data());
}

void InputSync::connect(const String &p_address) {
	sock.connect(p_address.utf8().get_data());
}

void InputSync::send_input_event(const Ref<InputEvent> &p_event) {
	sock.send(var_to_str(p_event).utf8().get_data(), true);
}

void InputSync::receive_input_events() {
	std::string msg;

	while (sock.receive(msg, true)) {
		Input::get_singleton()->parse_input_event((Ref<InputEvent>)str_to_var(msg.c_str()));
	}
}

void InputSync::_bind_methods() {
	ClassDB::bind_method(D_METHOD("bind", "address"), &InputSync::bind, DEFVAL(INPUT_SYNC_ADDRESS));
	ClassDB::bind_method(D_METHOD("send_input_event", "event"), &InputSync::send_input_event);
}

InputSync::InputSync()
	: sock(zmqpp::socket(ctx, zmqpp::socket_type::pair)) {
}

InputSync::~InputSync() {
	sock.close();
}
