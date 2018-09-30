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

int run_process(process current, int time)
{
 int i;
 for(i=0; i<time; i++)
 {
  current->completed++;
  if(current->completed >= current->duration)
   return 1;
 }
 return 0;
}

void lower_priority(int qno, process x)
{
 process temp;

 if(qno >= MAX_Q - 1)  // bottom cue or invalid
  return;

 
 for(temp=mlfq[qno]; temp; temp=temp->link)
  if(temp->link == x)
  {
   if(rear[qno] == x)         // x at back of cue
    rear[qno] = temp;

   temp->link = x->link;      // skip over process
   break;
  } 
 
 rear[qno+1]->link = x;  // add to back of lower queue
 rear[qno+1] = x;
 x->link = NULL;

 mlfq[qno]->duration--;
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
 printf("\nMULTI LEVEL FEEDBACK QUEUE:\n\n");
 for(i=0; i<MAX_Q; i++)
 {
  printf("Level %d (%d):\t", i, mlfq[i]->duration);
  for(temp=mlfq[i]->link; temp; temp=temp->link)
  {
   printf("%d (%d/%d)\t", temp->pid, temp->completed, temp->duration);
  }
  printf("\n\n\n");  
 }
}

void run_schedule()
{
 process current, next; 
 int slot = 10, i;
 for(i=0; i<MAX_Q; i++)     // iterate over all queues
 {
  while(mlfq[i]->duration)       // queue still has processes
  {
   for(current=mlfq[i]->link; current;)
   {
    next = current->link;       // current will be changed due to pointer manip

    if(run_process(current, slot))       // run process and if completed, move on
     terminate_process(current->pid);
    else
     lower_priority(i, current);

    current = next;     // effectively next = next->link

    system("clear");    // print stuff
    print_mlfq();
    sleep(1);
   }
  }
 }
}

int main()
{
 init_queue();
 int i, j;
 for(i=1; i<=6; i++)
  new_process(i, 20*(i%3+2));

 print_mlfq();
 sleep(1);
 run_schedule();

 

 return 0;
}
