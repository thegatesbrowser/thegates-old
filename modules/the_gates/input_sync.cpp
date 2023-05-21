
#include "input_sync.h"
#include "socket.hpp"
#include "zmq_context.h"
#include "core/input/input.h"
#include "core/core_string_names.h"
#include "core/variant/variant.h"
#include "core/variant/variant_parser.h"

static inline String var_to_str(const Variant &p_var) {
	String vars;
	VariantWriter::write_to_string(p_var, vars);
	return vars;
}

static inline Variant str_to_var(const String &p_var) {
	VariantParser::StreamString ss;
	ss.s = p_var;

	String errs;
	int line;
	Variant ret;
	(void)VariantParser::parse(&ss, ret, errs, line);

	return ret;
}

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
