#ifndef	_mig_user_
#define	_mig_user_

/* Module mig */

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

/* Routine getnum */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t S_getnum
#if	defined(LINTLIBRARY)
    (data_port, num)
	mach_port_t data_port;
	int *num;
{ return S_getnum(data_port, num); }
#else
(
	mach_port_t data_port,
	int *num
);
#endif

/* Routine setnum */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t S_setnum
#if	defined(LINTLIBRARY)
    (data_port, num)
	mach_port_t data_port;
	int num;
{ return S_setnum(data_port, num); }
#else
(
	mach_port_t data_port,
	int num
);
#endif

#endif	/* not defined(_mig_user_) */
