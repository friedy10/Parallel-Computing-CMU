#include <stdio.h>
#include <pthread.h>

#include "CycleTimer.h"

typedef struct {
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int* output;
    int threadId;
    int numThreads;
    int startRow;
} WorkerArgs;


extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int maxIterations,
    int output[]);


//
// workerThreadStart --
//
// Thread entrypoint.
void* workerThreadStart(void* threadArgs) {

    WorkerArgs* args = static_cast<WorkerArgs*>(threadArgs);

    // TODO: Implement worker thread here.
    printf("Hello world from thread %d\n", args->threadId);
    mandelbrotSerial(args->x0, args->y0, args->x1, args->y1, args->width,
    800, args->startRow, args->height, args->maxIterations, args->output);
   

    return NULL;
}

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Multi-threading performed via pthreads.
void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    const static int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    pthread_t workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    int splitHeight = height/numThreads;
    int start = 0;
    for (int i=0; i<numThreads; i++) {
        // TODO: Set thread arguments here.
        args[i].x0 = x0;
        args[i].y0 = y0;
        args[i].x1 = x1;
        args[i].y1 = y1;
        args[i].maxIterations = maxIterations;
        args[i].threadId = i;
        args[i].startRow = start;
        args[i].height = splitHeight;
        args[i].width = width;
        args[i].output = new int[width * 800];

        start+=splitHeight;
    }
    if(start == height){
        printf("Its working\n");
        printf("The height is %d\n", height);
        printf("The split height is %d\n", splitHeight);
    }
    
    //printf("Running junk thing\n");
    //mandelbrotSerial(x0,y0,x1,y1,width,height,0,400, maxIterations, output);
    //printf("Some thing fucked up!\n");

    // Fire up the worker threads.  Note that numThreads-1 pthreads
    // are created and the main app thread is used as a worker as
    // well.
    
    for (int i=1; i<numThreads; i++)
        pthread_create(&workers[i], NULL, workerThreadStart, &args[i]);


    workerThreadStart(&args[0]);

    // wait for worker threads to complete

    for (int i=1; i<numThreads; i++)
        pthread_join(workers[i], NULL);

    for(int i = 0; i < (args[0].width*800); i++){
        output[i] = args[0].output[i];
    }
    for(int i = width*splitHeight; i < (width*800); i++){
        output[i] = args[1].output[i];
    }
    
    //memcpy(output, args[0].output, sizeof(args[0].output));
}
