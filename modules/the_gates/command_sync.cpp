#include "command_sync.h"
#include "zmq_context.h"

void CommandSync::bind(const String &p_address) {
	sock.bind(p_address.utf8().get_data());
}

void CommandSync::connect(const String &p_address) {
	sock.connect(p_address.utf8().get_data());
}

String CommandSync::call_execute_function(const String &p_command) {
	ERR_FAIL_COND_V_MSG(!execute_function.is_valid(), String(), "CommandSync requires a valid 'execute_function'.");

	Array argv;
	argv.append(p_command);
	Variant ret = execute_function.callv(argv);
	ERR_FAIL_COND_V_MSG(ret.get_type() != Variant::STRING, String(), "The execute function must return a String.");

	return ret;
}

void CommandSync::send_command(const String &p_command) {
	sock.send(p_command.utf8().get_data(), true);
}

void CommandSync::receive_commands() {
	std::string msg;

	while (sock.receive(msg, true)) {
		String command(msg.c_str());
		String res = call_execute_function(command);
	}
}

void CommandSync::_bind_methods() {
	ClassDB::bind_method(D_METHOD("bind", "address"), &CommandSync::bind, DEFVAL(COMMAND_SYNC_ADDRESS));
	ClassDB::bind_method(D_METHOD("receive_commands"), &CommandSync::receive_commands);

	ClassDB::bind_method(D_METHOD("get_execute_function"), &CommandSync::get_execute_function);
	ClassDB::bind_method(D_METHOD("set_execute_function", "execute_function"), &CommandSync::set_execute_function);
	ADD_PROPERTY(PropertyInfo(Variant::CALLABLE, "execute_function", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NO_EDITOR), "set_execute_function", "get_execute_function");
}

CommandSync::CommandSync(zmqpp::socket_type type)
	: sock(zmqpp::socket(ctx, type)) {
}

CommandSync::~CommandSync() {
	sock.close();
}
