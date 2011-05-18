#ifndef	_download_user_
#define	_download_user_

/* Module download */

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
#include "download_types.h"

/* Routine send_message */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Download_send_message
#if	defined(LINTLIBRARY)
    (port, message)
	mach_port_t port;
	string_t message;
{ return Download_send_message(port, message); }
#else
(
	mach_port_t port,
	string_t message
);
#endif

/* Routine get_message */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Download_get_message
#if	defined(LINTLIBRARY)
    (port, message)
	mach_port_t port;
	string_t message;
{ return Download_get_message(port, message); }
#else
(
	mach_port_t port,
	string_t message
);
#endif

/* Routine create_request */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Download_create_request
#if	defined(LINTLIBRARY)
    (port, host, response)
	mach_port_t port;
	string_t host;
	bpi_response *response;
{ return Download_create_request(port, host, response); }
#else
(
	mach_port_t port,
	string_t host,
	bpi_response *response
);
#endif

/* Routine send_request */
#ifdef	mig_external
mig_external
#else
extern
#endif
kern_return_t Download_send_request
#if	defined(LINTLIBRARY)
    (port, request)
	mach_port_t port;
	string_t request;
{ return Download_send_request(port, request); }
#else
(
	mach_port_t port,
	string_t request
);
#endif

#endif	/* not defined(_download_user_) */
