#include <stdio.h>
#include <mach.h>
#include <error.h>
#include "common.h"
int main()
{
	mach_port_t port;
	kern_return_t err;
	err = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
	if (err != KERN_SUCCESS)
	{
		return;
	}
	printf("the port is %d.\n", port);

	struct message mess;
	
	mess.head.msgh_size = sizeof (struct message);
	mess.head.msgh_local_port = port;
	mach_msg_receive(&(mess.head));
	printf("THE NUM RECEIVED IS %d.\n", mess.integer);


	return 0;
}
