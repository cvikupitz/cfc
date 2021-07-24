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
#include "linkedlist.h"

/*
 * Struct for a node inside the linked list.
 */
typedef struct node {
    struct node *next;  /* Pointer to the next node */
    struct node *prev;  /* Pointer to the previous node */
    void *data;         /* Pointer to hold the element */
} Node;

/*
 * Struct for the linked list ADT.
 */
struct linkedlist {
    Node head;          /* Sentinel node for the list's head */
    Node tail;          /* Sentinel node for the list's tail */
    long size;          /* The linked list's current size */
};

/* Macro that provides the address of the head sentinel node */
#define HEADER(x)  (&((x)->head))
/* Macro that provides the address of the tail sentinel node */
#define TRAILER(x) (&((x)->tail))

Status linkedlist_new(LinkedList **list) {

    /* Allocates the struct, checks for allocation failure */
    LinkedList *temp = (LinkedList *)malloc(sizeof(LinkedList));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initializes the remaining struct members */
    HEADER(temp)->data = NULL;
    HEADER(temp)->next = TRAILER(temp);
    HEADER(temp)->prev = NULL;
    TRAILER(temp)->data = NULL;
    TRAILER(temp)->next = NULL;
    TRAILER(temp)->prev = HEADER(temp);
    temp->size = 0L;
    *list = temp;

    return OK;
}

/* Macro used to validate the given index i */
#define INDEX_VALID(i,N) ( ( 0L <= (i) && (i) < (N) ) ? TRUE : FALSE )
/* Macro to check if the list is currently empty */
#define IS_EMPTY(x)  ( ((x)->size == 0L) ? TRUE : FALSE )

/*
 * Fetches the node from list at the specified index.
 */
static Node *fetchNode(LinkedList *list, long index) {

    Node *temp;
    long i;
    long mid = ( list->size / 2 );  /* Middle index in list */

    /* If the index is closer to the head, traverse from the head */
    /* Otherwise, traverse from tail for faster access */
    if (index <= mid) {
        /* Traverses forward from the head to the node */
        temp = HEADER(list)->next;
        for (i = 0L; i < index; i++)
            temp = temp->next;
    } else {
        /* Traverses backwards from the tail to node */
        temp = TRAILER(list)->prev;
        for (i = list->size - 1; i > index; i--)
            temp = temp->prev;
    }

    return temp;
}

/*
 * Links up the specified node 'node' with 'prev' and 'next' such that
 * the node linkage will become:
 *     prev <------> node <-----> next
 */
static void linkNodes(Node *node, Node *prev, Node *next) {
    node->prev = prev;
    node->next = next;
    next->prev = node;
    prev->next = node;
}

/*
 * Unlinks the node 'node' from the list. The previous and next nodes
 * will be linked up removing the node.
 */
static void unlinkNode(Node *node) {
    Node *next = node->next;
    Node *prev = node->prev;
    next->prev = prev;
    prev->next = next;
}

Status linkedlist_addFirst(LinkedList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Inserts the node into the front of the list */
    node->data = item;
    linkNodes(node, HEADER(list), HEADER(list)->next);
    list->size++;

    return OK;
}

Status linkedlist_addLast(LinkedList *list, void *item) {

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Inserts the node into the tail of the list */
    node->data = item;
    linkNodes(node, TRAILER(list)->prev, TRAILER(list));
    list->size++;

    return OK;
}

Status linkedlist_insert(LinkedList *list, long i, void *item) {

    /* Check if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    if (i == 0)         /* Same operation as addFirst() */
        return linkedlist_addFirst(list, item);
    if (i == list->size)        /* Same operation as addLast() */
        return linkedlist_addLast(list, item);

    /* Allocates the node for insertion */
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL)
        return ALLOC_FAILURE;

    /* Fetch the node at the current index, relink for insertion */
    node->data = item;
    Node *temp = fetchNode(list, i);
    linkNodes(node, temp->prev, temp);
    list->size++;

    return OK;
}

Status linkedlist_first(LinkedList *list, void **first) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the first item, stores into pointer */
    *first = HEADER(list)->next->data;

    return OK;
}

Status linkedlist_last(LinkedList *list, void **last) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Fetches the last item, stores into pointer */
    *last = TRAILER(list)->prev->data;

    return OK;
}

Status linkedlist_get(LinkedList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Fetches the item at the index, stores into pointer */
    Node *temp = fetchNode(list, i);
    *item = temp->data;

    return OK;
}

Status linkedlist_set(LinkedList *list, long i, void *item, void **previous) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    /* Retrieve the node, swaps with new element */
    Node *temp = fetchNode(list, i);
    *previous = temp->data;
    temp->data = item;

    return OK;
}

Status linkedlist_removeFirst(LinkedList *list, void **first) {

    /* Check if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the first node, unlinks from list */
    Node *temp = HEADER(list)->next;
    *first = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return OK;
}

Status linkedlist_removeLast(LinkedList *list, void **last) {

    /* Check if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Fetches the last node, unlinks from list */
    Node *temp = TRAILER(list)->prev;
    *last = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return OK;
}

Status linkedlist_remove(LinkedList *list, long i, void **item) {

    /* Checks if the list is empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;
    /* Checks if the index is valid */
    if (INDEX_VALID(i, list->size) == FALSE)
        return INVALID_INDEX;

    if (i == 0L)        /* Same operation as removeFirst() */
        return linkedlist_removeFirst(list, item);
    if (i == list->size - 1)    /* Same operation as removeLast() */
        return linkedlist_removeLast(list, item);

    /* Fetch the node to be removed, unlink from list */
    Node *temp = fetchNode(list, i);
    *item = temp->data;
    unlinkNode(temp);
    free(temp);
    list->size--;

    return OK;
}

/*
 * Clears out the list of all its elements. Frees up all reserved memory
 * back to the heap.
 */
static void clearList(LinkedList *list, void (*destructor)(void *)) {

    Node *curr = HEADER(list)->next, *next = NULL;
    long i;

    for (i = 0L ; i < list->size; i++) {
        next = curr->next;
        if (destructor != NULL)
            (*destructor)(curr->data);
        free(curr);
        curr = next;
    }
}

void linkedlist_clear(LinkedList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    HEADER(list)->next = TRAILER(list);
    TRAILER(list)->prev = HEADER(list);
    list->size = 0L;
}

long linkedlist_size(LinkedList *list) {
    return list->size;
}

Boolean linkedlist_isEmpty(LinkedList *list) {
    return IS_EMPTY(list);
}

/*
 * Generates and returns an array representation of the linked list.
 */
static void **generateArray(LinkedList *list) {

    Node *temp = NULL;
    long i = 0L;
    size_t bytes;
    void **items = NULL;

    /* Allocates memory for the array */
    bytes = ( list->size * sizeof(void *) );
    items = (void **)malloc(bytes);
    if (items == NULL)
        return NULL;

    /* Populates the array with linked list items */
    for (temp = HEADER(list)->next; temp != TRAILER(list); temp = temp->next)
        items[i++] = temp->data;

    return items;
}

Status linkedlist_toArray(LinkedList *list, Array **array) {

    /* Does not create the array if empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Generate the array of linked list items */
    void **items = generateArray(list);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Allocate memory for the array struct */
    Array *temp = (Array *)malloc(sizeof(Array));
    if (temp == NULL) {
        free(items);
        return ALLOC_FAILURE;
    }

    /* Initialize remaining struct members */
    temp->items = items;
    temp->len = list->size;
    *array = temp;

    return OK;
}

Status linkedlist_iterator(LinkedList *list, Iterator **iter) {

    Iterator *temp = NULL;

    /* Does not create the array if empty */
    if (IS_EMPTY(list) == TRUE)
        return STRUCT_EMPTY;

    /* Generates the array of items for iterator */
    void **items = generateArray(list);
    if (items == NULL)
        return ALLOC_FAILURE;

    /* Creates a new iterator with the items */
    Status status = iterator_new(&temp, items, list->size);
    if (status != OK) {
        free(items);
        return status;
    }
    *iter = temp;

    return OK;
}

void linkedlist_destroy(LinkedList *list, void (*destructor)(void *)) {
    clearList(list, destructor);
    free(list);
}
