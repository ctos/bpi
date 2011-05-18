#include <mach.h>
#include <stdio.h>
#include <error.h>
#include <mach/mach_interface.h>
#include <mach_init.h>
#include <mach/task_special_ports.h>
#include "common.h"
void send(mach_port_t dest, int data)
{
	kern_return_t err;
	struct message mess;


	mess.head.msgh_bits = MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_MAKE_SEND);
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
	printf("THE MACH_TASK_SELF IS %d.\n", mach_task_self());
	err = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
	if (err != KERN_SUCCESS)
	{
		perror("ERROR:COULD NOT ALLOCATE THE PORT.\n");
		return ;
	}
	printf("THE REGISTER PORT IS %d.\n", port);
	mach_ports_register(mach_task_self(), &port, 1);
	send(port, 9999);

	mach_port_t *recvport;
	int recv_num;
	task_get_kernel_port(mach_task_self(),&port);
	
	printf("SPECIAL KERNAL PORT IS %d.\n",port);
	mach_ports_lookup(mach_task_self(), &recvport, &recv_num);
	int i;
	for (i = 0; i < recv_num; i++)
	{
		printf("THE RECV PORT IS %d.AND THE NUM IS %d.\n", *(recvport + i), recv_num);
	}

	return 0;
}
