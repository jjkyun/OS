#include <stdio.h>
#include <stdlib.h>
#define STACK_CAPACITY 5
#define REF_CAPACITY 13

struct Stack{
    int top;
    int capacity;
    int* arr;
};

// Create Stack starting from 0
struct Stack* CreateStack(int capacity){
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->arr = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

// Return 1 if stack is full
int IsFull(struct Stack* stack){
    return stack->top == stack->capacity - 1;
}

// Return -1 if stack is empty
int IsEmpty(struct Stack* stack){
    return stack->top == -1;
}

// Push data to stack
void Push(struct Stack* stack, int data){
    // Check if stack is full
    if(IsFull(stack)){
        printf("Stack is Full\n");
    }
    stack->arr[++stack->top] = data;
    printf("Push: %d\n", data);
}

// Pop data from the top
int Pop(struct Stack* stack){
    if(IsEmpty(stack)){
        printf("Stack is Empty\n");
        return -1;
    }
    return stack->arr[stack->top--];
}

// Return index if hit
int FindElement(struct Stack* stack, int data){
    // Check if Hit 
    for(int i = 0; i < stack->top; i++){
        if(stack->arr[i] == data) 
            return i;
    }
    return -1;
}

void PrintStack(struct Stack* stack){
    for(int i = 0; i <= stack->top; i++){
        printf("[%d] ", stack->arr[i]);
    }
    printf("\n");
}

int main(){
    int hit_index = -1;
    int pop_data;
    int ref[REF_CAPACITY] = {4, 7, 0, 7, 1, 0, 1, 2, 1, 2, 7, 1, 2};
    struct Stack* stack = CreateStack(STACK_CAPACITY);
    
    // Push reference data
    for(int i = 0; i < REF_CAPACITY; i++){
        // If stack is empty then push
        if(IsEmpty(stack)){
            Push(stack, ref[i]);
        } else {
            // Check if there are same element in Stack
            hit_index = FindElement(stack, ref[i]);
            // No hit -> Push
            if(hit_index == -1){
                // If stack full then Pop, Push
                if(IsFull(stack)){
                    pop_data = Pop(stack);
                    Push(stack, ref[i]);
                } else {
                    Push(stack, ref[i]);
                }
            // Hit -> Move the hit element to top 
            } else {
                printf("Hit: %d\n", ref[i]);
                for(int j = hit_index; j < stack->top; j++){
                    stack->arr[j] = stack->arr[j + 1];
                }
                stack->arr[stack->top] = ref[i];
            }
        }   
        PrintStack(stack);
    }
    return 0;
}
