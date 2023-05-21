#ifndef PROJECT_SETTINGS_EXPOSED_H
#define PROJECT_SETTINGS_EXPOSED_H

#include "core/object/ref_counted.h"

class ProjectSettingsExposed : public RefCounted {
    GDCLASS(ProjectSettingsExposed, RefCounted);

protected:
    static void _bind_methods();

public:
    static void add_autoload(const StringName &p_name, const String &p_path, bool p_is_singleton);
    static void remove_autoload(const StringName &p_name);
    static PackedStringArray get_autoload_list();
};

#endif // PROJECT_SETTINGS_EXPOSED_H
