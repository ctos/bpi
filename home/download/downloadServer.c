/* Module download */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#define EXPORT_BOOLEAN
#include <mach/boolean.h>
#include <mach/kern_return.h>
#include <mach/message.h>
#include <mach/mig_errors.h>
#include <mach/mig_support.h>

#ifndef	mig_internal
#define	mig_internal	static
#endif

#ifndef	mig_external
#define mig_external
#endif

#ifndef	TypeCheck
#define	TypeCheck 1
#endif

#ifndef	UseExternRCSId
#define	UseExternRCSId		1
#endif

#define BAD_TYPECHECK(type, check) ({\
  union { mach_msg_type_t t; unsigned32_t w; } _t, _c;\
  _t.t = *(type); _c.t = *(check); _t.w != _c.w; })
#define msgh_request_port	msgh_local_port
#define MACH_MSGH_BITS_REQUEST(bits)	MACH_MSGH_BITS_LOCAL(bits)
#define msgh_reply_port		msgh_remote_port
#define MACH_MSGH_BITS_REPLY(bits)	MACH_MSGH_BITS_REMOTE(bits)

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
mig_internal void _Xsend_message
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t messageType;
		string_t message;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t Download_send_message
		(mach_port_t port, string_t message);

	auto const mach_msg_type_t messageCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_STRING_C,
		/* msgt_size = */		8,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 1052) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->messageType, &messageCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = Download_send_message(In0P->Head.msgh_request_port, In0P->message);
}

/* Routine get_message */
mig_internal void _Xget_message
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t messageType;
		string_t message;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t Download_get_message
		(mach_port_t port, string_t message);

	auto const mach_msg_type_t messageType = {
		/* msgt_name = */		MACH_MSG_TYPE_STRING_C,
		/* msgt_size = */		8,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 24) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = Download_get_message(In0P->Head.msgh_request_port, OutP->message);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 1060;

	OutP->messageType = messageType;
}

/* Routine create_request */
mig_internal void _Xcreate_request
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t hostType;
		string_t host;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
		mach_msg_type_t responseType;
		bpi_response response;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t Download_create_request
		(mach_port_t port, string_t host, bpi_response *response);

	auto const mach_msg_type_t hostCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_STRING_C,
		/* msgt_size = */		8,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	auto const mach_msg_type_t responseType = {
		/* msgt_name = */		2,
		/* msgt_size = */		32,
		/* msgt_number = */		3,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 1052) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->hostType, &hostCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = Download_create_request(In0P->Head.msgh_request_port, In0P->host, &OutP->response);
	if (OutP->RetCode != KERN_SUCCESS)
		return;

	OutP->Head.msgh_size = 48;

	OutP->responseType = responseType;
}

/* Routine send_request */
mig_internal void _Xsend_request
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t requestType;
		string_t request;
	} Request;

	typedef struct {
		mach_msg_header_t Head;
		mach_msg_type_t RetCodeType;
		kern_return_t RetCode;
	} Reply;

	register Request *In0P = (Request *) InHeadP;
	register Reply *OutP = (Reply *) OutHeadP;
	mig_external kern_return_t Download_send_request
		(mach_port_t port, string_t request);

	auto const mach_msg_type_t requestCheck = {
		/* msgt_name = */		MACH_MSG_TYPE_STRING_C,
		/* msgt_size = */		8,
		/* msgt_number = */		1024,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

#if	TypeCheck
	if ((In0P->Head.msgh_size != 1052) ||
	    (In0P->Head.msgh_bits & MACH_MSGH_BITS_COMPLEX))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

#if	TypeCheck
	if (BAD_TYPECHECK(&In0P->requestType, &requestCheck))
		{ OutP->RetCode = MIG_BAD_ARGUMENTS; return; }
#endif	/* TypeCheck */

	OutP->RetCode = Download_send_request(In0P->Head.msgh_request_port, In0P->request);
}

static mig_routine_t Download_download_server_routines[] = {
		_Xsend_message,
		_Xget_message,
		_Xcreate_request,
		_Xsend_request,
};

mig_external boolean_t Download_download_server
	(mach_msg_header_t *InHeadP, mach_msg_header_t *OutHeadP)
{
	register mach_msg_header_t *InP =  InHeadP;
	register mig_reply_header_t *OutP = (mig_reply_header_t *) OutHeadP;

	auto const mach_msg_type_t RetCodeType = {
		/* msgt_name = */		MACH_MSG_TYPE_INTEGER_32,
		/* msgt_size = */		32,
		/* msgt_number = */		1,
		/* msgt_inline = */		TRUE,
		/* msgt_longform = */		FALSE,
		/* msgt_deallocate = */		FALSE,
		/* msgt_unused = */		0
	};

	register mig_routine_t routine;

	OutP->Head.msgh_bits = MACH_MSGH_BITS(MACH_MSGH_BITS_REPLY(InP->msgh_bits), 0);
	OutP->Head.msgh_size = sizeof *OutP;
	OutP->Head.msgh_remote_port = InP->msgh_reply_port;
	OutP->Head.msgh_local_port = MACH_PORT_NULL;
	OutP->Head.msgh_seqno = 0;
	OutP->Head.msgh_id = InP->msgh_id + 100;

	OutP->RetCodeType = RetCodeType;

	if ((InP->msgh_id > 2014) || (InP->msgh_id < 2011) ||
	    ((routine = Download_download_server_routines[InP->msgh_id - 2011]) == 0)) {
		OutP->RetCode = MIG_BAD_ID;
		return FALSE;
	}
	(*routine) (InP, &OutP->Head);
	return TRUE;
}

mig_external mig_routine_t Download_download_server_routine
	(const mach_msg_header_t *InHeadP)
{
	register int msgh_id;

	msgh_id = InHeadP->msgh_id - 2011;

	if ((msgh_id > 3) || (msgh_id < 0))
		return 0;

	return Download_download_server_routines[msgh_id];
}

