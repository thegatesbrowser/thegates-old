#include "register_types.h"
#include "core/object/class_db.h"
#include "exposed/script_server_exposed.h"
#include "exposed/project_settings_exposed.h"
#include "zmq/input_sync.h"

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

    GDREGISTER_CLASS(ScriptServerExposed);
    GDREGISTER_CLASS(ProjectSettingsExposed);
	GDREGISTER_CLASS(InputSync);
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
