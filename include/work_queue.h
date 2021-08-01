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

#ifndef _WORK_QUEUE_H__
#define _WORK_QUEUE_H__

/**
 * Declared interface for the concurrent work queue ADT.
 *
 * Designed to be a thread safe queue containing work items to be processed by
 * some number of threads.
 */
typedef struct work_queue WorkQueue;

/**
 * Creates a new instance of WorkQueue, then stores the new instance into the
 * address of '*queue'.
 *
 * Params:
 *    queue - The pointer address to store the new work queue instance.
 *    threads - The numberof threads expected to access this work queue.
 * Returns:
 *    0 if successful.
 *    1 if allocation fails.
 *    The int returned from the failed call to 'pthread_mutex_init()' and
 *    'pthread_cond_init()' will be returned if either of those calls fail.
 */
int work_queue_new(WorkQueue **queue, int threads);

/**
 * Adds the specified item 'item' into the work queue.
 *
 * Params:
 *    queue - The work queue to operate on.
 *    item - The item to add.
 * Returns:
 *    0 if successful.
 *    1 if failed (allocation failed).
 */
int work_queue_add(WorkQueue *queue, void *item);

/**
 * Removes the next item from the queue, then stores the result into '*item'. If
 * a thread tries to dequeue an item while other threads are 'working', then the
 * thread will wait until either a new item is added, or all remaining threads
 * are waiting for work.
 *
 * Params:
 *    queue - The work queue to operate on.
 *    item - The pointer address to store the removed item into.
 * Returns:
 *    0 if successful.
 *    1 if failed (queue was empty & all threads are waiting).
 */
int work_queue_poll(WorkQueue *queue, void **item);

/**
 * Destroys the work queue instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the queue is destroyed.
 *
 * Params:
 *    queue - The work queue to operate on.
 *    destructor - Function to operate on each element prior to work queue
 *                 destruction.
 * Returns:
 *    None
 */
void work_queue_destroy(WorkQueue *queue, void (*destructor)(void *));

#endif  /* _WORK_QUEUE_H__ */
