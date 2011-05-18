#include "test.h"
#include <stdio.h>
#include <mach.h>
//#include <mach/mig_support.h>
//#include <mach/mach_init.h>
int main()
{
	mach_port_t port;
	kern_return_t err;
//	mach_init();
	err = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
	if (err == KERN_SUCCESS)
	{
		mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
		printf("port have been allocated.\n");
		err = send(port, 9999);
		if (err == MACH_MSG_SUCCESS)
		{
			printf("the port is %d.\n", port);
		
		}
		else{
			perror("error: some unexpected error in send.\n");
		
		}

		mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MOVE_RECEIVE);
		int num;
		err = receive(port, &num);	
		if (err == MACH_MSG_SUCCESS)
		{
			printf("the num is %d.\n", num);
		}
	}




	return 0;
}
