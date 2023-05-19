#include "register_types.h"
#include "core/string/print_string.h"
#include "input_sync.h"

void initialize_zmq_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	try
	{
		ctx = zmqpp::context();
		print_line("ZeroMQ initialized");
	}
	catch(const std::exception& e)
	{
		ERR_PRINT("ZeroMQ initialization failure.");
	}

	GDREGISTER_CLASS(InputSync);
}

void uninitialize_zmq_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	try
	{
		ctx.terminate();
	}
	catch(const std::exception& e)
	{
		ERR_PRINT("ZeroMQ termination failure.");
	}
}
