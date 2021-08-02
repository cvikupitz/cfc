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

#include <pthread.h>
#include <stdlib.h>
#include "treeset.h"
#include "ts_treeset.h"

/*
 * Struct for the thread-safe treeset.
 */
struct ts_treeset {
    pthread_mutex_t lock;       /* The lock */
    TreeSet *instance;          /* Internal instance of TreeSet */
};

/* Macro used for locking the tree */
#define LOCK(x)    pthread_mutex_lock( &((x)->lock) )
/* Macro used for unlocking the tree */
#define UNLOCK(x)  pthread_mutex_unlock( &((x)->lock) )

Status ts_treeset_new(ConcurrentTreeSet **tree, int (*comparator)(void *, void *)) {

    ConcurrentTreeSet *temp;
    Status status;
    pthread_mutexattr_t attr;

    /* Allocates memory for the treeset */
    temp = (ConcurrentTreeSet *)malloc(sizeof(ConcurrentTreeSet));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Creates the internal treeset instance */
    status = treeset_new(&(temp->instance), comparator);
    if (status != OK) {
        free(temp);
        return status;
    }

    /* Creates the pthread_mutex for locking */
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&(temp->lock), &attr);
    pthread_mutexattr_destroy(&attr);
    *tree = temp;

    return OK;
}

void ts_treeset_lock(ConcurrentTreeSet *tree) {
    LOCK(tree);
}

void ts_treeset_unlock(ConcurrentTreeSet *tree) {
    UNLOCK(tree);
}

Status ts_treeset_add(ConcurrentTreeSet *tree, void *item) {

    LOCK(tree);
    Status status = treeset_add(tree->instance, item);
    UNLOCK(tree);

    return status;
}

Boolean ts_treeset_contains(ConcurrentTreeSet *tree, void *item) {

    LOCK(tree);
    Boolean contains = treeset_contains(tree->instance, item);
    UNLOCK(tree);

    return contains;
}

Status ts_treeset_first(ConcurrentTreeSet *tree, void **first) {

    LOCK(tree);
    Status status = treeset_first(tree->instance, first);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_last(ConcurrentTreeSet *tree, void **last) {

    LOCK(tree);
    Status status = treeset_last(tree->instance, last);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_floor(ConcurrentTreeSet *tree, void *item, void **floor) {

    LOCK(tree);
    Status status = treeset_floor(tree->instance, item, floor);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_ceiling(ConcurrentTreeSet *tree, void *item, void **ceiling) {

    LOCK(tree);
    Status status = treeset_ceiling(tree->instance, item, ceiling);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_lower(ConcurrentTreeSet *tree, void *item, void **lower) {

    LOCK(tree);
    Status status = treeset_lower(tree->instance, item, lower);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_higher(ConcurrentTreeSet *tree, void *item, void **higher) {

    LOCK(tree);
    Status status = treeset_higher(tree->instance, item, higher);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_pollFirst(ConcurrentTreeSet *tree, void **first) {

    LOCK(tree);
    Status status = treeset_pollFirst(tree->instance, first);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_pollLast(ConcurrentTreeSet *tree, void **last) {

    LOCK(tree);
    Status status = treeset_pollLast(tree->instance, last);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_remove(ConcurrentTreeSet *tree, void *item, void (*destructor)(void *)) {

    LOCK(tree);
    Status status = treeset_remove(tree->instance, item, destructor);
    UNLOCK(tree);

    return status;
}

void ts_treeset_clear(ConcurrentTreeSet *tree, void (*destructor)(void *)) {

    LOCK(tree);
    treeset_clear(tree->instance, destructor);
    UNLOCK(tree);
}

long ts_treeset_size(ConcurrentTreeSet *tree) {

    LOCK(tree);
    long size = treeset_size(tree->instance);
    UNLOCK(tree);

    return size;
}

Boolean ts_treeset_isEmpty(ConcurrentTreeSet *tree) {

    LOCK(tree);
    Boolean isEmpty = treeset_isEmpty(tree->instance);
    UNLOCK(tree);

    return isEmpty;
}

Status ts_treeset_toArray(ConcurrentTreeSet *tree, Array **array) {

    LOCK(tree);
    Status status = treeset_toArray(tree->instance, array);
    UNLOCK(tree);

    return status;
}

Status ts_treeset_iterator(ConcurrentTreeSet *tree, ConcurrentIterator **iter) {

    Array *array;
    Status status;

    /* Creates the array of items and locks it */
    LOCK(tree);
    status = treeset_toArray(tree->instance, &array);
    if (status != OK) {
        UNLOCK(tree);
        return status;
    }

    /* Creates the iterator */
    status = ts_iterator_new(iter, &(tree->lock), array->items, array->len);
    if (status != OK) {
        FREE_ARRAY(array);
        UNLOCK(tree);
    } else {
        free(array);
    }

    return status;
}

void ts_treeset_destroy(ConcurrentTreeSet *tree, void (*destructor)(void *)) {

    LOCK(tree);
    treeset_destroy(tree->instance, destructor);
    UNLOCK(tree);
    pthread_mutex_destroy(&(tree->lock));
    free(tree);
}
