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
#include "iterator.h"

/*
 * The struct for the Iterator ADT .
 */
struct iterator {
    void **items;       /* The collection of elements to iterate */
    long next;          /* Index to next element in iteration */
    long len;           /* The total number of elements */
};

Status iterator_new(Iterator **iter, void **items, long len) {

    /* Allocate memory for the struct, check for malloc() errors */
    Iterator *temp = (Iterator *)malloc(sizeof(Iterator));
    if (temp == NULL)
        return ALLOC_FAILURE;

    /* Initialize the rest of iterator members */
    temp->items = items;
    temp->next = 0L;
    temp->len = len;
    *iter = temp;

    return OK;
}

Boolean iterator_hasNext(Iterator *iter) {
    return ( iter->next < iter->len ) ? TRUE : FALSE;
}

Status iterator_next(Iterator *iter, void **next) {

    /* Return status is the current iteration has already ended */
    if (iter->next == iter->len)
        return ITER_END;
    /* Advances to next item in iteration */
    *next = iter->items[iter->next++];

    return OK;
}

void iterator_destroy(Iterator *iter) {
    free(iter->items);
    free(iter);
}
