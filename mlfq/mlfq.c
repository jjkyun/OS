#include <stdio.h>
#include <stdlib.h>
#include "mlfq.h"

extern struct mlfq_t* mlfq[MLFQ_LEVEL];

// Create Multilevel Feedback Queue
// quantum is for RR queue
struct mlfq_t *CreateMLFQ(int capacity, enum MLFQ_TYPE type, int quantum){
    struct mlfq_t *new = (struct mlfq_t*)malloc(sizeof(sizeof(struct mlfq_t)));
    new->queue = CreateQueue(capacity);
    new->type = type;
    new->quantum = quantum;

    return new;
}

// Initiate Multilevel Feedback Queue
void InitMLFQ(){
    // Make 8 quantum RR and 16 quantum RR
    for(int i = 0; i < MLFQ_LEVEL-1; i++){
        mlfq[i] = CreateMLFQ(MLFQ_CAPACITY, ROUND_ROBIN, 8 * (i+1));
    }

    // Make FCFS: Supposing quantum 0
    mlfq[MLFQ_LEVEL-1] = CreateMLFQ(MLFQ_CAPACITY, FCFS, 0);
}

void PrintMLFQ(){
    for(int i = 0; i < MLFQ_LEVEL; i++){
        printf("mlfq[%d], quantum:%d: ", i,  mlfq[i]->quantum);
        PrintQueue(mlfq[i]->queue);
    }
}

