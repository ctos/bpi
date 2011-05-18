#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <hurd/ports.h>
#include <hurd/hurd_types.h>

#include "cache.h"
int main()
{
	mach_port_t port;

	port = file_name_lookup("/tmp/cache", 0, 0);
	Cache_send_message(port, "hello,world.\n");
	
	string_t message;
	Cache_get_message(port, message);

	printf("%s\n", message);


	return 0;
}
