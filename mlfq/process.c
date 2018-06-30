#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mlfq.h"

extern struct cpu_t cpu;
extern struct mlfq_t* mlfq[MLFQ_LEVEL];

struct process_t* CreateProcess(char name, int burst)
{
    struct process_t* new = (struct process_t *)malloc(sizeof(struct process_t));
    new->name = name;
    new->initial_burst = burst;
    new->current_burst = burst;
    new->mlfq_level = 0;
    
    return new;
}

// Process name from a-z
char GenerateNameRandomly(){
    /* 97 is 'a' in ASCII */
    /* 97 + 25 is 'z' in ASCII */
    return 97 + rand() % 26;
}

void GenerateProcessRandomly(){
    static int count = 0;
    // count is total process number
    if(count < 10){
        // 50% of making process 
        if((rand() % 2) == 0){
            char name = GenerateNameRandomly();
            // Automatically push process to 0-queue
            // Random burst from 1-50
            Push(mlfq[0]->queue, CreateProcess(name, rand() % 50 + 1));
            count++;
        }
    }
}

// CPU finding next process to work
void FindNextProcess(){
    for(int i = 0; i < MLFQ_LEVEL; i++){
        // if MLFQ is not empty
        if(!IsEmpty(mlfq[i]->queue)){
            // pop the process for CPU
            struct process_t poped = Pop(mlfq[i]->queue);
            cpu.current = CreateProcess(poped.name, poped.current_burst);
            cpu.current->mlfq_level = poped.mlfq_level;
            cpu.burst_left = mlfq[i]->quantum;
            break;
        }
    }
}

// CPU handling the process
void HandleProcess(){
    cpu.current->current_burst--;
    cpu.burst_left--;
}

void DeleteProcess(){
    free(cpu.current);
    cpu.current = NULL;
}





















