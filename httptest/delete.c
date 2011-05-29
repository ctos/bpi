#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	FILE *fp = fopen("cms.hit.edu.cn", "a+");
	if (fp == NULL)
	{
		printf("open file %s failed", "cms.hit.edu.cn");
	}
	
	char temp[1024];
	FILE *cur = fp;
	char temp_file_name[] = "temp_del_cookieXXXXXX";
	mkstemp(temp_file_name);
	FILE *tp = fopen(temp_file_name, "a+");
	
	while (fgets(temp, 1024, fp))
	{
		printf("%s\n", temp);
		fputs(temp, tp);
	}
	remove("cms.hit.edu.cn");
	rename(temp_file_name, "cms.hit.edu.cn");
	fclose(fp);
	return 0;
}
