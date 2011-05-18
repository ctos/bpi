#define _GNU_SOURCE

#include <mach.h>

#include <stdio.h>
#include <error.h>

struct integer_message
{
    mach_msg_header_t head;
    mach_msg_type_t type;

    int inline_integer;
};

void send_integer( mach_port_t destination, int i ){
    kern_return_t err;
    struct integer_message message;

    message.head.msgh_bits =MACH_MSGH_BITS_REMOTE(MACH_MSG_TYPE_MAKE_SEND);
    message.head.msgh_size = sizeof( struct integer_message );
    message.head.msgh_local_port = MACH_PORT_NULL;
    message.head.msgh_remote_port = destination;


    message.type.msgt_name = MACH_MSG_TYPE_INTEGER_32;
    message.type.msgt_size = 32;
    message.type.msgt_number = 1;
    message.type.msgt_inline = TRUE;
    message.type.msgt_longform = FALSE;
    message.type.msgt_deallocate = FALSE;

    message.inline_integer = i;

    err = mach_msg( &(message.head), MACH_SEND_MSG,message.head.msgh_size, 0, MACH_PORT_NULL,MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL );

    if( err == MACH_MSG_SUCCESS )
    {
        printf( "success: the message was queued\n" );
    }
    else
    {
        perror( "error: some unexpected error ocurred!\n");
    }

    return;
}


void receive_integer( mach_port_t source, int *ip )
{
    kern_return_t err;

    struct integer_message message;
   message.head.msgh_size = sizeof(message);

    err = mach_msg( &(message.head), MACH_RCV_MSG, 0,message.head.msgh_size, source,MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL );

    if( err == MACH_MSG_SUCCESS )
    {
        printf( "success: the message was received\n" );
    }
    else
    {
        perror( "error: Some unexpected error ocurred\n" );
    }

    *ip = message.inline_integer;

    return;
}


int main( void )
{
    int i, n_queue;
    int s, r;
    mach_port_t destination;

    kern_return_t err;
    printf("in qlimit\n");

    err = mach_port_allocate( mach_task_self(), MACH_PORT_RIGHT_RECEIVE,&destination );

    if( err != KERN_SUCCESS )
    {
        perror( "Error : could not allocate any port\n" );
    }
    if (fork() == 0)
        send_integer( destination, 9999);

    else 
    {
   	 receive_integer( destination, &r );

  	  printf("The received integer is : %d\n", r );

    }
    mach_port_deallocate( mach_task_self(), destination );

    return(r);
}
