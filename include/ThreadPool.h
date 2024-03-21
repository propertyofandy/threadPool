#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include "./TaskBuffer.h"

//prototype
void initThreadPool(size_t number_of_threads, TaskBuffer *buffer);

#endif