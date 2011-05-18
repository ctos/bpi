#ifndef	_cookie_user_
#define	_cookie_user_

/* Module cookie */

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
kern_return_t Cookie_send_message
#if	defined(LINTLIBRARY)
    (port, send_message)
	mach_port_t port;
	string_t send_message;
{ return Cookie_send_message(port, send_message); }
#else
(
	mach_port_t port,
	string_t send_message
);
#endif

/* Routine get_message */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Cookie_get_message
#if	defined(LINTLIBRARY)
    (port, recv_message)
	mach_port_t port;
	string_t recv_message;
{ return Cookie_get_message(port, recv_message); }
#else
(
	mach_port_t port,
	string_t recv_message
);
#endif

#endif	/* not defined(_cookie_user_) */
