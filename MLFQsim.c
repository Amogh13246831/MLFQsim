//Implementation of an MLFQ using an array of linked lists

#include<stdio.h>
#include<stdlib.h>

#define MAX_Q 8

typedef struct proc* process;
typedef struct proc
{
 int pid;
 int duration;
 int completed;
 process link;
} proc;

process mlfq[MAX_Q];  // the Multi Level Feedback Queue
process rear[MAX_Q];  // point to the rear of the queue, to insert in O(1)

void init_queue()
{
 int i;
 for(i=0; i<MAX_Q; i++)
 {
  mlfq[i] = (process) malloc(sizeof(proc)); // a header node for each queue
  mlfq[i]->duration = 0;                    // used to store no. of processes
  mlfq[i]->link = NULL;
  rear[i] = mlfq[i];   // no processes yet, so front == rear
 }
}

void new_process(int p, int d)
{
 process temp = (process) malloc(sizeof(proc)); // allocate memory for a new process
 temp->pid = p;
 temp->duration = d;
 temp-> completed = 0;
 rear[0]->link = temp;   // add it to the top queue
 rear[0] = temp;
}

void lower_priority(int qno)
{
 process temp;

 if(qno == MAX_Q - 1)  // bottom cue
  return;

 temp = mlfq[qno]->link;    // front of queue to be removed from
 mlfq[qno]->link = temp->link;  // link to the next in queue
 
 rear[qno-1]->link = temp;    // add process to back of the lower queue
 rear[qno-1] = temp;
 rear[qno-1]->link = NULL;     
}

int main()
{
 return 0;
}
