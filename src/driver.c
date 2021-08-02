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

#include <regex.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "crawler.h"
#include "regex_engine.h"
#include "ts_treeset.h"
#include "work_queue.h"

static ProgArgs *args = NULL;
static RegexEngine *regex = NULL;
static ConcurrentTreeSet *results = NULL;
static WorkQueue *paths = NULL;

/*
 * Similar to strcmp(char *, char *), but parameters are passed as 'void *' to comply
 * with the TreeSet comparator's signature.
 */
static int str_comparison(void *s1, void *s2) {
    return strcmp((char *)s1, (char *)s2);
}

/*
 * Inverse function to strcmp(char *, char *); used for reverse ordering when the reverse
 * flag is specified.
 */
static int str_comparison_reverse(void *s1, void *s2) {
    return strcmp((char *)s2, (char *)s1);
}

/*
 * Cleans up all allocated structure by returning its reserved memory back to heap.
 */
static void cleanUp(void) {
    if (args != NULL)
        free(args);
    if (results != NULL)
        ts_treeset_destroy(results, free);
    if (paths != NULL)
        work_queue_destroy(paths, (void *)crawler_dir_free);
    if (regex != NULL)
        destroy_regex_engine(regex);
}

/*
 * Prints the specified error message then exits with a non-success code.
 */
static void error(int status, const char *format, ...) {

    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    fprintf(stdout, "\n");
    cleanUp();
    exit(status);
}

/*
 * Runs the program.
 */
int main(int argc, char **argv) {

    CrDir *dir;
    char buffer[BUFFER_SIZE];
    int (*comparator)(void *, void *);
    int status, cflags;

    /* Parse command line arguments */
    if ((status = prog_args_parse(argc, argv, &args)) != 0)
        return status;

    /* Instantiate the necessary ADTs */
    comparator = (!GET_BIT(args->progFlags, REVERSE)) ? str_comparison : str_comparison_reverse;
    if (ts_treeset_new(&results, comparator) != OK)
        error(2, "ERROR: Failed to allocate enough memory from heap.");
    if (work_queue_new(&paths, args->nThreads) != OK)
        error(2, "ERROR: Failed to allocate enough memory from heap.");
    if ((regex = regex_engine_new(1)) == NULL)
        error(2, "ERROR: Failed to allocate enough memory from heap.");
    cflags = (!GET_BIT(args->progFlags, IGNORE_CASE)) ? REG_EXTENDED|REG_NEWLINE : REG_EXTENDED|REG_NEWLINE|REG_ICASE;
    status = regex_engine_compile_pattern(regex, args->regex, cflags);
    if (status) {
        (void)regex_engine_error(regex, buffer, sizeof(buffer));
        error(2, "ERROR: Failed to compile the pattern '%s' - %s", args->regex, buffer);
    }

    /* Adds each of the specified search directories into the list */
    if (args->nPaths == 0) {
        /* If user has not specified any paths, add current working directory */
        if ((dir = crawler_dir_malloc("./", args->maxDepth)) == NULL) {
            error(2, "ERROR: Failed to allocate enough memory from heap.");
        }
        if (work_queue_add(paths, dir) != 0) {
            crawler_dir_free(dir);
            error(2, "ERROR: Failed to allocate enough memory from heap.");
        }
    } else {
        /* Otherwise, add each of the specified paths into the list */
        int i;
        for (i = 0; i < args->nPaths; i++) {
            if ((dir = crawler_dir_malloc(args->searchPaths[i], args->maxDepth)) == NULL) {
                error(2, "ERROR: Failed to allocate enough memory from heap.");
            }
            if (work_queue_add(paths, dir) != 0) {
                crawler_dir_free(dir);
                error(2, "ERROR: Failed to allocate enough memory from heap.");
            }
        }
    }

    /*
     * Crawls over the files, prints the results, then cleans up all the
     * heap storage
     */
    process(regex, results, paths, args);
    display_results(results, args->maxResults, args->progFlags);
    cleanUp();

    return 0;
}
