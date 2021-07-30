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

#include <stdio.h>
#include "arg_parser.h"
#include "crawler.h"

void display_results(TreeSet *results, long max, int flags) {

    Iterator *iter = NULL;
    long matches = treeset_size(results);
    char *entry;

    if (!GET_BIT(flags, QUIET) && treeset_isEmpty(results) == FALSE) {

        if (treeset_iterator(results, &iter) != OK) {
            fprintf(stderr, "ERROR: Failed to create the iterator for the results.\n");
            return;
        }

        while (iterator_hasNext(iter) == TRUE) {
            (void)iterator_next(iter, (void **)&entry);
            fprintf(stdout, "%s\n", entry);
            if ((--max) == 0)
                break;
        }

        iterator_destroy(iter);
    }

    fprintf(stdout, "\nFound %ld match(es)\n", matches);
}
