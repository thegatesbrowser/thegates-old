#ifndef COMMAND_H
#define COMMAND_H

#include "core/object/ref_counted.h"
#include "core/variant/array.h"

class Command : public RefCounted {
	GDCLASS(Command, RefCounted);

	String name;
	Array args;

protected:
	static void _bind_methods();

public:
	void set_name(const String &p_name) { name = p_name; };
	String get_name() { return name; };

	void set_args(const Array &p_args) { args = p_args; };
	Array get_args() { return args; };

	Command();
	~Command();
};

#endif // COMMAND_H
