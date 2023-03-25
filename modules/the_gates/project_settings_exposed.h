#ifndef PROJECT_SETTINGS_EXPOSED
#define PROJECT_SETTINGS_EXPOSED

#include "core/object/ref_counted.h"

class ProjectSettingsExposed : public RefCounted {
    GDCLASS(ProjectSettingsExposed, RefCounted);

protected:
    static void _bind_methods();

public:
    static void add_autoload(const StringName &name, const String &path, const bool &is_singleton);
    static void remove_autoload(const StringName &name);
    static PackedStringArray get_autoload_list();
};

#endif // PROJECT_SETTINGS_EXPOSED
