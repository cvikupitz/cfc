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
#include "queue.h"

/*
 * Struct for a node inside the queue.
 */
typedef struct node {
    struct node *next;  /* Points to the next node */
    void *data;         /* Pointer that holds the element */
} Node;

/*
 * The struct for the queue ADT.
 */
struct queue {
    Node *head;         /* Pointer to the queue's head */
    Node *tail;         /* Pointer to the queue's tail */
    long size;          /* The queue's current size */
};

Status queue_new(Queue **queue) {

    /* Allocate the struct, check for allocation failures */
    Queue *temp = (Queue *)malloc(sizeof(Queue));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    temp->head = NULL;
    temp->tail = NULL;
    temp->size = 0L;
    *queue = temp;

    return OK;
}

/* Macro to check if the queue is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

Status queue_add(Queue *queue, void *item) {

    /* Allocate the node for item insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;
    node->next = NULL;
    node->data = item;

    /* Edge case: queue is empty, link the front and rear nodes */
    if (IS_EMPTY(queue) == TRUE)
        queue->head = node;
    /* Otherwise, links to the rear node */
    else
        queue->tail->next = node;
    queue->tail = node;
    queue->size++;

    return OK;
}

Status queue_peek(Queue *queue, void **first) {

    /* Checks of the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STRUCT_EMPTY;
    /* Extract the element, saves to pointer */
    *first = queue->head->data;

    return OK;
}

Status queue_poll(Queue *queue, void **first) {

    /* Checks if the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STRUCT_EMPTY;

    /* Unlink the head node from the queue */
    Node *temp = queue->head;
    queue->head = temp->next;
    queue->size--;
    /* Edge case: one item, nullify both head and tail pointers */
    if (IS_EMPTY(queue) == TRUE) {
        queue->head = NULL;
        queue->tail = NULL;
    }
    /* Free the allocated node's struct */
    *first = temp->data;
    free(temp);

    return OK;
}

/*
 * Clears out the queue of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearQueue(Queue *queue, void (*destructor)(void *)) {

    Node *curr = queue->head, *next = NULL;
    long i;

    for (i = 0L; i < queue->size; i++) {
        next = curr->next;
        /* Frees the allocated memory */
        if (destructor != NULL)
            (*destructor)(curr->data);
        free(curr);
        curr = next;
    }
}

void queue_clear(Queue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0L;
}

long queue_size(Queue *queue) {
    return queue->size;
}

Boolean queue_isEmpty(Queue *queue) {
    return IS_EMPTY(queue);
}

/*
 * Generates and returns an array representation of the queue.
 */
static void **generateArray(Queue *queue) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( queue->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with the queue items */
    for (temp = queue->head; temp != NULL; temp = temp->next)
        items[i++] = temp->data;

    return items;
}

Status queue_toArray(Queue *queue, Array **array) {

    /* Checks if the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of queue items */
    void **items = generateArray(queue);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initialize the array struct members */
    temp->items = items;
    temp->len = queue->size;
    *array = temp;

    return OK;
}

Status queue_iterator(Queue *queue, Iterator **iter) {

    Iterator *temp = NULL;

    /* Checks if the queue is empty */
    if (IS_EMPTY(queue) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(queue);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, queue->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void queue_destroy(Queue *queue, void (*destructor)(void *)) {
    clearQueue(queue, destructor);
    free(queue);
}
