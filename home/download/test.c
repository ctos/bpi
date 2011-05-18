
#include <stdio.h>
#include <stdlib.h>
#include <hurd/hurd_types.h>
#include <hurd/ports.h>

int main()
{
	mach_port_t port;

	port = file_name_lookup("/tmp/download", 0, 0);
	
	printf("THE PORT IS %d.\n", port);



	return 0;
}
