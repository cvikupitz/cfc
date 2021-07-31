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

#ifndef _FILE_CRAWLER_H__
#define _FILE_CRAWLER_H__

#include "arg_parser.h"
#include "queue.h"
#include "regex_engine.h"
#include "ts_treeset.h"

/**
 * Structure to represent a directory to search as part of the file crawler.
 */
typedef struct crawler_directory {
    char *path;         /* The full directory path */
    int depth;          /* The max depth in sub-directories to crawl into */
} CrDir;

/**
 * Creates a new CrDir* object and returns the pointer to the new instance, or
 * NULL if allocation fails.
 *
 * Params:
 *    dir - The directory path.
 *    depth - The max depth to traverse.
 * Returns:
 *    The new CrDir*, or NULL if allocation failed.
 */
CrDir *crawler_dir_malloc(char dir[], int depth);

/**
 * Destroys the specified CrDir* object by freeing its reserved heap memory.
 *
 * Params:
 *    dir - The CrDir* object to deallocate.
 * Returns:
 *    None
 */
void crawler_dir_free(CrDir *dir);

/**
 * Performs the file crawl given the regex engine to use for matching and the queue of
 * directories to search in.
 *
 * Params:
 *    regex - The regex engine that will perform the string comparisons.
 *    results - The set where the results will be stored.
 *    paths - The queue of paths to search in.
 *    args - The program arguments.
 * Returns:
 *    None
 */
void process(RegexEngine *regex, ConcurrentTreeSet *results, Queue *paths, ProgArgs *args);

/**
 * Displays all matched results contained in 'results'.
 *
 * Params:
 *    results - The set containing the results.
 *    max - The maximum number of results to display (0 means no maximum).
 *    flags - Additional flags that can affect the final output.
 * Returns:
 *    None
 */
void display_results(ConcurrentTreeSet *results, long max, int flags);

#endif  /* _FILE_CRAWLER_H__ */
