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
#include <time.h>

#define SET_COOKIE "set-cookie:"
#define BLOCK_NUM 1024
#define COOKIE_NAME_NUM 1024
#define MONTH_LENGTH 5 
#define MONTH_SUM 13
#define WEEK_LENGTH 5
#define WEEK_SUM 8
//////////////////////////////httpclient.c 开始///////////////////////////////////////////
char *Rstrchr(char * s, char x);
void  ToLowerCase(char  s[]);
void  GetHost(char * src, char * web, char * file, int * port);
int   saveCookie(char url[], char cookie[]);
int   getCookie(char url[], char cookie[]);
void  getCookieName(char cookie[], char cookie_name[]);
int   checkCookieName(char url[], char cookie_name[], char path[]);
void  deleteCookie(char url[], char cookie_name[], char path[]);
int   getSegmentData(char cookie[],char cookie_name[],  char segment_name[], char data[]);
void  createTempFile();
void  removeTempFile();
int   checkTime(char expires[]);
int   deletePath(char cookie[]);
int   judgeDomainEqual(char path[], char url[]);
void downloadHtml(char url[]);
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
	if (argc != 2)
	{
		printf("error: not enough parameters.\n");
		return 1;
	}
	downloadHtml(argv[1]);
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
void ToLowerCase(char  s[])   {
	int i;
	for (i = 0; s[i] != 0; i ++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			s[i] = s[i] - 'A' + 'a';
		}
	}
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
	char file_name[1024] = "COOKIE/";
	char cookie_information[1024];
	char cookie_name[1024];
	char path[1024];
	char expires[1024];
	char absolute_path[1024];
	char domain[1024];
	int i;

	getCookieName(cookie, cookie_name);
	if (getSegmentData(cookie, cookie_name, "expires", expires))
	{
		strcat(file_name, url);
	}
	else 
	{
		strcat(file_name, "temp");
	}


	getSegmentData(cookie, cookie_name, "path", path);
	if (getSegmentData(cookie, cookie_name, "domain", domain))
	{
		strcpy(absolute_path, domain);
	}
	else 
	{
		strcpy(absolute_path, url);
	}
	strcat(absolute_path, path);

	if (checkCookieName(file_name, cookie_name, absolute_path))
	{
		deleteCookie(file_name, cookie_name, absolute_path);
		printf("deleted cookie : %s.\n", cookie_name);
	}


	deletePath(cookie);

	strcat(cookie, "; path=");
	strcat(cookie, absolute_path);
	fp = fopen(file_name, "a+");
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
int checkCookieName(char url[], char cookie_name[], char path[])
{
	char cookie_information[1024];
	int flag;
	FILE *fp;	
	char *c;
//	printf("in checkCookieName\n");
	fp = fopen(url, "a+");
	flag = 0;

	printf("file %s:\n", url);
	while (1)
	{
		c = fgets(cookie_information, 1024, fp);
//		printf("c:%s.", c);
		if (c == NULL)
		{
			break;
		}
		cookie_information[strlen(cookie_information) - 1] = 0;
		char temp[1024];
		char temp_path[1024];
//		printf("%s", cookie_information);
		getCookieName(cookie_information, temp);

		getSegmentData(cookie_information, temp, "path", temp_path);
//		printf("cookie_information:%s.\n", cookie_name);
//		printf("temp:		  :%s.\n", temp);

		if (strcmp(temp, cookie_name) == 0 && strcmp(path, temp_path) == 0)
		{
//			fclose(fp);
	//		return 1;
			fclose(fp);
//			printf("equal:%s:%s.\n", temp, cookie_name);
			return 1;
//			;
		}
//		printf("check :%s\n", temp);
	}
	fclose(fp);

	return 0;

}
void deleteCookie(char url[], char cookie_name[], char path[])
{
	FILE *fp;	
	FILE *tp;
	char read_block[BLOCK_NUM];
	char temp_file_name[] = "temp_del_cookieXXXX";

	fp = fopen(url, "a+");
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
		char temp_path[1024];
		read_block[strlen(read_block) - 1] = 0;
		getCookieName(read_block, cookie_name_line);	
		getSegmentData(read_block, cookie_name_line, "path", temp_path);
//		printf("read_block:%s.\n", read_block);
//		printf("cookie_name :%s.\n", cookie_name);
//		printf("cookie_name_line :%s.\n", cookie_name_line);

//		printf("path		:%s.\n", path);
//		printf("temp_path	:%s.\n",temp_path);
		if (strcmp(cookie_name, cookie_name_line) != 0 || strcmp(path, temp_path) != 0)
		{
//			printf("fputs : %s.\n", read_block);
			fputs(read_block, tp);
			fwrite("\n", 1, 1, tp);
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
void createTempFile()
{
	FILE *fp;
	char file_name[] = "COOKIE/temp";
	fp = fopen(file_name, "a+");

	fclose(fp);


}
void removeTempFile()
{
	remove("COOKIE/temp");	
}
int  deletePath(char cookie[])
{
	
		int i = 0;
		while (cookie[i] != 0)
		{
			int flag = 0;
			char temp_segment_name[1024];
			char data[1024];
			int k;
			int l;

			int recent = i;
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
//			printf("temp_segment_name:%s.\n", temp_segment_name);
//			printf("recent:%d.\n", recent);
//			printf("i:%d.\n", i);
			if (strcmp(temp_segment_name, "path") == 0)
			{

				int z;
				for (z = recent; cookie[z + i - recent] != 0 ;  z++)
				{
					cookie[z] = cookie[z + i - recent]; 	
				}
				cookie[z] = 0;
				return 1;
			}
		}
		return 0;

}
int  judgeDomainEqual(char path[], char url[])
{
	if (strstr(url, path))
	{
		return 1;
	}

	return 0;
}
int getCookie(char url[], char cookie[])
{
	char file[1024];
	char web[1024];
	int  port;
	char file_name[1024];
	char every_line[1024];
	char path[1024];
	FILE *fp;

	GetHost(url, web, file, &port);
	strcat(url, "/");
	strcpy(file_name, "COOKIE/");
	strcat(file_name, web);

	fp = fopen(file_name, "r");
	cookie[0] = 0;
	while (fgets(every_line, 1024, fp) != NULL)
	{
		every_line[strlen(every_line) - 1] = 0;
		char cookie_name[1024];
		char data[1024];
		char expire[1024];
		printf("%s", every_line);
		getCookieName(every_line, cookie_name);
		getSegmentData(every_line,cookie_name, "path", path); 		
		getSegmentData(every_line,cookie_name, "expires", expire); 		
		printf("Path:%s.\n", path);
		printf("url :%s.\n", url);
		printf("expire:%s.\n", expire);
		if (judgeDomainEqual(path, url) && checkTime(expire))
		{
			getSegmentData(every_line, cookie_name, cookie_name, data);
			strcat(cookie, cookie_name);
			strcat(cookie, "=");
			strcat(cookie, data);	
			strcat(cookie, ";");
		}	
	}
	strcpy(file_name, "COOKIE/temp");
	fp = fopen(file_name, "r");
	while (fgets(every_line, 1024, fp) != NULL)
	{
		every_line[strlen(every_line) - 1] = 0;
		char cookie_name[1024];
		char data[1024];
		printf("%s", every_line);
		getCookieName(every_line, cookie_name);
		getSegmentData(every_line,cookie_name, "path", path); 		
		printf("Path:%s.\n", path);
		printf("url :%s.\n", url);
		if (judgeDomainEqual(path, url))
		{
			getSegmentData(every_line, cookie_name, cookie_name, data);
			strcat(cookie, cookie_name);
			strcat(cookie, "=");
			strcat(cookie, data);	
			strcat(cookie, ";");
		}	
	}

	if (cookie[0] != 0)
	{
		cookie[strlen(cookie) - 1] = 0;
		return 1;	
	}
	else 
	{
		return 0;
	}

}
int checkTime(char expires[])
{
	char month[MONTH_LENGTH];
	char every_month[MONTH_SUM][MONTH_LENGTH] = {"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	char week[WEEK_LENGTH];
	char every_week[WEEK_SUM][WEEK_LENGTH] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	int  day;
	int year;
	int second;
	int minute;
	int hour;
	char temp[1024];
	char expire_time[1024];
	char now_time[1024];
	int month_num;
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime (&rawtime);
	
	
	sscanf(expires, "%s %d-%[a-zA-Z]-%d %d:%d:%d", week, &day, month, &year, &hour, &minute, &second);

	printf("MONTH:%s.\n", month);

	for (month_num = 1; month_num < MONTH_SUM; month_num++)
	{
		if (strcmp(every_month[month_num], month) == 0)
		{
			break;
		}
	
	}
	sprintf(expire_time, "%04d-%02d-%02d-%02d-%02d-%02d", year, month_num, day, hour, minute, second);
	printf("EXPIRE_TIME:%s.\n",expire_time);
	sprintf(now_time, "%04d-%02d-%02d-%02d-%02d-%02d", timeinfo -> tm_year + 1900, timeinfo -> tm_mon + 1, timeinfo -> tm_mday, timeinfo -> tm_hour, timeinfo -> tm_min, timeinfo -> tm_sec);
	printf("NOW_TIME:%s.\n", now_time);
	if (strcmp(now_time, expire_time) < 0)
	{
		return 1;
	}
	return 0;
}
void downloadHtml(char url[])
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

	   ToLowerCase(url);

	   GetHost(url, host_addr, host_file, &portnumber);

//	   printf("webhost:%s\n", host_addr);
//	   printf("hostfile:%s\n", host_file);
//	   printf("portnumber:%d\n\n", portnumber);



	   if((host=gethostbyname(host_addr))==NULL)
	   {

		   fprintf(stderr,"Gethostname error, %s\n", strerror(errno));
		   exit(1);
	   }


	   if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	   {
		   fprintf(stderr,"Socket Error:%s\a\n",strerror(errno));
		   exit(1);
	   }
	  
	   bzero(&server_addr,sizeof(server_addr));
	   server_addr.sin_family=AF_INET;
	   server_addr.sin_port=htons(portnumber);
	   server_addr.sin_addr=*((struct in_addr *)host->h_addr);

	   if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
	   {
		   fprintf(stderr,"Connect Error:%s\a\n",strerror(errno));
		   exit(1);
	   }


	   sprintf(request, "GET /%s HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-cn\r\n\
User-Agent: Mozilla/5.0\r\n\
Host: %s:%d\r\nConnection: Close\r\n\r\n", host_file, host_addr, portnumber); 
	   printf("%s", request);

	   

	   if(host_file && *host_file) 
	   {
		   pt = Rstrchr(host_file, '/');
	   }
	   else 
	   {
		   pt = 0;
	   }

	   memset(local_file, 0, sizeof(local_file));

	   if(pt && *pt)
	   {
		   if((pt + 1) && *(pt+1))   
		   {
			   strcpy(local_file, pt + 1);
		   }
		   else 
		   {
			   memcpy(local_file, host_file, strlen(host_file) - 1);
		   }
	   }

	   else if(host_file && *host_file) 
	   {
		   strcpy(local_file, host_file);
	   }
	   else
	   {
		   strcpy(local_file, "index.html");
	   }

	   printf("local filename to write:%s\n\n", local_file);

	   send = 0;
	   totalsend = 0;
	   nbytes=strlen(request);

	   while(totalsend < nbytes) {

		   send = write(sockfd, request + totalsend, nbytes - totalsend);

		   if(send==-1)   
		   {
			   printf("send error!%s\n", strerror(errno));
			   exit(0);
		   }

		   totalsend+=send;
		   printf("%d bytes send OK!\n", totalsend);
	   }

	   fp = fopen(local_file, "a");

	   if(!fp)  
	   {
		   printf("create file error! %s\n", strerror(errno));
		   return;
	   }

	   printf("\nThe following is the response header:\n");

	   i=0;
	   everyline_number = 0;	

	   createTempFile();

//	   removeTempFile();
	   while((nbytes=read(sockfd,buffer,1))==1)
	   {

		   if(i < 4)   {
			   if(buffer[0] == '\r' || buffer[0] == '\n')  
			   {
				   i++;
				   head_line[everyline_number] = 0;
				   everyline_number = 0;
				   printf("%s\n", head_line);

				   ToLowerCase(head_line);
				   char *ptr;
				   
				   printf("HEAD_LINE:%s.\n", head_line);



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
//			   printf("break.\n");
			   break;
		   }
	   }

	   char test_cookie[1024];
	   if (getCookie(url, test_cookie))
	   {
		   printf("test_cookie:%s.\n", test_cookie);
	   }
	   else 
	   {
	   	pritnf("can not find the cookie in COOKIE.\n");
	   }

	   while ((nbytes=read(sockfd,buffer,1))==1)	   
	   {
		   fwrite(buffer, 1, 1, fp);
		   i++;

		   if (i % 1024 == 0) 
		   {
			   fflush(fp);	   
		   }
	   }

	   fclose(fp);
	   close(sockfd);

}
