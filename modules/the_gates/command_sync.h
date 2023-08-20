#ifndef COMMAND_SYNC_H
#define COMMAND_SYNC_H

#include "command.h"
#include "scene/main/node.h"
#include "thirdparty/zmqpp/socket.hpp"

#ifdef _WIN32
static const String COMMAND_SYNC_ADDRESS("ipc://command_sync");
#else
static const String COMMAND_SYNC_ADDRESS("ipc:///tmp/command_sync");
#endif

class CommandSync : public Node {
	GDCLASS(CommandSync, Node);

	static CommandSync *singleton;

	zmqpp::socket sock;
	Callable execute_function;

protected:
	static void _bind_methods();

public:
	void bind(const String &p_address = COMMAND_SYNC_ADDRESS);
	void connect(const String &p_address = COMMAND_SYNC_ADDRESS);
	void send_command(const Ref<Command> &p_command);
	void send_command(const String &p_name);
	void send_command(const String &p_name, const Array &p_args);

	Variant call_execute_function(const Ref<Command> &p_command);
	void set_execute_function(Callable p_execute_function) { execute_function = p_execute_function; }
	Callable get_execute_function() const { return execute_function; }

	void bind_commands();
	void receive_commands();

	CommandSync(zmqpp::socket_type type = zmqpp::socket_type::pair);
	~CommandSync();
};

#endif // COMMAND_SYNC_H
