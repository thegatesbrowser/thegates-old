#ifndef ZMQ_REGISTER_TYPES_H
#define ZMQ_REGISTER_TYPES_H

#include "modules/register_module_types.h"

void initialize_zmq_module(ModuleInitializationLevel p_level);
void uninitialize_zmq_module(ModuleInitializationLevel p_level);

#endif // ZMQ_REGISTER_TYPES_H
