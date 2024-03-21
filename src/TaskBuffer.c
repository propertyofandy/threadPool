#include "../include/TaskBuffer.h"

// creates and returns an instance of TaskBuffer
TaskBuffer newTaskBuffer(size_t capacity){
    return (TaskBuffer){
        .data_points = (Task*)malloc(sizeof(Task)*capacity),
        .capacity = capacity,
        .first_item = 0,
        .last_item = 0,
        .mutex = PTHREAD_MUTEX_INITIALIZER,
        .empty = PTHREAD_COND_INITIALIZER,
        .full = PTHREAD_COND_INITIALIZER
    };
}

// free and desroy all associated Task
void deleteTaskBuffer(TaskBuffer *buffer){
    free(buffer->data_points);
    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->empty);
    pthread_cond_destroy(&buffer->full);
}

// retrieves a task from the buffer and updates index 
Task getTask(TaskBuffer *buffer){
    
    //get item
    Task task = buffer->data_points[buffer->first_item]; 
    buffer->first_item = (buffer->first_item+1) % buffer->capacity;
    
    pthread_cond_signal(&buffer->full);
    return task;
}

//submit a taks as a Task member to the buffer
void submitTask(TaskBuffer *buffer, Task task){

    pthread_mutex_lock(&buffer->mutex); // lock to avoid race conditions 
    
    // while buffer is full we wait until a signal on full
    while( (buffer->last_item+1)% buffer->capacity == buffer->first_item) {
        //printf("wait full\n");
        pthread_cond_wait(&buffer->full, &buffer->mutex);
        //printf("signal full\n");
    }

    buffer->data_points[buffer->last_item] = task; // add Task

    buffer->last_item = 
        (buffer->last_item+1) % buffer->capacity;// generate next index position

    pthread_mutex_unlock(&buffer->mutex); // free lock
    
    pthread_cond_signal(&buffer->empty); // signal buffer condition 
}

void executeTask(Task task){
    task.function(task.a, task.b);
}