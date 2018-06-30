// Header Guard
#ifndef MLFQ_H
#define MLFQ_H
typedef struct process_t process;

#define MLFQ_LEVEL 3 
#define MLFQ_CAPACITY 10

/* About queue */
struct queue_t{
    // Changed int arr to process arr because of mlfq
    process* arr;
    int capacity;
    int size;
    int head;
    int tail;
};

struct queue_t* CreateQueue(int capacity);
void DeleteQueue(struct queue_t* queue);
int IsEmpty(struct queue_t* queue);
int IsFull(struct queue_t* queue);
int Push(struct queue_t* queue, process* data);
process Pop(struct queue_t* queue);
void PrintQueue(struct queue_t* queue);


/* About mlfq */
enum MLFQ_TYPE{
    ROUND_ROBIN,
    FCFS
};

struct mlfq_t{
    enum MLFQ_TYPE type;
    struct queue_t *queue;
    int quantum;
};

void InitMLFQ();
struct mlfq_t* CreateMLFQ(int capacity, enum MLFQ_TYPE type, int quantum);
void PrintMLFQ();


/* About Process */
// process type which is element of Queue
struct process_t{
    char name;
    int initial_burst;
    int current_burst;
    int mlfq_level;
};

char GenerateNameRandomly();
void GenerateProcessRandomly();
void FindNextProcess();
void HandleProcess();
void DeleteProcess();

/* About CPU */
// [process name, burst]:: (ex) [a, 20], [b, 34]
struct cpu_t{
    struct process_t *current;
    int burst_left;
};

void InitCPU();
void PrintCPU();

#endif /* MLFQ_H */
