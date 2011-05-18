#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <mach.h>
#include <stdio.h>
#include <hurd.h>
#include <error.h>
#include <mach/mach_interface.h>
#include <mach_init.h>
#include <mach/task_special_ports.h>
struct message{
	mach_msg_header_t head;
	mach_msg_type_t type;
	int integer;

};
void send(mach_port_t dest, int data)
{
	kern_return_t err;
	struct message mess;


	mess.head.msgh_bits = MACH_MSGH_BITS(19, 0);
	mess.head.msgh_size = sizeof (struct message);
	mess.head.msgh_local_port = MACH_PORT_NULL;
	mess.head.msgh_remote_port = dest;

	mess.type.msgt_name = MACH_MSG_TYPE_INTEGER_32;
	mess.type.msgt_size = 32;
	mess.type.msgt_number = 1;
	mess.type.msgt_inline = TRUE;
	mess.type.msgt_longform = FALSE;
	mess.type.msgt_deallocate = FALSE;

	mess.integer = data;

	err = mach_msg_send(&(mess.head));
	if (err == MACH_MSG_SUCCESS)
	{
		printf("SUCCESS: THE MESSAGE WAS SENT.\n");
	}
}

void receive(mach_port_t source, int * ip)
{
	struct message mess;
	kern_return_t err;
	err = mach_msg_receive(&(mess.head));
	if (err == MACH_MSG_SUCCESS)
	{
		printf("HAVE BEEN RECEIVE SUCCEESSFUL.\n");
	}

	*ip = mess.integer;
	return;

}
int main()
{
	mach_port_t port;
	kern_return_t err;

	port = file_name_lookup("/tmp/trans1", 0, 0);

	printf("the port is %d.\n", port);
	send(port, 9999);

	return 0;
}
