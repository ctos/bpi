#include <mach.h>
#include <stdio.h>
#include <mach/mach_interface.h>

int main()
{
	mach_port_t *port;
	int num;
	mach_ports_lookup(1, &port, &num);
	printf("THE LOOKUP PORT IS %d.\n", port[0]);
	printf("THE LOOKUP PORT IS %d.\n", port[1]);
	printf("THE LOOKUP PORT IS %d.\n", port[2]);
	printf("THE LOOKUP PORT IS %d.\n", port[3]);
	printf("THE LOOKUP PORT IS %d.\n", port[4]);
	printf("THE LOOKUP PORT IS %d.\n", port[5]);

	printf("THE PORT NUM IS %d.\n", num);

}
