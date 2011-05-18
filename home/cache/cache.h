#ifndef	_cache_user_
#define	_cache_user_

/* Module cache */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/std_types.h>
#include <mach/mach_types.h>
#include <device/device_types.h>
#include <device/net_status.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <hurd/hurd_types.h>

/* Routine send_message */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Cache_send_message
#if	defined(LINTLIBRARY)
    (send_port, message)
	mach_port_t send_port;
	string_t message;
{ return Cache_send_message(send_port, message); }
#else
(
	mach_port_t send_port,
	string_t message
);
#endif

/* Routine get_message */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Cache_get_message
#if	defined(LINTLIBRARY)
    (recv_port, message)
	mach_port_t recv_port;
	string_t message;
{ return Cache_get_message(recv_port, message); }
#else
(
	mach_port_t recv_port,
	string_t message
);
#endif

#endif	/* not defined(_cache_user_) */
