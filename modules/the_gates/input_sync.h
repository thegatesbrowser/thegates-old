#ifndef INPUT_SYNC_H
#define INPUT_SYNC_H

#include "core/object/ref_counted.h"
#include "core/input/input.h"
#include "thirdparty/zmqpp/socket.hpp"

#ifdef _WIN32
static const String INPUT_SYNC_ADDRESS("ipc://input_sync");
#else
static const String INPUT_SYNC_ADDRESS("ipc:///tmp/input_sync");
#endif

class InputSync : public RefCounted {
	GDCLASS(InputSync, RefCounted);

	zmqpp::socket sock;

protected:
	static void _bind_methods();

public:
	void bind(const String &p_address = INPUT_SYNC_ADDRESS);
	void connect(const String &p_address = INPUT_SYNC_ADDRESS);
	void send_input_event(const Ref<InputEvent> &p_event);
	void receive_input_events();

	InputSync();
	~InputSync();
};

#endif // INPUT_SYNC_H
