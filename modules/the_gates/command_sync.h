#ifndef COMMAND_SYNC_H
#define COMMAND_SYNC_H

#include "scene/main/node.h"
#include "thirdparty/zmqpp/socket.hpp"

static const String COMMAND_SYNC_ADDRESS("ipc://command_sync");

class CommandSync : public Node {
	GDCLASS(CommandSync, Node);

    zmqpp::socket sock;
	Callable execute_function;

protected:
	static void _bind_methods();

public:
	void bind(const String &p_address = COMMAND_SYNC_ADDRESS);
	void connect(const String &p_address = COMMAND_SYNC_ADDRESS);
    void send_command(const String &p_command);
    void receive_commands();

    String call_execute_function(const String &p_command);
	void set_execute_function(Callable p_execute_function) { execute_function = p_execute_function; }
	Callable get_execute_function() const { return execute_function; }

	CommandSync(zmqpp::socket_type type = zmqpp::socket_type::pair);
	~CommandSync();
};

#endif // COMMAND_SYNC_H
