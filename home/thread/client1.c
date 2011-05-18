#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <mach.h>
#include <error.h>
#include <hurd.h>
struct message{
	mach_msg_header_t head;
	mach_msg_type_t type;
	int integer;
};
int main()
{
	mach_port_t port;
	kern_return_t err;
	port = file_name_lookup("/tmp/trans1", 0, 0);
	printf("the port is %d.\n", port);


	struct message mess;
	mess.integer = 0;
	mess.head.msgh_size = sizeof (struct message);
	mess.head.msgh_local_port = port;
	mach_msg_receive(&(mess.head));
	printf("THE NUM RECEIVED IS %d.\n", mess.integer);


	return 0;
}
