#ifndef _PROCESS_H_
#define _PROCESS_H_
#include <sys/types.h>

#define A_UNIT_TIME()					\
{							\
    volatile unsigned long i; for(i=0;i<1000000UL;i++); \
}							\

struct Process
{
    char name[32];
    int ready_time, execu_time;
    int pid;
};

int Assign_CPU(int pid, int core);

int Stop_Process(int pid);

int Run_Process(int pid);

int Execute_Process(struct Process now);
#endif
