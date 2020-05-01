#ifndef _PROCESS_CONTROL_H_
#define _PROCESS_CONTROL_H_
#include <sys/types.h>
#include <stdio.h>
#include "process.h"
#define FIFO 1
#define RR 2
#define SJF 3
#define PSJF 4

int scheduling(struct Process *process, int Num_of_Process, int policy);

#endif
