#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include "process.h"
#include "process_control.h"
#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4


int main()
{
    int policy, Num_of_Process;
    char buffer[8];
    struct Process *process;

    scanf("%s%d", buffer, &Num_of_Process);

    if(strcmp(buffer, "FIFO") == 0)
        policy = FIFO;
    else if(strcmp(buffer, "RR") == 0)
        policy = RR;
    else if(strcmp(buffer, "SJF") == 0)
        policy = SJF;
    else if(strcmp(buffer, "PSJF") == 0)
        policy = PSJF;
    else
    {
        fprintf(stderr, "wrong policy type\n");
	exit(0);
    }

    process = (struct Process *)malloc(Num_of_Process * sizeof(struct Process));

    for(int i = 0 ; i < Num_of_Process; i++)
        scanf("%s%d%d", process[i].name, &process[i].ready_time, &process[i].execu_time);

    Scheduling(process, Num_of_Process, policy);
}
