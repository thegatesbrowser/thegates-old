#ifndef INPUT_SYNC_H
#define INPUT_SYNC_H

#include "core/object/ref_counted.h"
#include "core/input/input.h"
#include "thirdparty/zmqpp/zmqpp.hpp"
#include "thirdparty/zmqpp/context.hpp"

static const String IPC_ADDRESS("ipc://input_sync");
inline zmqpp::context ctx;

class InputSync : public RefCounted {
	GDCLASS(InputSync, RefCounted);

	zmqpp::socket sock;

protected:
	static void _bind_methods();

public:
	void bind(const String &p_address = IPC_ADDRESS);
	void connect(const String &p_address = IPC_ADDRESS);
	void send_input_event(const Ref<InputEvent> &p_event);
	void receive_input_events();

	InputSync();
	~InputSync();
};

#endif // INPUT_SYNC_H
