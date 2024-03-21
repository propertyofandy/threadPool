#ifndef DATABUFFER_H_
#define DATABUFFER_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
// Data will contain the necessary data 
// and the function that will operate on data
typedef struct Task {
    void (*function)(int, int);
    int a, b; 
} Task;

// DataBuffer implamentation is designed as a circular array
// such that modular arithmatic is used to keep indexes with in 
// bounds along with that a given DataBuffer will maintain a
// mutex and two condition variables that allow thread safe access 
// to the structure
typedef struct TaskBuffer {
    Task *data_points;
    pthread_mutex_t mutex;
    pthread_cond_t  empty, full;
    size_t  capacity, 
            first_item, 
            last_item;

} TaskBuffer;  

//prototypes 
TaskBuffer newTaskBuffer(size_t capacity);

void deleteTaskBuffer(TaskBuffer *buffer);

Task getTask(TaskBuffer *buffer);

void submitTask(TaskBuffer *buffer, Task task);

void executeTask(Task task);

#endif