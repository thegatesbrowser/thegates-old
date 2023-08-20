#ifndef SANDBOXING
#define SANDBOXING

#include "core/object/ref_counted.h"

class Sandboxing : public RefCounted {
	GDCLASS(Sandboxing, RefCounted);

protected:
	static void _bind_methods();

public:
	static Error sandbox();

	Sandboxing();
	~Sandboxing();
};

#endif // SANDBOXING
