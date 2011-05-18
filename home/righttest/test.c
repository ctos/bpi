#include <mach.h>
#include <stdio.h>
#include <cthreads.h>
#include "ux.h"
int main()
{
	mach_port_t *abc;
	int nname;
	int ncount;
	mach_port_type_t *abctype;

	kern_return_t err;
	err = mach_port_names(mach_task_self(), &abc, &nname, &abctype, &ncount);
	if (err == KERN_SUCCESS)
	{
		int i;
		for ( i = 0; i < nname; i++)
		{
			printf("PORT:%d\tRIGHT:%d\n", *(abc + i), *(abctype + i));
		
		}
	
	}
	int pid;
	scanf("%d.\n", &pid);
	printf("TASK IS %d.\n", task_by_pid(pid));





	return 0;
}

