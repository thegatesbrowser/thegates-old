#ifndef SCRIPT_SERVER_EXPOSED_H
#define SCRIPT_SERVER_EXPOSED_H

#include "core/object/ref_counted.h"

class ScriptServerExposed : public RefCounted {
    GDCLASS(ScriptServerExposed, RefCounted);

protected:
    static void _bind_methods();

public:
    static void add_global_class(const StringName &p_class, const StringName &p_base, const StringName &p_language, const String &p_path);
    static void remove_global_class(const StringName &p_class);
    static PackedStringArray get_global_class_list();
};

#endif // SCRIPT_SERVER_EXPOSED_H
