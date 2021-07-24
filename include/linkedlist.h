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

#ifndef _CDS_LINKEDLIST_H__
#define _CDS_LINKEDLIST_H__

#include "cds_common.h"
#include "iterator.h"

/**
 * Interface for the LinkedList ADT.
 *
 * Doubly-linked list implementation. Implements all optional list operations, and
 * permits all elements (including null).
 *
 * Modeled after the Java 7 LinkedList interface.
 */
typedef struct linkedlist LinkedList;

/**
 * Constructs a new linked list instance, then stores the new instance into '*list'.
 *
 * Params:
 *    None
 * Returns:
 *    OK - LinkedList was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_new(LinkedList **list);

/**
 * Inserts the specified element at the beginning of the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 *    item - The element to add.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_addFirst(LinkedList *list, void *item);

/**
 * Appends the specified element to the end of the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 *    item - The element to add.
 * Returns:
 *    OK - Operation was successful.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_addLast(LinkedList *list, void *item);

/**
 * Inserts the specified element at the specified position in this list. Shifts the
 * element currently at that position (if any) and any subsequent elements to the
 * right (adds one to their indices).
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index at which the specified element is to be inserted.
 *    item - The element to be inserted.
 * Returns:
 *    OK - Operation was successful.
 *    INVALID_INDEX - Index given is out of range (index < 0 || index > size()).
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_insert(LinkedList *list, long i, void *item);

/**
 * Retrieves the first element from the linked list without removing it, and stores
 * the element into '*first'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    first - The pointer address to store the first element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 */
Status linkedlist_first(LinkedList *list, void **first);

/**
 * Retrieves the last element from the linked list without removing it, and stores
 * the element into '*last'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    last - The pointer address to store the last element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 */
Status linkedlist_last(LinkedList *list, void **last);

/**
 * Retrieves the element at the specified index from the linked list without removing
 * it, and stores the element into '*item'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to return.
 *    item - The pointer address to store the fetched element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 *    INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status linkedlist_get(LinkedList *list, long i, void **item);

/**
 * Replaces the element at the specified position in the linked list with the specified
 * element. The old element is then placed into '*previous'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to replace
 *    item - The element to be stored at the specified position
 *    previous - The pointer address to store the element previously at the specified
 *               position.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 *    INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status linkedlist_set(LinkedList *list, long i, void *item, void **previous);

/**
 * Removes the first element from the linked list, and stores the result into '*first'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    first - The pointer address to store the removed element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 */
Status linkedlist_removeFirst(LinkedList *list, void **first);

/**
 * Removes the last element from the linked list, and stores the result into '*last'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    last - The pointer address to store the removed element into.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 */
Status linkedlist_removeLast(LinkedList *list, void **last);

/**
 * Removes the element at the specified position in the linked list. Shifts any
 * subsequent elements to the left (subtracts one from their indices). Stores the
 * removed element into '*item'.
 *
 * Params:
 *    list - The linked list to operate on.
 *    i - The index of the element to remove.
 *    item - The pointer address to store the element previously at the specified position.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 *    INVALID_INDEX - Index given is out of range (index < 0 || index >= size()).
 */
Status linkedlist_remove(LinkedList *list, long i, void **item);

/**
 * Removes all elements from the linked list. If 'destructor' is not NULL, it will be
 * invoked on each element in the linked list after being removed.
 *
 * Params:
 *    list - The linked list to operate on.
 *    destructor - Function to operate on each element after removal.
 * Returns:
 *    None
 */
void linkedlist_clear(LinkedList *list, void (*destructor)(void *));

/**
 * Returns the number of elements in the linked list.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    The linked list's current size.
 */
long linkedlist_size(LinkedList *list);

/**
 * Returns TRUE if the linked list contains no elements, FALSE if otherwise.
 *
 * Params:
 *    list - The linked list to operate on.
 * Returns:
 *    TRUE if the linked list is empty, FALSE if not.
 */
Boolean linkedlist_isEmpty(LinkedList *list);

/**
 * Allocates and generates an array containing all of the elements in the linked list in
 * proper sequence (from first to last element), then stores the array into '*array'. Caller
 * is responsible for freeing the array when finished.
 *
 * Params:
 *    list - The linked list to operate on.
 *    array - Address where the new array will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_toArray(LinkedList *list, Array **array);

/**
 * Createss an Iterator instance to iterate over the linked list's elements in proper
 * sequence (from first to last element), then stores the iterator into '*iter'. Caller
 * is responsible for destroying the iterator instance when finished.
 *
 * Params:
 *    list - The linked list to operate on.
 *    iter - Address where the new iterator will be stored.
 * Returns:
 *    OK - Operation was successful.
 *    STRUCT_EMPTY - Linked list is currently empty.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status linkedlist_iterator(LinkedList *list, Iterator **iter);

/**
 * Destroys the linked list instance by freeing all of its reserved memory. If 'destructor'
 * is not NULL, it will be invoked on each element before the linked list is destroyed.
 *
 * Params:
 *    list - The linked list to destroy.
 *    destructor - Function to operate on each element prior to linked list destruction.
 * Returns:
 *    None
 */
void linkedlist_destroy(LinkedList *list, void (*destructor)(void *));

#endif  /* _CDS_LINKEDLIST_H__ */
