#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <hurd/hurd_types.h>
#include <hurd/ports.h>

#include "download.h"

int main()
{
	mach_port_t port;

	port = file_name_lookup("/tmp/download", 0, 0);

	string_t a;
	Download_create_request(port, "list.image.baidu.com/t/image_category/image_funny.html", a);
	Download_send_request(port, a);
	return 0;
}
