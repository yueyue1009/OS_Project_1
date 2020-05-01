#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#include "process.h"


int Assign_CPU(int pid, int core)
{
	if (core > sizeof(cpu_set_t)) {
		fprintf(stderr, "Wrong number of core\n");
		exit(-1);
	}

	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(core, &mask);

	if (sched_setaffinity(pid, sizeof(mask), &mask) < 0) {
		perror("sched_setaffinity error");
		exit(1);
	}

	return 0;
}

int Stop_Process(int pid)
{
    struct sched_param now;

    now.sched_priority = 0;
    int error = sched_setscheduler(pid, SCHED_IDLE, &now);

    if(error < 0)
    {
        perror("sched_setscheduler error");
        return -1;
    }

    return 0;
}

int Run_Process(int pid)
{
    struct sched_param now;

    now.sched_priority = 0;
    int error = sched_setscheduler(pid, SCHED_OTHER, &now);

    if(error < 0)
    {
        perror("sched_setscheduler error");
        return -1;
    }

    return 0;
}


int Execute_Process(struct Process now)
{
    int  pid = fork();

    if(pid < 0)
    {
        perror("fork error");
        return -1;
    }

    if(pid == 0)
    {
        unsigned long start_s, start_ns, end_s, end_ns;

        syscall(333, &start_s, &start_ns);

	for (int i = 0; i < now.execu_time; i++)
        {
		A_UNIT_TIME();
//		if(i%500==0)
//		    fprintf(stderr, "%s %d\n", now.name, i);
	}
	syscall(333, &end_s, &end_ns);
	syscall(334, start_s, start_ns, end_s, end_ns, getpid());
	exit(0);
    }

    Assign_CPU(pid, 1);
    return pid;

}
