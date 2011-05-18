#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <hurd.h>
#include <hurd/hurd_types.h>
#include "mig.h"


int main()
{
	mach_port_t data_server_port;

	data_server_port = file_name_lookup("/tmp/trans", 0, 0);
	printf("data_server_port is [%u]\n", data_server_port);

	S_setnum(data_server_port, 9999);




	return 0;
}
