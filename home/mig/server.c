#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/stat.h>

#include <hurd/ports.h>
#include <hurd/hurd_types.h>
#include <hurd/trivfs.h>

#include "mig.h"

extern boolean_t S_mig_server
(mach_msg_header_t * in, mach_msg_header_t * out);

int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;
int trivfs_support_read = 0;
int trivfs_support_write = 0;
int trivfs_support_exec = 0;
int trivfs_allow_open = 0x00;
int trivfs_proid_nportclasses = 0;
int trivfs_cntl_nportclasses = 0;

int data;


int demuxer(mach_msg_header_t * in, mach_msg_header_t *out)
{
	return (S_mig_server(in, out) || trivfs_demuxer(in, out));
}

void trivfs_modify_stat(struct trivfs_protid * cred, io_statbuf_t *st)
{}
error_t trivfs_goaway(struct trivfs_control *fsys, int flags)
{
	exit(0);
}
kern_return_t S_setnum(mach_port_t in, int value)
{
	data = value;
	return 0;
}
kern_return_t S_getnum(mach_port_t in, int *value)
{
	*value = data;
	return 0;
}
int main()
{
	kern_return_t err;
	mach_port_t bootstrap;
	struct trivfs_control *fsys;
	
	task_get_bootstrap_port(mach_task_self(), &bootstrap);
	
	if (bootstrap == MACH_PORT_NULL)
	{
		printf("bootstrap.\n");
	}

	
	err = trivfs_startup(bootstrap, 0, 0, 0, 0, 0, &fsys);

	if (err)
	{
		printf("the trivfs_startup.\n");
	}

	ports_manage_port_operations_one_thread(fsys -> pi.bucket, demuxer, 0);



	return 0;
}



