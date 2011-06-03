/******* http客户端程序 httpclient.c ************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <limits.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

#define SET_COOKIE "Set-Cookie:"
#define BLOCK_NUM 1024
#define COOKIE_NAME_NUM 1024
//////////////////////////////httpclient.c 开始///////////////////////////////////////////
char * Rstrchr(char * s, char x);
void ToLowerCase(char * s);
void GetHost(char * src, char * web, char * file, int * port);
int saveCookie(char url[], char cookie[]);
void getCookieName(char cookie[], char cookie_name[]);
int  checkCookieName(char url[], char cookie_name[]);
void  deleteCookie(char url[], char cookie_name[]);
int  getSegmentData(char cookie[],char cookie_name[],  char segment_name[], char data[]);

/*********************************************************************
  *filename: httpclient.c
  *purpose: HTTP协议客户端程序，可以用来下载网页
  *wrote by: zhoulifa(zhoulifa@163.com) 周立发(http://zhoulifa.bokee.com)
              Linux爱好者 Linux知识传播者 SOHO族 开发者 最擅长C语言
	      *date time:2006-03-11 21:49:00
	      *Note: 任何人可以任意复制代码并运用这些代码，当然包括你的商业用途
	      *                          但请遵循GPL
	      *********************************************************************/
int main(int argc, char *argv[])
{
	   int sockfd;
	   char buffer[1024];
           struct sockaddr_in server_addr;
           struct hostent *host;
           int portnumber,nbytes;
	   char host_addr[256];
	   char host_file[1024];
	   char local_file[256];
	   FILE * fp;	
	   char request[1024];	         
	   int send, totalsend;
	   int i;
	   char * pt;
	   char head_line[1024];
	   int everyline_number;


	   if(argc!=2)

	   {

		   fprintf(stderr,"Usage:%s web-address\a\n",argv[0]);

		   exit(1);

	   }

	   printf("parameter.1 is: %s\n", argv[1]);






	   GetHost(argv[1], host_addr, host_file, &portnumber);/*分析网址、端口、文件名等*/

	   printf("webhost:%s\n", host_addr);

	   printf("hostfile:%s\n", host_file);

	   printf("portnumber:%d\n\n", portnumber);



	   if((host=gethostbyname(host_addr))==NULL)/*取得主机IP地址*/

	   {

		   fprintf(stderr,"Gethostname error, %s\n", strerror(errno));

		   exit(1);

	   }



	   /* 客户程序开始建立 sockfd描述符 */



	   if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)/*建立SOCKET连接*/

	   {

		   fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));

		   exit(1);

	   }



	   /* 客户程序填充服务端的资料 */

	   bzero(&server_addr,sizeof(server_addr));
	   server_addr.sin_family=AF_INET;
	   server_addr.sin_port=htons(portnumber);
	   server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	   /* 客户程序发起连接请求 */

	   if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)/*连接网站*/

	   {
		   fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		   exit(1);
	   }


		sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
User-Agent: Mozilla/5.0\r\n\
Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber); 
	   printf("%s", request);/*准备request，将要发送给主机*/

	   /*取得真实的文件名*/

	   if(host_file && *host_file)   pt = Rstrchr(host_file, '/');
	   else pt = 0;

	   memset(local_file, 0, sizeof(local_file));

	   if(pt && *pt)   {
		   if((pt + 1) && *(pt+1))   strcpy(local_file, pt + 1);
		   else   memcpy(local_file, host_file, strlen(host_file) - 1);
	   }

	   else if(host_file && *host_file)   strcpy(local_file, host_file);
	   else   strcpy(local_file, "index.html");
	   printf("local filename to write:%s\n\n", local_file);


	   /*发送http请求request*/

	   send = 0;totalsend = 0;
	   nbytes=strlen(request);

	   while(totalsend < nbytes) {



		   send = write(sockfd, request + totalsend, nbytes - totalsend);

		   if(send==-1)   {printf("send error!%s\n", strerror(errno));exit(0);}

		   totalsend+=send;

		   printf("%d bytes send OK!\n", totalsend);

	   }


	   fp = fopen(local_file, "a");

	   if(!fp)   {
		   printf("create file error! %s\n", strerror(errno));
		   return 0;
	   }

	   printf("\nThe following is the response header:\n");

	   i=0;
	   everyline_number = 0;	


	   /* 连接成功了，接收http响应，response */

	   while((nbytes=read(sockfd,buffer,1))==1)
	   {

		   if(i < 4)   {
			   if(buffer[0] == '\r' || buffer[0] == '\n')  
			   {
				   i++;
				   head_line[everyline_number] = 0;
				   everyline_number = 0;
				   printf("%s\n", head_line);
				   char *ptr;
				   if ((ptr = strstr(head_line, SET_COOKIE)) != NULL)
				   {
					   fflush(fp);
					saveCookie(host_addr, ptr + strlen(SET_COOKIE));


				   }
			   }
			   else 
			   {
				   i = 0;
				   head_line[everyline_number] = buffer[0];
				   everyline_number ++;
			   }
		   }
		   else 
		   {
			   break;
		   }
	   }


	   while ((nbytes=read(sockfd,buffer,1))==1)	   
	   {
		   fwrite(buffer, 1, 1, fp);/*将http主体信息写入文件*/
		   i++;
		   if (i % 1024 == 0) fflush(fp);	   
	   }

	   fclose(fp);
	   /* 结束通讯 */
	   close(sockfd);

	   exit(0);
}
/********************************************
  功能：搜索字符串右边起的第一个匹配字符
  ********************************************/
char * Rstrchr(char * s, char x)   {
	   int i = strlen(s);
	   while(s[i-1]) if(strchr(s + (i - 1), x))   return (s + (i - 1));   else i--;
	   return 0;
}

/********************************************
  功能：把字符串转换为全小写
  ********************************************/
void ToLowerCase(char * s)   {
	   while(*s)   *s=tolower(*s++);
}

/**************************************************************
  功能：从字符串src中分析出网站地址和端口，并得到用户要下载的文件
  ***************************************************************/
void GetHost(char * src, char * web, char * file, int * port)   {
	char * pA;
      	char * pB;
	memset(web, 0, sizeof(web));
        memset(file, 0, sizeof(file));
        *port = 0;
        if(!(*src))   return;
	pA = src;
	if(!strncmp(pA, "http://", strlen("http://")))   pA = src+strlen("http://");

	else if(!strncmp(pA, "https://", strlen("https://")))   pA = src+strlen("https://");			
	pB = strchr(pA, '/');			 
	if(pB)   {

		memcpy(web, pA, strlen(pA) - strlen(pB));

		if(pB+1)   {

			memcpy(file, pB + 1, strlen(pB) - 1);

			file[strlen(pB) - 1] = 0;

		}

	}

	else   memcpy(web, pA, strlen(pA));

	if(pB)   web[strlen(pA) - strlen(pB)] = 0;

	else   web[strlen(pA)] = 0;

	pA = strchr(web, ':');

	if(pA)   *port = atoi(pA + 1);

	else *port = 80;
}

int saveCookie(char url[], char cookie[])
{
	FILE *fp;
	char file_name[1024];
	char cookie_information[1024];
	char cookie_name[1024];

	int i;
	strcpy(file_name, url);



	getCookieName(cookie, cookie_name);
	if (checkCookieName(file_name, cookie_name))
	{
		deleteCookie(url, cookie_name);
		printf("deleted cookie : %s.\n", cookie_name);
	}


	fp = fopen(file_name, "a");
	for (i = 0; cookie[i] != 0; i ++)
	{
		fwrite(&cookie[i], 1, 1, fp);		
		fflush(fp);
	}

		fwrite("\n", 1, 1, fp);

	fflush(fp);
//	printf("url : %s\n cookie : %s\n", url, cookie);
	fclose(fp);
}
void getCookieName(char cookie[], char cookie_name[])
{
	int i, k;

	for (i = 0; cookie[i] == ' '; i ++ )
	{
		;
	}
	for (k = 0; cookie[i] != '=' && cookie[i] != 0; i ++, k ++)
	{
			cookie_name[k] = cookie[i];
	}
	cookie_name[k] = 0;
}
int checkCookieName(char url[], char cookie_name[])
{
	char cookie_information[1024];
	int flag;
	FILE *fp = fopen(url, "a+");	
	char *c;

	freopen(url, "r", stdin);
//	printf("in checkCookieName\n");

	flag = 0;

	printf("file %s:\n", url);
	while (1)
	{
		c = fgets(cookie_information, 1024, fp);
		printf("c:%s.", c);
		if (c == NULL)
		{
			break;
		}
		char temp[1024];
		printf("%s", cookie_information);
		getCookieName(cookie_information, temp);
		printf("cookie_information:%s.\n", cookie_name);
		printf("temp:		  :%s.\n", temp);
		if (strncmp(temp, cookie_name, strlen(temp) > strlen(cookie_name)? strlen(temp): strlen(cookie_name)) == 0)
		{
//			fclose(fp);
	//		return 1;
			printf("equal:%s:%s.\n", temp, cookie_name);
			return 1;
//			;
		}
		printf("check :%s\n", temp);
	}
	fclose(fp);
	
	return 0;

}
void deleteCookie(char url[], char cookie_name[])
{
	FILE *fp = fopen(url, "a+");	
	FILE *tp;
	char read_block[BLOCK_NUM];
	char temp_file_name[] = "temp_del_cookieXXXX";

	if (fp == NULL)
	{
		printf("Did not have cookie %s.\n", url);
	}

	mkstemp(temp_file_name);

	tp = fopen(temp_file_name, "a+");

	if (tp == NULL)
	{
		printf("Can not create temp file.\n");
	}

	while (fgets(read_block, BLOCK_NUM, fp))
	{
		char cookie_name_line[COOKIE_NAME_NUM];
		getCookieName(read_block, cookie_name_line);

//		printf("read_block:%s.\n", read_block);
//		printf("cookie_name :%s.\n", cookie_name);
//		printf("cookie_name_line :%s.\n", cookie_name_line);

		if (strcmp(cookie_name, cookie_name_line) != 0)
		{
			printf("fputs : %s.\n", read_block);
			fputs(read_block, tp);
			fflush(tp);
		}
	}

	remove(url);
	rename(temp_file_name, url);
	fflush(fp);
	fclose(fp);
}
int  getSegmentData(char cookie[], char cookie_name[], char segment_name[], char data[])
{

		char temp_name[1024];
		getCookieName(cookie, temp_name);
//		printf("temp_name:%s.\n", temp_name);
//		printf("cookie_name:%s.\n", cookie_name);
		if (strcmp(temp_name, cookie_name) != 0)
		{
			data[0] = 0;
			return 0;
		}

		int i = 0;
		while (cookie[i] != 0)
		{
			int flag = 0;
			char temp_segment_name[1024];
			int k;
			int l;

			for (; cookie[i] != 0 && (cookie[i] == ' ' || cookie[i] == ';'); i++);
			for (k = 0, l = 0; cookie[i] != 0; i ++)
			{
				if (cookie[i] == '=')
				{
					flag = 1;
				}
				else if (!flag)
				{
					temp_segment_name[k] = cookie[i];
					k++;
				}
				else if (flag == 1 && cookie[i] != ';')
				{
					data[l] = cookie[i];	
					l++;
				}

				else if (cookie[i] == ';')
				{
					flag = 2;
					break;
				}
			}
			temp_segment_name[k] = 0;
			data[l] = 0;
			if (strcmp(temp_segment_name, segment_name) == 0)
			{

				return 1;
			}
		}
		return 0;

}
//////////////////////////////httpclient.c 结束///////////////////////////////////////////

