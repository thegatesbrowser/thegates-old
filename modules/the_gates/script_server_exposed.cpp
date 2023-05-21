#include "script_server_exposed.h"
#include "core/object/script_language.h"

void ScriptServerExposed::add_global_class(const StringName &p_class, const StringName &p_base, const StringName &p_language, const String &p_path) {
    ScriptServer::add_global_class(p_class, p_base, p_language, p_path);
}

void ScriptServerExposed::remove_global_class(const StringName &p_class) {
    ScriptServer::remove_global_class(p_class);
}

PackedStringArray ScriptServerExposed::get_global_class_list() {
	List<StringName> classes;
    ScriptServer::get_global_class_list(&classes);

    PackedStringArray result;
    for (const StringName &e : classes) {
        result.push_back(e);
    }
    return result;
}

void ScriptServerExposed::_bind_methods() {
    ClassDB::bind_static_method("ScriptServerExposed", D_METHOD("add_global_class", "class", "base", "language", "path"), &ScriptServerExposed::add_global_class);
    ClassDB::bind_static_method("ScriptServerExposed", D_METHOD("remove_global_class", "class"), &ScriptServerExposed::remove_global_class);
    ClassDB::bind_static_method("ScriptServerExposed", D_METHOD("get_global_class_list"), &ScriptServerExposed::get_global_class_list);
}
