#include "command_sync.h"
#include "zmq_context.h"
#include "variant_tools.h"
#include "core/input/input.h"

CommandSync *CommandSync::singleton = nullptr;

void CommandSync::bind(const String &p_address) {
	sock.bind(p_address.utf8().get_data());
}

void CommandSync::connect(const String &p_address) {
	sock.connect(p_address.utf8().get_data());
}

void CommandSync::send_command(const Ref<Command> &p_command) {
	sock.send(var_to_str(p_command).utf8().get_data(), true);
}

void CommandSync::send_command(const String &p_name) {
	Command *command = memnew(Command);
	command->set_name(p_name);
	send_command(Ref<Command>(command));
}

void CommandSync::send_command(const String &p_name, const Array &p_args) {
	Command *command = memnew(Command);
	command->set_name(p_name);
	command->set_args(p_args);
	send_command(Ref<Command>(command));
}

Variant CommandSync::call_execute_function(const Ref<Command> &p_command) {
	ERR_FAIL_COND_V_MSG(!execute_function.is_valid(), Variant(), "CommandSync requires a valid 'execute_function'");

	Array argv;
	argv.append(p_command);
	Variant ret = execute_function.callv(argv);
	ERR_FAIL_COND_V_MSG(ret.get_type() == Variant::NIL, Variant(), "Failed to execute function");

	return ret;
}

void CommandSync::receive_commands() {
	std::string msg;

	while (sock.receive(msg, true)) {
		Variant res = call_execute_function((Ref<Command>)str_to_var(msg.c_str()));
	}
}

void CommandSync::bind_commands() {
	auto _input_set_mouse_mode = [] (Input::MouseMode p_mode) {
		print_line("_input_set_mouse_mode " + itos((int)p_mode));
		DisplayServer::_input_set_mouse_mode(p_mode);

		Array args;
		args.append(p_mode);
		singleton->send_command("set_mouse_mode", args);
	};
	Input::set_mouse_mode_func = _input_set_mouse_mode;

	auto _scene_tree_open_gate = [] (String p_url) {
		print_line("_scene_tree_open_gate " + p_url);

		Array args;
		args.append(p_url);
		singleton->send_command("open_gate", args);
	};
	SceneTree::open_gate_func = _scene_tree_open_gate;
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
	if (singleton == nullptr) {
		singleton = this;
	}
}

CommandSync::~CommandSync() {
	sock.close();
}
