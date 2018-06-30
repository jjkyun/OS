/* Multilevel feedback Queue
 * Jae Kyun Kim
 * 1801410
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "mlfq.h"

// MLFQ: 0-queue:RR(quantum=8), 1-queue:RR(quantum=16), 2-queue:FCFS
#define MLFQ_LEVEL 3 
#define MLFQ_CAPACITY 10

// Make Multilevel Feedback Queue and CPU
struct mlfq_t* mlfq[MLFQ_LEVEL];
struct cpu_t cpu;

// This function is just to help easy to understand the printing the results
void ClearScreen(){
    printf("\e[1;1H\e[2J");
}

// Printing all the current situation
void PrintStatistic(){
    PrintMLFQ();
    PrintCPU();
    ClearScreen();
}

// Check if process burst is done
int Done(){
    return cpu.current->current_burst == 0;
}

// Check if CPU has spent all given time in process
int TimeOut(){
    // Except CPU in FCFS
    if(cpu.current->mlfq_level != MLFQ_LEVEL-1){
        return cpu.burst_left == 0;
    }
    return 0;
}

// Process going to lower proirty queue
void LowerQueue(){
    cpu.current->mlfq_level += 1;
    Push(mlfq[cpu.current->mlfq_level]->queue, cpu.current);
    cpu.current = NULL;
}

int main(int argc, const char* argv[]){
    srand(time(NULL));
    // Make MLFQ and CPU
    InitMLFQ();
    InitCPU();

    /* Assume that 1 loop is 1 burst */
    while(1){
        GenerateProcessRandomly();
        // if cpu is not pointing any process
        if(!cpu.current){
            FindNextProcess();
            usleep(1 * 1000000); // usleep function -> microsecond
        } else {
            HandleProcess();
            // If process burst is done
            if(Done()){
                DeleteProcess();
            }// If process burst is done but because of CPU givien time 
            else if(TimeOut()){
                LowerQueue();
            }
        }
        PrintStatistic();
        usleep(1 * 1000000);
    }
    return 0;
}
