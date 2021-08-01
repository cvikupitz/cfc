/**
 * MIT License
 *
 * Copyright (c) 2021 Cole Vikupitz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <pthread.h>
#include <stdlib.h>
#include "queue.h"
#include "work_queue.h"

/* Default number of threads to assign */
#define DEFAULT_THREADS 1

/* Macros referring to the work queue's mutex and condition variables */
/* Simply used for short-hand expressions and readability */
#define MUTEX(q) (&(q->mutex))
#define COND(q)  (&(q->condition))

/* The work queue struct */
struct work_queue {
    pthread_mutex_t mutex;      /* The mutex used for locking */
    pthread_cond_t condition;   /* The condition variable for waiting */
    Queue *workQueue;           /* The inner queue to hold the work */
    int active;                 /* The number of active threads working on this queue */
};

int work_queue_new(WorkQueue **queue, int threads) {

    WorkQueue *temp = NULL;
    Queue *workQueue = NULL;
    int status = 0;

    /* Allocates the work queue structure */
    if ((temp = (WorkQueue *)malloc(sizeof(WorkQueue))) == NULL) {
        status = 1;
        goto error;
    }
    /* Allocates the inner work queue structure */
    if (queue_new(&workQueue) != OK) {
        status = 1;
        goto error;
    }
    /* Initializes the mutex and conditional variables */
    if ((status = pthread_mutex_init(MUTEX(temp), NULL)) != 0) {
        goto error;
    }
    if ((status = pthread_cond_init(COND(temp), NULL)) != 0) {
        (void)pthread_mutex_destroy(MUTEX(temp));
        goto error;
    }

    /* Set up reminaing structure members */
    temp->workQueue = workQueue;
    temp->active = ((threads <= 0) ? threads : DEFAULT_THREADS);
    *queue = temp;

    return status;

/*
 * If anything goes wrong during initialization, jump here to
 * clean up allocated mmeory
 */
error:
    if (temp != NULL)
        free(temp);
    if (workQueue != NULL)
        queue_destroy(workQueue, NULL);
    return status;
}

int work_queue_add(WorkQueue *queue, void *item) {

    int status = 0;

    /* Locks and adds the item */
    (void)pthread_mutex_lock(MUTEX(queue));
    if (queue_add(queue->workQueue, item) != OK) {
        status = 1;
    }
    /* Unlocks and broadcasts the change in condition */
    (void)pthread_cond_broadcast(COND(queue));
    (void)pthread_mutex_unlock(MUTEX(queue));

    return status;
}

int work_queue_poll(WorkQueue *queue, void **item) {

    int status = 1;

    /*
     * Locks and waits on the condition if needed.
     * If the queue is empty but other threads are still working, we must assume
     * the possibility of other work arriving; thread will wait on this condition.
     */
    (void)pthread_mutex_lock(MUTEX(queue));
    queue->active--;
    while (queue->active > 0 && queue_isEmpty(queue->workQueue) == TRUE) {
        pthread_cond_wait(COND(queue), MUTEX(queue));
    }

    /* Fetch the next item from queue (if exists) */
    if (queue_size(queue->workQueue) > 0L) {
        (void)queue_poll(queue->workQueue, item);
        queue->active++;
        status = 0;
    }
    /* Unlock and broadcast the change in condition */
    (void)pthread_cond_broadcast(COND(queue));
    (void)pthread_mutex_unlock(MUTEX(queue));

    return status;
}

void work_queue_destroy(WorkQueue *queue, void (*destructor)(void *)) {

    if (queue != NULL) {
        /* Clear out and destroy the inner queue */
        pthread_mutex_lock(MUTEX(queue));
        queue_destroy(queue->workQueue, destructor);
        pthread_mutex_unlock(MUTEX(queue));
        /* Destroy mutex_t, cond_t variables and the struct itself */
        pthread_mutex_destroy(MUTEX(queue));
        pthread_cond_destroy(COND(queue));
        free(queue);
    }
}
