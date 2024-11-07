// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"


// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n,int quantum) 
{ 
  /*
     1. Create an array *rem_bt[]* to keep track of remaining burst time of processes. This array is initially a copy of *plist[].bt* (all processes burst times)
     2. Store waiting times of processes in plist[].wt. Initialize this array as 0.
     3. Initialize time : t = 0
     4. Keep traversing the all processes while all processes are not done. Do following for i'th process if it is not done yet.
        - If rem_bt[i] > quantum
          (i)  t = t + quantum
          (ii) bt_rem[i] -= quantum;
        - Else // Last cycle for this process
          (i)  t = t + bt_rem[i];
          (ii) plist[i].wt = t - plist[i].bt
          (iii) bt_rem[i] = 0; // This process is over
       
   */
   int t = 0;  // Current time
    int *rem_bt = (int *)malloc(n * sizeof(int));  // Array to store remaining burst times for each process

    // Initialize remaining burst times as the original burst times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
        plist[i].wt = 0;  // Initialize waiting times to 0
    }

    // Continue looping until all processes are complete
    while (1) {
        int all_done = 1;  // Assume all processes are done

        // Traverse each process
        for (int i = 0; i < n; i++) {
            // Check if this process still has remaining burst time
            if (rem_bt[i] > 0) {
                all_done = 0;  // There’s at least one process left to execute

                if (rem_bt[i] > quantum) {
                    // If remaining burst time is more than quantum
                    t += quantum;       // Increase current time by quantum
                    rem_bt[i] -= quantum;  // Decrease remaining burst time by quantum
                } else {
                    // Last cycle for this process
                    t += rem_bt[i];       // Increase current time by the remaining burst time
                    plist[i].wt = t - plist[i].bt;  // Calculate waiting time for this process
                    rem_bt[i] = 0;         // Process is complete
                }
            }
        }

        // If all processes are done, exit the loop
        if (all_done) break;
    }

    free(rem_bt);
} 

// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{ 
      /*
       * 1 Traverse until all process gets completely executed.
         - Find process with minimum remaining time at every single time lap.
         - Reduce its time by 1.
         - Check if its remaining time becomes 0 
         - Increment the counter of process completion.
         - Completion time of *current process = current_time +1;*
         - Calculate waiting time for each completed process. *wt[i]= Completion time - arrival_time-burst_time*
         - Increment time lap by one.
 
     */
    int completed = 0, current_time = 0, min_bt_index;
    int min_remaining_bt = INT_MAX;
    int *rem_bt = (int *)malloc(n * sizeof(int));
    int *is_completed = (int *)calloc(n, sizeof(int));  // Track which processes are completed

    // Initialize remaining burst times
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;
    }

    while (completed != n) {
        min_bt_index = -1;
        min_remaining_bt = INT_MAX;

        // Find process with the minimum remaining burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= current_time && !is_completed[i] && rem_bt[i] < min_remaining_bt) {
                min_remaining_bt = rem_bt[i];
                min_bt_index = i;
            }
        }

        if (min_bt_index != -1) {
            // Process found, reduce remaining burst time
            rem_bt[min_bt_index] -= 1;
            current_time++;

            // If process is completed, calculate waiting time
            if (rem_bt[min_bt_index] == 0) {
                completed++;
                is_completed[min_bt_index] = 1;
                int completion_time = current_time;
                plist[min_bt_index].wt = completion_time - plist[min_bt_index].art - plist[min_bt_index].bt;
                if (plist[min_bt_index].wt < 0) {
                    plist[min_bt_index].wt = 0;  // Avoid negative waiting time
                }
            }
        } else {
            // No process is ready, increment time
            current_time++;
        }
    }

    free(rem_bt);
    free(is_completed);

} 

// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
int my_comparer(const void *this, const void *that)
{ 
  
    /*  
     * 1. Cast this and that into (ProcessType *)
     * 2. return 1 if this->pri < that->pri
     */ 
    ProcessType *proc1 = (ProcessType *)this;
    ProcessType *proc2 = (ProcessType *)that;

    if (proc1->pri < proc2->pri) {
        return -1;
    } else if (proc1->pri > proc2->pri) {
        return 1;
    } else {
        return 0;
    }
} 

//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority( ProcessType plist[], int n) 
{ 
  
   /*
    * 1- Sort the processes (i.e. plist[]), burst time and priority according to the priority.
    * 2- Now simply apply FCFS algorithm.
    */
    qsort(plist, n, sizeof(ProcessType), my_comparer);
    findavgTimeFCFS(plist, n);
  
    //Display processes along with all details 
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 