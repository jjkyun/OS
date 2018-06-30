#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mlfq.h"

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_RESET    "\x1b[0m"

extern struct cpu_t cpu;

void InitCPU(){
    cpu.current = NULL;
    cpu.burst_left = 0;
}

void PrintCPU(){
    // if cpu have process going on
    if(cpu.current){
        printf("current process: [%c, %d]\n", 
                cpu.current->name, cpu.current->current_burst);
        
        // print process that CPU is working on
        for(int i = 0; i < cpu.current->current_burst; i++){
            printf(ANSI_COLOR_YELLOW "|" ANSI_COLOR_RESET);
        }
        for(int i = 0; i < cpu.current->initial_burst 
                - cpu.current->current_burst; i++){
            printf("|");
        }
        printf("\n");
        for(int i = 0; i < cpu.burst_left; i++){
            printf(ANSI_COLOR_RED "|" ANSI_COLOR_RESET);
        }
    } else {
        printf("current process: empty\n");
    }
    printf("\n");
}
