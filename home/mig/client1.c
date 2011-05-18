#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <hurd.h>
#include <hurd/hurd_types.h>
#include "mig.h"

int main()
{
	int value = 0;
	mach_port_t port;
	
	port = file_name_lookup("/tmp/trans", 0, 0);

	S_getnum(port, &value);
	printf("the data is [%d].\n", value);

	return 0;

}
