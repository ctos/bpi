#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <stdio.h>

#include <stdlib.h>
#include "cache.h"
#include <hurd/ports.h>
#include <hurd/hurd_types.h>
#include <hurd/trivfs.h>
#include <error.h>
#include <sys/stat.h>

extern boolean_t Cache_cache_server(mach_msg_header_t *in, mach_msg_header_t *out);

int trivfs_fstype		= FSTYPE_MISC;
int trivfs_fsid 		= 0;
int trivfs_support_read		= 0;
int trivfs_support_write	= 0;
int trivfs_support_exec		= 0;
int trivfs_allow_open		= 0x00;
int trivfs_protid_nportclasses	= 0;
int trivfs_cntl_nportclasses	= 0;
string_t message;

int demuxer(mach_msg_header_t * in, mach_msg_header_t *out)
{
	return (Cache_cache_server(in, out) || trivfs_demuxer(in, out));
}
void trivfs_modify_stat (struct trivfs_protid * cred, io_statbuf_t *st)
{
}
error_t trivfs_goaway(struct trivfs_control *fsys, int flags)
{
	exit(0);
}

kern_return_t Cache_send_message(mach_port_t port, string_t send_message)
{
	strcpy(message, send_message);
	return KERN_SUCCESS;
}
kern_return_t Cache_get_message(mach_port_t port, string_t recv_message)
{
	strcpy(recv_message, message);
	return KERN_SUCCESS;
}

int main()
{
	int err;
	mach_port_t bootstrap;
	struct trivfs_control *fsys;

	task_get_bootstrap_port(mach_task_self(), &bootstrap);
	if (bootstrap == MACH_PORT_NULL)
	{
		error(2, 0, "MUST BE STARTED AS A TRANSLATOR");
	}

	err = trivfs_startup(bootstrap, 0, 0, 0, 0, 0, &fsys);

	if (err)
	{
		return 0;
	}

	ports_manage_port_operations_one_thread(fsys -> pi.bucket, demuxer, 0);
	return 0;


	return 0;
}
