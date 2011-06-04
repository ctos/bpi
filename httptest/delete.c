#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void getSegmentData(char cookie[],char cookie_name[],  char segment_name[], char data[]);
void getCookieName(char cookie[], char cookie_name[]);
int deletePath(char cookie[]);
int main()
{
	FILE *fp = fopen("COOKIE/cms.hit.edu.cn", "a+");
	if (fp == NULL)
	{
		printf("open file %s failed", "cms.hit.edu.cn");
	}
	
	char temp[1024];
	
	while (fgets(temp, 1024, fp))
	{
		printf("%s\n", temp);

		temp[strlen(temp) - 1] = 0;
		deletePath(temp);
		printf("%s.\n", temp);
	}
	fclose(fp);
	return 0;
}
void getSegmentData(char cookie[], char cookie_name[], char segment_name[], char data[])
{
		
		char temp_name[1024];
		getCookieName(cookie, temp_name);
		printf("temp_name:%s.\n", temp_name);
		printf("cookie_name:%s.\n", cookie_name);
		if (strcmp(temp_name, cookie_name) != 0)
		{
			data[0] = 0;
			return;
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

				return ;
			}
		}
		return;

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

			for (; cookie[i] != 0 && (cookie[i] == ' ' || cookie[i] == ';'); i++);
			int recent = i;
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
			printf("temp_segment_name:%s.\n", temp_segment_name);
			printf("recent:%d.\n", recent);
			printf("i:%d.\n", i);
			if (strcmp(temp_segment_name, "path") == 0)
			{

				int z;
				for (z = recent; z < i && cookie[z + i - recent - 1] != 0;  z++)
				{
					cookie[z] = cookie[z + i - recent]; 	
				}
				return 1;
			}
		}
		return 0;



}
