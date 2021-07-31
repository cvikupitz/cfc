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

#ifndef _CDS_TS_ITERATOR_H__
#define _CDS_TS_ITERATOR_H__

#include <pthread.h>
#include "cds_common.h"

/**
 * Interface for the thread-safe Iterator ADT.
 *
 * An iterator over a collection of elements.
 *
 * This iterator is returned from all of the iterator() methods and contains a shallow
 * copy of all the items in the structure it's iterating from; therefore, the items
 * returned from the iterator should be accessed in a read-only manner. The iterator
 * itself is a 'snapshot' iterator - the items contained in the iterator represent the
 * structure's current state. Any changes made to the structure after creating the
 * iterator will not be reflected.
 *
 * Modeled after the Java 7 Iterator interface.
 */
typedef struct ts_iterator ConcurrentIterator;

/**
 * Creates a new iterator instance for the given array of items, then assigns the new
 * iterator instance to '*iter'.
 *
 * Params:
 *    iter - The pointer address to store the new Iterator into.
 *    lock - The mutex that currently is locked on the ADT being iterated over.
 *    items - The array of items to iterate through.
 *    len - The length of the array.
 * Returns:
 *    OK - Iterator was successfully created.
 *    ALLOC_FAILURE - Failed to allocate enough memory from the heap.
 */
Status ts_iterator_new(ConcurrentIterator **iter, pthread_mutex_t *lock, void **items, long len);

/**
 * Returns TRUE if the iteration has more elements, FALSE if not.
 *
 * Params:
 *    iter - The iterator to operate on.
 * Returns:
 *    TRUE if iterator has more elements, FALSE if not.
 */
Boolean ts_iterator_hasNext(ConcurrentIterator *iter);

/**
 * Advances to the next element in the iteration, and stores it into '*next'.
 *
 * Params:
 *    iter - The iterator to operate on.
 *    next - The pointer address to store the next element into.
 * Returns:
 *    OK - Next iteration item was returned.
 *    ITER_END - The current iteration has already ended.
 */
Status ts_iterator_next(ConcurrentIterator *iter, void **next);

/**
 * Destroys the iterator instance by freeing all of its reserved memory.
 *
 * Params:
 *    iter - The iterator to destroy.
 * Returns:
 *    None
 */
void ts_iterator_destroy(ConcurrentIterator *iter);

#endif  /* _CDS_TS_ITERATOR_H__ */
