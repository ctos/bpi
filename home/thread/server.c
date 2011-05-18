#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <hurd/ports.h>
#include <hurd/hurd_types.h>
#include <hurd/trivfs.h>
#include <stdlib.h>


int trivfs_fstype = FSTYPE_MISC;
int trivfs_fsid = 0;
int trivfs_support_read = 0;
int trivfs_support_write = 0;
int trivfs_support_exec = 0;
int trivfs_allow_open = 0x00;
int trivfs_proid_nportclasses = 0;
int trivfs_cntl_nportclasses = 0;

void trivfs_modify_stat(struct trivfs_protid *cred, io_statbuf_t *st)
{}
error_t trivfs_goaway(struct trivfs_control *fsys, int flags)
{
	exit(0);
}
int main()
{
	int err;
	mach_port_t bootstrap;
	struct trivfs_control *fsys;
	task_get_bootstrap_port(mach_task_self(), &bootstrap);
	if (bootstrap == MACH_PORT_NULL)
	{
		printf("NOT A TRANSLATOR.\n");
		exit(1);
	}
	err = trivfs_startup(bootstrap, 0, 0, 0, 0, 0, &fsys);
	if(err)
	{
		printf("cannot start up.\n");
		return 0;
	}


	ports_manage_port_operations_one_thread(fsys-> pi.bucket, trivfs_demuxer, 0);

	return 0;



}
