#ifndef _GNU_SOURCE
	#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <hurd/hurd_types.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <hurd/ports.h>
#include <hurd/trivfs.h>

#include "download.h"
#include "download_types.h"

#define MAX_WEB_LENGTH 1024
#define MAX_FILE_LENGTH 1024

extern boolean_t Download_download_server(mach_msg_header_t *in, mach_msg_header_t *out);


int trivfs_fstype		= FSTYPE_MISC;
int trivfs_fsid 		= 0;
int trivfs_support_read		= 0;
int trivfs_support_write	= 0;
int trivfs_support_exec		= 0;
int trivfs_allow_open		= 0x00;
int trivfs_protid_nportclasses	= 0;
int trivfs_cntl_nportclasses	= 0;
/*web: the destiny website such as www.google.com
 *file:the destiny file name you want to get
 *portnumber:the destiny use the portnumber to connect destiny website the default portnumber is 80(http1.1's port)
 */
string_t message;
char web[MAX_WEB_LENGTH];
char file[MAX_FILE_LENGTH];
int  portnumber;
	
void get_host(char *src, char *web, char *file, int *port);
char *Rstrchr(char *s, char x);


int demuxer(mach_msg_header_t * in, mach_msg_header_t *out)
{
	return (Download_download_server(in, out) || trivfs_demuxer(in, out));
}

void trivfs_modify_stat (struct trivfs_protid * cred, io_statbuf_t *st)
{
}

error_t trivfs_goaway(struct trivfs_control *fsys, int flags)
{
	exit(0);
}

kern_return_t Download_send_message(mach_port_t port, string_t send_message)
{
	strcpy(message, send_message);
	return KERN_SUCCESS;
}

kern_return_t Download_get_message(mach_port_t port, string_t recv_message)
{
	strcpy(recv_message, message);
	return KERN_SUCCESS;
}

kern_return_t Download_create_request(mach_port_t port, string_t host, struct bpi_response &response)
{
	get_host(host, web, file, &portnumber);
	string_t request;

	sprintf(request,   "GET   /%s   HTTP/1.1\r\nAccept:   */*\r\nAccept-Language:   zh-cn\r\n\
User-Agent:   Mozilla/4.0   (compatible;   MSIE   5.01;   Windows   NT   5.0)\r\n\
Host:   %s:%d\r\nConnection:   Close\r\n\r\n ",   file,   web,   portnumber);
 


	return KERN_SUCCESS;
}

kern_return_t Download_send_request(mach_port_t port, string_t request)
{
    struct   	sockaddr_in   server_addr;
    struct   	hostent   *host;
    char   	*pt;
    char   	local_file[256];
    char	buffer[1024];
    FILE   	*fp;
    FILE	*file_out;
    int		sockfd;
    int		nbytes;
    int 	send;
    int		totalsend;

    file_out = fopen("1.txt", "w");

    fprintf(file_out, "dsfioahwdfiohadfhaiofhudsf.\n");
    if((host=gethostbyname(web))==NULL)/*get ip address*/
    {
        fprintf(stderr, "Gethostname   error,   %s\n ",   strerror(errno));
        exit(1);
    }

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)/*build socket connection*/
    {
        fprintf(stderr, "Socket   Error:%s\a\n ",strerror(errno));
        exit(1);
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(portnumber);
    server_addr.sin_addr=*((struct   in_addr   *)host-> h_addr);

/*   客户程序发起连接请求   */
    if(connect(sockfd,(struct   sockaddr   *)(&server_addr),sizeof(struct   sockaddr))==-1)/*连接网站*/
    {
        fprintf(stderr, "Connect   Error:%s\a\n ",strerror(errno));
        exit(1);
    }

    /*取得真实的文件名*/
    if(*file)    
    {
	    pt   =   Rstrchr(file,   '/');
    }
    else 
    {
	    pt   =   0;
    }

    memset(local_file,   0,   sizeof(local_file));

    if(pt   &&   *pt)    
    {
        if((pt   +   1)   &&   *(pt+1)) 
	{
		strcpy(local_file,   pt   +   1);
	}
        else
	{
		memcpy(local_file,   file,   strlen(file)   -   1);
	}
    }

    else   if(*file) 
    {
	    strcpy(local_file,   file);
    }

    else     
    {
	    strcpy(local_file,   "index.html");
    }

    fprintf(file_out, "local   filename   to   write:%s\n\n ",   local_file);
    fp   =   fopen(local_file,   "w");

    if(!fp)     {
        printf( "create   file   error!   %s\n ",   strerror(errno));
        return   0;
    }

    send   =   0;
    totalsend   =   0;
    nbytes=strlen(request);

    while(totalsend   <   nbytes)
    {
        send   =   write(sockfd,   request   +   totalsend,   nbytes   -   totalsend);

        if(send == -1) 
	{
		printf( "send   error!%s\n ",   strerror(errno));exit(0);
	}
		
        totalsend += send;
        printf( "%d   bytes   send   OK!\n ",   totalsend);

    }
    
    while((nbytes=read(sockfd,buffer,1))==1)
    {
	    fprintf(file_out, "%c ", buffer[0]);
	    fwrite(buffer, 1, 1, fp);
    }

    fclose(fp);
    close(sockfd);

    exit(0);

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
}

void   get_host(char   *   src,   char   *   web,   char   *   file,   int   *   port)     {
    char   *   pA;
    char   *   pB;

    memset(web,   0,   sizeof(web));
    memset(file,   0,   sizeof(file));
    *port   =   0;

    if(!(*src))  
    {
	    return;
    }

    pA   =   src;

    if(!strncmp(pA,   "http://",   strlen( "http://")))   
    {
	    pA   =   src+strlen( "http://");
    }

    else   if(!strncmp(pA,   "https://",   strlen( "https://")))
    {
	    pA   =   src+strlen( "https://");
    }

    pB   =   strchr(pA,   '/');

    if(pB)     
    {
        memcpy(web,   pA,   strlen(pA)   -   strlen(pB));

        if(pB+1)     
	{
            memcpy(file,   pB   +   1,   strlen(pB)   -   1);
            file[strlen(pB)   -   1]   =   0;
        }
    }
    else
    {
	    memcpy(web,   pA,   strlen(pA));
    }

    if(pB)
    {
	    web[strlen(pA)   -   strlen(pB)]   =   0;
    }
    else 
    {
	    web[strlen(pA)]   =   0;
    }

    pA   =   strchr(web,   ':');
    
    if(pA)
    {
	    *port   =   atoi(pA   +   1);
	    *pA = 0;
    }
    else 
    {
	    *port   =   80;
    }
}

char   *   Rstrchr(char   *   s,   char   x)     
{
    int   i   =   strlen(s);

    if(!(*s))
    {
	    return   0;
    }

    while(s[i-1])
    {
	    if(strchr(s   +   (i   -   1),   x))   
	    {
		   return   (s   +   (i   -   1));     
	    }
	    else
	    {
		    i--;
	    }
    }

    return   0;
}


