#include <stdio.h>
#include <stdlib.h>
#define CAPACITY 3

struct frame_t{
    int* arr;
    int capacity;
    int size;
    int head;
    int tail;
};

struct frame_t* CreateFrame(int capacity){
    if(capacity > 1){
        struct frame_t* frame = (struct frame_t*)malloc(sizeof(struct frame_t));
        frame->capacity = capacity;
        frame->arr = (int *)malloc(sizeof(int) * frame->capacity);
        frame->size = 0;
        frame->head = 1;
        frame->tail = 0;
        
        return frame;
     } 
     else{
        return NULL;
     }
}

int IsFull(struct frame_t* frame){
    return frame->size == frame->capacity;
}

int IsEmpty(struct frame_t* frame){
    return frame->size == 0;
}

int Push(struct frame_t* frame, int data){
    if(IsFull(frame)){
        return 0;
    }   
    else{
        frame->tail = ++(frame->tail) % frame->capacity;
        frame->arr[frame->tail] = data;
        frame->size++;
        return 1;
    }
}

int Pop(struct frame_t* frame){
    if(!IsEmpty(frame)){
        int temp = frame->arr[frame->head];
        frame->head = ++(frame->head) % frame->capacity;
        frame->size--;
        return temp;
    }
}

void PrintFrame(struct frame_t* frame){
    for (int i = 0; i < frame->capacity; i++){
        printf("%d ", frame->arr[(i+1)%frame->capacity]);
    }
    printf("\n");
}


int FindElement(struct frame_t* frame, int data){
    for(int j = 0; j < frame->size; j++){
        if(frame->arr[j] == data){
            return 1;
        }
    }
    return 0;
}
int main(){
    
    int arr[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int pop_data;
    struct frame_t* frame;
    frame = CreateFrame(CAPACITY);
    
    // Insert data
    for(int i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
        //  Compare if the data is already existed
        if(FindElement(frame, arr[i])){
            printf("Hit\n");    
        } else{
            // if not, then Push the data
            pop_data = Pop(frame);
            Push(frame, arr[i]);
        }
    PrintFrame(frame);
    }
}
