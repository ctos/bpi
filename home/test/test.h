#ifndef	_test_user_
#define	_test_user_

/* Module test */

#include <mach/kern_return.h>
#include <mach/port.h>
#include <mach/message.h>

#include <mach/std_types.h>

/* SimpleRoutine send */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t send
#if	defined(LINTLIBRARY)
    (sendPort, num)
	mach_port_t sendPort;
	int num;
{ return send(sendPort, num); }
#else
(
	mach_port_t sendPort,
	int num
);
#endif

/* Routine receive */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t receive
#if	defined(LINTLIBRARY)
    (recvPort, num)
	mach_port_t recvPort;
	int *num;
{ return receive(recvPort, num); }
#else
(
	mach_port_t recvPort,
	int *num
);
#endif

#endif	/* not defined(_test_user_) */
