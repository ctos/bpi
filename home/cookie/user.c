#include "cookie.h"
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>
#define STRINGLENGTH 1024

int main()
{
	mach_port_t port;

	char send_path[STRINGLENGTH];

	/*fscanf("/home/cookie/userconfig", "%s", send_path);
	*/
	port = file_name_lookup("/tmp/cookie", 0, 0);
	
	Cookie_send_message(port, "hello,world\n");

	string_t recv_message;
	Cookie_get_message(port, recv_message);

	printf("%s\n",recv_message);
	return 0;
}






