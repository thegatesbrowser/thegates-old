#include "core/object/class_db.h"
#include "register_types.h"
#include "zmq_context.h"
#include "external_texture.h"
#include "input_sync.h"
#include "command_sync.h"
#include "command.h"

void initialize_the_gates_module(ModuleInitializationLevel p_level) {
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
	GDREGISTER_CLASS(Command);
	GDREGISTER_CLASS(CommandSync);
    GDREGISTER_CLASS(ExternalTexture);
}

void uninitialize_the_gates_module(ModuleInitializationLevel p_level) {
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
