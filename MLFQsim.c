//Implementation of an MLFQ using an array of linked lists

#include<stdio.h>
#include<stdlib.h>

#define MAX_Q 8

typedef struct proc* process;
typedef struct proc
{
 int pid;
 int duration;
} proc;

process mlfq[MAX_Q];  // the Multi Level Feedback Queue

void init_queue()
{
 int i;
 for(i=0; i<MAX_Q; i++)
  mlfq[i] = NULL;
}

int main()
{
 return 0;
}
