#include <mach.h>

struct message{
	mach_msg_header_t head;
	mach_msg_type_t type;

	int integer;
};

