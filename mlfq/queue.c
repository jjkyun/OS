#include <stdio.h>
#include <stdlib.h>
#include "mlfq.h"

struct queue_t* CreateQueue(int capacity){
    if(capacity > 1){
        struct queue_t* queue = (struct queue_t*)malloc(sizeof(struct queue_t));
        queue->capacity = capacity;
        queue->arr = (process *)malloc(sizeof(process) * queue->capacity);
        queue->size = 0;
        queue->head = 1;
        queue->tail = 0;
        
        return queue;
    } else {
        /* if capacity is lower than 1, it is just a variable not array */
        return NULL;
    }   
}

void DeleteQueue(struct queue_t* queue){
    free(queue->arr);
    free(queue);
}

int IsFull(struct queue_t* queue){
    return queue->size == queue->capacity;
}

int IsEmpty(struct queue_t* queue){
    return queue->size == 0;
}

int Push(struct queue_t* queue, process* data){
    // Check if queue is full
    if(IsFull(queue)){
        return 0;
    }else{ 
        queue->tail = ++(queue->tail) % queue->capacity;
        queue->arr[queue->tail] = *data;
        queue->size++;
        return 1;
    }
}

struct process_t Pop(struct queue_t* queue){
    if(!IsEmpty(queue)){
        struct process_t data = queue->arr[queue->head];
        queue->head = ++(queue->head) % queue->capacity;
        queue->size--;
        return data;
    }
}

void PrintQueue(struct queue_t *queue){
    int index = queue->head;
    for (int i = 0; i < queue->size; i++){
        // print name and current burst of process
        printf("[%c, %d], ", queue->arr[index].name, queue->arr[index].current_burst);
        index = ++index % queue->capacity;
    }
    printf("\n");
}

