#include "register_types.h"
#include "core/object/class_db.h"
#include "script_server_exposed.h"
#include "project_settings_exposed.h"

void initialize_the_gates_module(ModuleInitializationLevel p_level) {
    ClassDB::register_class<ScriptServerExposed>();
    ClassDB::register_class<ProjectSettingsExposed>();
}

void uninitialize_the_gates_module(ModuleInitializationLevel p_level) {

}
