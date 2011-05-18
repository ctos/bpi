#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include "cookie.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <hurd/ports.h>
#include <hurd/trivfs.h>
#include <hurd/hurd_types.h>
#include <sys/stat.h>
extern boolean_t Cookie_cookie_server
(mach_msg_header_t *in, mach_msg_header_t *out);

int trivfs_fstype	 = FSTYPE_MISC;
int trivfs_fsid		 = 0;
int trivfs_support_read  = 0;
int trivfs_support_write = 0;
int trivfs_support_exec  = 0;
int trivfs_allow_open  	 = 0x00;
int trivfs_protid_nportclasses = 0;
int trivfs_cntl_nportclasses = 0;

string_t message;


int demuxer(mach_msg_header_t *in, mach_msg_header_t *out)
{
	return (Cookie_cookie_server(in, out) || trivfs_demuxer(in, out));
}
void trivfs_modify_stat(struct trivfs_protid *cred, io_statbuf_t *st)
{
}
error_t trivfs_goaway(struct trivfs_control *fsys, int flags)
{
	exit(0);
}
kern_return_t Cookie_send_message(mach_port_t port, string_t recv_message)
{
	strcpy(message, recv_message);

	return KERN_SUCCESS;
}
kern_return_t Cookie_get_message(mach_port_t port, string_t send_message)
{
	strcpy(send_message, message);

	return KERN_SUCCESS;

}


int main()
{
	kern_return_t err;
	mach_port_t bootstrap;
	struct trivfs_control *fsys;

	task_get_bootstrap_port(mach_task_self(), &bootstrap);

	err = trivfs_startup(bootstrap, 0, 0, 0, 0, 0, &fsys);
	mach_port_deallocate(mach_task_self(), bootstrap);

	if (err != KERN_SUCCESS)
	{
		printf("something wrong when trivfs_startup.\n");
		return 0;
	}

	ports_manage_port_operations_one_thread(fsys -> pi.bucket, demuxer, 0);









	return 0;
}
