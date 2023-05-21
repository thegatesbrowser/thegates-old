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
    enum SocketType {
        SOCKET_TYPE_REQUEST = (int)zmqpp::socket_type::request,
        SOCKET_TYPE_REPLAY = (int)zmqpp::socket_type::reply
    };

	void bind(const String &p_address = COMMAND_SYNC_ADDRESS);
	void connect(const String &p_address = COMMAND_SYNC_ADDRESS);

    String call_execute_function(const String &p_command);
	void set_execute_function(Callable p_execute_function) { execute_function = p_execute_function; }
	Callable get_execute_function() const { return execute_function; }

	CommandSync(SocketType type = SOCKET_TYPE_REPLAY);
	~CommandSync();
};

// VARIANT_ENUM_CAST(CommandSync::SocketType)

#endif // COMMAND_SYNC_H
