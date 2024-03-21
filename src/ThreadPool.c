#include "../include/ThreadPool.h"


int bufferIsEmpty(TaskBuffer *buffer){
    return buffer->first_item == buffer->last_item;
}

void* startThread(void *args){

    //displaying thread count
    static int thread_number = 0;
    
    printf("\nthread: %d created successfully\n", ++thread_number);
    TaskBuffer *buffer = (TaskBuffer*)args; // cast args to DataBuffer

    while(1){

        //if(bufferIsEmpty(buffer)) return NULL; 
        pthread_mutex_lock(&buffer->mutex); // lock DataBuffer

        while(bufferIsEmpty(buffer)){ // if empty wait and unlock
            printf("wait empty\n");
            pthread_cond_wait(&buffer->empty, &buffer->mutex);
            printf("signal empty\n");
        }
        
        Task data = getTask(buffer); // get copy of data to execute

        pthread_mutex_unlock(&buffer->mutex); // free mutex

        executeTask(data); // exicute task after no longer associated with buffer
    }
} 

void initThreadPool(size_t number_of_threads, TaskBuffer *buffer){

    // dynamic array allocation of threads
    pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t)*number_of_threads);

    for (size_t i = 0; i < number_of_threads; i++) //create threads
    {
        if(pthread_create(&threads[i], NULL, &startThread, (void*)buffer) != 0){
            perror("failed to create thread");
        }
    }

    for (size_t i = 0; i < number_of_threads; i++) // join threads
    {
        if(pthread_join(threads[i], NULL) != 0){
            perror("failed to join threads");
        }
    }

    printf("\nthreads joined successfully\n");

    free(threads); 
}