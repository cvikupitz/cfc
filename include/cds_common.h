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

#ifndef _CDS_COMMON_H__
#define _CDS_COMMON_H__

/**
 * An enumeration representing a simple boolean type.
 *
 * Reserved as the return type for operations where only 2 outcomes can exist, or
 * the result of some operation can be either true or false (i.e. isEmpty(),
 * containsKey(), containsValue(), etc.).
 *
 * Only used for readability. The returning Boolean enumeration can still function
 * in C code where 0 is treated false and !0 is true.
 */
typedef enum {
    FALSE = 0,          /* false */
    TRUE = 1            /* true */
} Boolean;

/**
 * An enumeration representing the status of some operation performed on a data
 * structure.
 *
 * There are some cases where a simple true/false outcome may not diagnose the
 * exact reason why an operation failed. In addition to the SUCCESS status, there
 * are other negative statuses that may be returned by the method that will further
 * describe the reason the operation failed (i.e. an allocation failure, a given
 * parameter was not valid, etc.).
 */
typedef enum {

    /**
     * Operation was successful and no additional checks are needed.
     */
    OK = 0,

    /**
     * Status reserved for the put() method for HashMap and TreeMap. Indicates that
     * the key-value insertion was successful.
     */
    INSERTED = 1,

    /**
     * Status reserved for the put() method for HashMap and TreeMap. Indicates that
     * the key-value insertion was successful, but that the map already contains the
     * key and the previous value has been replaced with the new value.
     */
    REPLACED = 2,

    /**
     * Operation could not be completed due to an already existing entry in the
     * structure. This is for set-like structures where only one of a kind elements
     * are allowed, but the caller has attempted to insert an identical item.
     */
    ALREADY_EXISTS = 3,

    /**
     * Operation could not be completed due to an empty structure. This is most common
     * for retrieval and removal operations on empty structures that have no elements
     * to search, retrieve, or delete.
     */
    STRUCT_EMPTY = 4,

    /**
     * Operation could not be completed due to a full structure. This is most common
     * for insertion operations on structures that have a capacity on the number of
     * elements it may contain at once.
     */
    STRUCT_FULL = 5,

    /**
     * The previous call to iterator_next() did not yield an item since the current
     * iteration has ended.
     */
    ITER_END = 6,

    /**
     * Operation could not be completed due to an invalid index specified. This is for
     * data structures whose elements may be accessed via a specified array index. The
     * index a caller may specify could fall outside the valid range (i.e. the value is
     * below 0 or greater than the largest index allowed).
     */
    INVALID_INDEX = 7,

    /**
     * Operation could not be completed due to a missing entry. This is for structure
     * operations where a caller may search for a specific item that does not exist. Think
     * of a key missing in a HashMap, an entry not found in a set, etc.
     */
    NOT_FOUND = 8,

    /**
     * A call to any of the dynamic memory allocator methods (i.e. malloc(), realloc(), etc.)
     * has failed during instantiation, additions, resizing, or any other operation that
     * requires allocating heap memory.
     */
    ALLOC_FAILURE = 9

} Status;

/**
 * A structure representing an array of objects that gets returned from the data
 * collection's toArray() method. Rather than having the caller provide a (void ***)
 * for where the array is initialized and a (long *) for the length of the array, this
 * structure should encapsulate everything needed.
 *
 * Once the Array object is initialized and populated with elements from the previous
 * toArray() invocation, callers can access the data through the structure's public
 * members. The caller is responsible for freeing the array's item and the array struct
 * itself once it's no longer needed.
 */
typedef struct {
    void **items;       /* The array of elements */
    long len;           /* The length of the array of elements */
} Array;

/**
 * Macro used for deallocating the specified Array* item.
 *
 * Simply used to save you one extra line of code. Alternatively, you can manually free
 * the internal array and the struct if you so choose.
 */
#define FREE_ARRAY(a) {\
    free(a->items); \
    free(a); \
}

#endif  /* _CDS_COMMON_H__ */
