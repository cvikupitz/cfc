/**
 * MIT License
 *
 * Copyright (c) 2020 Cole Vikupitz
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

#include <stdlib.h>
#include "ts_iterator.h"

/*
 * Struct for the thread-safe iterator.
 */
struct ts_iterator {
    pthread_mutex_t *lock;      /* The lock */
    void **items;               /* Array of iterable elements */
    long next;                  /* Index that points to next item in iteration */
    long len;                   /* Total number of elements */
};

Status ts_iterator_new(ConcurrentIterator **iter, pthread_mutex_t *lock, void **items, long len) {

    /* Allocates memory for the new iterator */
    ConcurrentIterator *temp = (ConcurrentIterator *)malloc(sizeof(ConcurrentIterator));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    temp->lock = lock;
    temp->items = items;
    temp->next = 0L;
    temp->len = len;
    *iter = temp;

    return OK;
}

Boolean ts_iterator_hasNext(ConcurrentIterator *iter) {
    return ( iter->next < iter->len ) ? TRUE : FALSE;
}

Status ts_iterator_next(ConcurrentIterator *iter, void **next) {

    /* Iteration already complete, return with status */
    if (iter->next == iter->len)
        return ITER_END;
    /* Advances to next item in iteration */
    *next = iter->items[iter->next++];

    return OK;
}

void ts_iterator_destroy(ConcurrentIterator *iter) {
    free(iter->items);
    pthread_mutex_unlock(iter->lock);
    free(iter);
}
