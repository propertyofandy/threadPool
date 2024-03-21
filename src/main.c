#include "../include/ThreadPool.h"

#define BUFFER_SIZE 500
#define THREAD_NUMBER 8


void sum(int a, int b){
    printf("sum(%d, %d) = %d\n", a, b, a+b);
}

void product(int a, int b){
    printf("product(%d, %d) = %d\n", a, b, a*b);
}

void *addBufferItems(void *args){

    srand(time(NULL)); // initilize random numbers

    TaskBuffer *buffer = (TaskBuffer *)args; // cast back to buffer

    while(1){ // add item indefanently
        Task d = {
            .a = rand() % 50,
            .b = rand() % 50
        };

        if(rand()%2 == 0) d.function = &sum;
        else d.function = &product;

        submitTask(buffer, d);
        sleep(1);
    }
}

// fill the buffer
void fillBuffer(TaskBuffer *buffer){

    srand(time(NULL));
    for (size_t i = 0; i < BUFFER_SIZE-1; i++)
    {
        Task d = {
            .a = rand() % 50,
            .b = rand() % 50
        };

        if(rand()%2 == 0) d.function = &sum;
        else d.function = &product;

        submitTask(buffer, d);
    }
    
}

int main(){

    clock_t start = clock();

    TaskBuffer buffer = newTaskBuffer(BUFFER_SIZE); // create data buffer
    
    fillBuffer(&buffer);
    
    pthread_t input; // create thread to add items to buffer
    pthread_create(&input, NULL, &addBufferItems, (void*)&buffer); 
    
    initThreadPool(THREAD_NUMBER,&buffer); 
    
    deleteTaskBuffer(&buffer);

    clock_t end = clock() - start;

    printf("\ncompleted in %ld ticks\n", end);

    return 0;
}
