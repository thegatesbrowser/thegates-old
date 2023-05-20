#include "project_settings_exposed.h"
#include "core/config/project_settings.h"
#include "core/object/script_language.h"
#include "core/io/resource_loader.h"
#include "scene/resources/packed_scene.h"
#include "scene/main/window.h"

void ProjectSettingsExposed::add_autoload(const StringName &name, const String &path, const bool &is_singleton = false) {
    // Add autoload
    ProjectSettings::AutoloadInfo autoload = { name, path, is_singleton };
    ProjectSettings::get_singleton()->add_autoload(autoload);

    // FROM: main.cpp line 2358 --->
	//first pass, add the constants so they exist before any script is loaded
    if (autoload.is_singleton) {
        for (int i = 0; i < ScriptServer::get_language_count(); i++) {
            ScriptServer::get_language(i)->add_global_constant(autoload.name, Variant());
        }
    }

	//second pass, load into global constants
    Ref<Resource> res = ResourceLoader::load(autoload.path);
    ERR_FAIL_COND_MSG(res.is_null(), "Can't autoload: " + autoload.path);
    Node *n = nullptr;
    Ref<PackedScene> scn = res;
    Ref<Script> script_res = res;
    if (scn.is_valid()) {
        n = scn->instantiate();
    } else if (script_res.is_valid()) {
        StringName ibt = script_res->get_instance_base_type();
        bool valid_type = ClassDB::is_parent_class(ibt, "Node");
        ERR_FAIL_COND_MSG(!valid_type, "Script does not inherit from Node: " + autoload.path);

        Object *obj = ClassDB::instantiate(ibt);

        ERR_FAIL_COND_MSG(!obj, "Cannot instance script for autoload, expected 'Node' inheritance, got: " + String(ibt) + ".");

        n = Object::cast_to<Node>(obj);
        n->set_script(script_res);
    }

    ERR_FAIL_COND_MSG(!n, "Path in autoload not a node or script: " + autoload.path);
    n->set_name(autoload.name);

    if (autoload.is_singleton) {
        for (int i = 0; i < ScriptServer::get_language_count(); i++) {
            ScriptServer::get_language(i)->add_global_constant(autoload.name, n);
        }
    }

    // Add to scene root
    SceneTree *sml = Object::cast_to<SceneTree>(OS::get_singleton()->get_main_loop());
    sml->get_root()->add_child(n);
}

void ProjectSettingsExposed::remove_autoload(const StringName &name) {
	ERR_FAIL_COND_MSG(!ProjectSettings::get_singleton()->has_autoload(name), "Trying to remove non-existent autoload.");
    ProjectSettings::AutoloadInfo autoload = ProjectSettings::get_singleton()->get_autoload(name);

	// Remove from global constants
    if (autoload.is_singleton) {
        for (int i = 0; i < ScriptServer::get_language_count(); i++) {
            ScriptServer::get_language(i)->remove_global_constant(autoload.name);
        }
    }

    // Remove from scene root
    SceneTree *sml = Object::cast_to<SceneTree>(OS::get_singleton()->get_main_loop());
    Node *n = sml->get_root()->find_child(autoload.name, false, false);

    ERR_FAIL_COND_MSG(!n, "Autoload with name " + autoload.name + " not found in SceneTree.");
    sml->get_root()->remove_child(n);

    // Remove autoload
    ProjectSettings::get_singleton()->remove_autoload(name);
}

PackedStringArray ProjectSettingsExposed::get_autoload_list() {
	HashMap<StringName, ProjectSettings::AutoloadInfo> autoloads = ProjectSettings::get_singleton()->get_autoload_list();

    PackedStringArray result;
    for (const KeyValue<StringName, ProjectSettings::AutoloadInfo> &e : autoloads) {
        result.push_back(e.key);
    }
    return result;
}

void ProjectSettingsExposed::_bind_methods() {
    ClassDB::bind_static_method("ProjectSettingsExposed", D_METHOD("add_autoload", "name", "path", "is_singleton"), &ProjectSettingsExposed::add_autoload, DEFVAL(false));
    ClassDB::bind_static_method("ProjectSettingsExposed", D_METHOD("remove_autoload", "name"), &ProjectSettingsExposed::remove_autoload);
    ClassDB::bind_static_method("ProjectSettingsExposed", D_METHOD("get_autoload_list"), &ProjectSettingsExposed::get_autoload_list);
}
