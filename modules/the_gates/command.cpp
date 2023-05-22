#include "command.h"

void Command::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_name", "name"), &Command::set_name);
	ClassDB::bind_method(D_METHOD("get_name"), &Command::get_name);
	ClassDB::bind_method(D_METHOD("set_args", "args"), &Command::set_args);
	ClassDB::bind_method(D_METHOD("get_args"), &Command::get_args);

    ADD_PROPERTY(PropertyInfo(Variant::STRING, "name"), "set_name", "get_name");
    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "args"), "set_args", "get_args");
}

Command::Command() {
}

Command::~Command() {
}
