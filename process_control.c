#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "process_control.h"
#include "process.h"

//store the last time context switch happen in RR
int RR_time;

// time has ran in total
int total_time;

// which process is running
int now;

// store the last process run in FIFO
int FIFO_now;

// store the number of process that has finished
int done;

// store the number of process has start running
int started;

int queue[3000];
int head, tail;

int cmp(const void *first, const void *second) {
	return ((struct Process *) first) -> ready_time - ((struct Process *) second) -> ready_time;
}

int Find_Next(struct Process *process, int Num_of_Process, int policy)
{
    if(now != -1 && (policy == FIFO || policy == SJF))
        return now;

    else if(policy == FIFO)
    {
        if(process[FIFO_now + 1].pid != -1)
            return ++FIFO_now;
        else
            return -1;
    }

    else if(policy == RR)
    {
        if(head == tail)
            return now;
        else if(now == -1)
        {
            int new = queue[head];
            head = (head + 1) % 3000;
            return new;
        }
        else if ((total_time - RR_time) % 500 == 0)
        {
            if(head == tail)
                return now;
            else
            {
                int new = queue[head];
                head = (head + 1) % 3000;
                queue[tail] = now;
                tail = (tail + 1) % 3000;
                return new;
            }
        }
        else
            return now;
    }

    else if(policy == SJF || policy == PSJF)
    {
        int shortest = -1;
        for(int i = 0; i < Num_of_Process; i++)
        {
            if(process[i].pid == -1 || process[i].execu_time == 0)
                continue;
            else if(shortest == -1 || process[shortest].execu_time > process[i].execu_time)
                shortest = i;
        }

        return shortest;
    }

    return -1;

}

int Scheduling(struct Process *process, int Num_of_Process, int policy)
{
    Assign_CPU(getpid(), 0);

    Run_Process(getpid());

    qsort(process, Num_of_Process, sizeof(struct Process), cmp);

    for(int i = 0 ; i < Num_of_Process; i++)
        process[i].pid = -1;

    now = -1;
    done = 0;
    head = 0;
    tail = 0;
    started = 0;
    FIFO_now = -1;
    total_time = 0;
    while(1)
    {
        while(started < Num_of_Process && process[started].ready_time == total_time)
        {	
            //fprintf(stderr, "now %d run %s\n", total_time, process[started].name);
            process[started].pid = Execute_Process(process[started]);
            Stop_Process(process[started].pid);
            if(policy == RR)
            {
                queue[tail] = started;
                tail = (tail + 1) % 3000;
            }
	    started++;
        }	
        if(now != -1 && process[now].execu_time == 0)
        {
            //fprintf(stderr, "%s done at %d\n", process[now].name, total_time);
            waitpid(process[now].pid, NULL, 0);
            printf("%s %d\n", process[now].name, process[now].pid);
            fflush(stdout);
            now = -1;
            if(++done == Num_of_Process)
                break;
        }

        int next = Find_Next(process, Num_of_Process, policy);

        if(next == -1)
        {
            A_UNIT_TIME(); 
            total_time++;
            continue;
        }
        else if(next != now)
        {
            Run_Process(process[next].pid);
            Stop_Process(process[now].pid);
            now = next;
            RR_time = total_time;
        }

        A_UNIT_TIME();
        process[now].execu_time--;
        total_time++;

    }
}
