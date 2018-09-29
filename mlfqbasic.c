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
  mlfq[i]->pid = -1;                        // invalid pid
  mlfq[i]->link = NULL;
  rear[i] = mlfq[i];   // no processes yet, so front == rear
 }
}

void new_process(int p, int d)
{
 process temp = (process) malloc(sizeof(proc)); // allocate memory for a new process
 temp->pid = p;
 temp->duration = d;
 temp->completed = 0;
 rear[0]->link = temp;   // add it to the top queue
 rear[0] = temp;
 rear[0]->link = NULL;
 mlfq[0]->duration++;    // one more process in the queue
}

void lower_priority(int qno)
{
 process temp;

 if(qno >= MAX_Q - 1)  // bottom cue or invalid
  return;

 temp = mlfq[qno]->link;    // front of queue to be removed from
 mlfq[qno]->link = temp->link;  // link to the next in queue
 
 rear[qno+1]->link = temp;    // add process to back of the lower queue
 rear[qno+1] = temp;
 rear[qno+1]->link = NULL;   

 mlfq[qno]->duration--;    // cange the number of processes in the queues
 mlfq[qno+1]->duration++;  
}

void terminate_process(int p) // terminate process given its pid
{
 process prev, del;
 int i, flag=0;             // flag to indicate found
 for(i=0; i<MAX_Q; i++)
 {
  for(del=mlfq[i]->link, prev=mlfq[i]; del; del=del->link, prev=prev->link)
  {
   
   if(del->pid == p)  // del points to process to be terminated
   {
    if(rear[i] == del) 
     rear[i] = prev;  // last element changes

    prev->link = del->link; // previous process skips doomed process
    free(del);
    flag = 1;
   }
  }
  if(flag == 1) break;    // no need to search after element found
 }

 mlfq[i]->duration--;   // one less process in the queue
}

void fast_terminate(int qno, int p)   // terminate a process knowing which queue
{
 process prev, del;
 for(del=mlfq[qno]->link, prev=mlfq[qno]; del; del=del->link, prev=prev->link)   
  if(del->pid == p)  // del points to process to be terminated
  {
   if(rear[qno] == del) 
    rear[qno] = prev;  // last element changes
 
   prev->link = del->link; // previous process skips doomed process
   free(del);
   break;
  }
 
 mlfq[qno]->duration--;   // one less process in the queue
}

void print_mlfq()
{
 int i;
 process temp;
 printf("MULTI LEVEL FEEDBACK QUEUE:\n\n");
 for(i=0; i<MAX_Q; i++)
 {
  printf("Level %d:", i);
  for(temp=mlfq[i]->link; temp; temp=temp->link)
  {
   printf("\t%d", temp->pid);
  }
  printf("\n");  
 }
 printf("\n");
 for(i=0; i<MAX_Q; i++)
  printf("Rear PID of queue %d: %d\n", i,rear[i]->pid); 
}

int main()
{
 init_queue();
 int i, j;
 for(i=0; i<10; i++)
 {
  new_process(i, i);
  for(j=0; j<i; j++)
  {
   lower_priority(j);
  }
  print_mlfq();
 }
 for(i=0; i<10; i++)
 {
  terminate_process(i);
  print_mlfq();
 }
 return 0;
}
