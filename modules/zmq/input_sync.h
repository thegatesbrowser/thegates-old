#ifndef INPUT_SYNC_H
#define INPUT_SYNC_H

#include "core/object/ref_counted.h"
#include "core/input/input.h"
#include "thirdparty/zmqpp/zmqpp.hpp"
#include "thirdparty/zmqpp/context.hpp"

inline zmqpp::context ctx;
static const String IPC_ADDR("ipc://tg-main");

class InputSync : public RefCounted {
	GDCLASS(InputSync, RefCounted);

	zmqpp::socket sock;

protected:
	static void _bind_methods();

public:
	void bind(const String &addr = IPC_ADDR);
	void connect(const String &addr = IPC_ADDR);
	void send_input_event(const Ref<InputEvent> &event);
	void receive_input_events();

	InputSync();
	~InputSync();
};

#endif // INPUT_SYNC_H