#include <stdio.h>
#include <sys/sysinfo.h>

int memory_info(struct sysinfo *info)
{
    int ret = -1;
//  struct sysinfo info;
//  if ((ret = sysinfo(&info)) == 0)
    if ((ret = sysinfo(info)) == 0)
    {
        printf("Seconds since boot: %ld\n", info->uptime);
        printf("Minutes since boot: %ld\n", info->uptime/60);
        printf("Total memory: %ldMB\n", (info->totalram/1024)/1024);
        printf("Total usable main memory size: %ldMB\n", (info->freeram/1024)/1024);
//      printf("Available memory size: %d\n", info.mem_unit);
        printf("Amount of shared memory: %ldMB\n", (info->sharedram/1024)/1024);
	    printf("Memory used by buffers: %ldMB\n", (info->bufferram/1024)/1024);
	    printf("Total swap space size: %ldMB\n", (info->totalswap/1024)/1024);
	    printf("swap space still available: %ldMB\n", (info->freeswap/1024)/1024);	
        printf("Number of current processes: %d\n", info->procs);

        return 0;
    }
    else
    {
        return -1;
    }
}