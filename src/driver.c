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

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "queue.h"
#include "treeset.h"

static ProgArgs *args = NULL;
static TreeSet *results = NULL;
static Queue *paths = NULL;

/*
 * Similar to strcmp(char *, char *), but parameters are passed as 'void *' to comply
 * with the TreeSet comparator's signature.
 */
static int str_comparison(void *s1, void *s2) {
    return strcmp((char *)s1, (char *)s2);
}

/*
 * Inverse function to strcmp(char *, char *); used for reverse ordering when the program
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
        treeset_destroy(results, free);
    if (paths != NULL)
        queue_destroy(paths, free);
}

/*
 * Prints the specified error message then exits with a non-success code.
 */
static void error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    cleanUp();
    exit(1);
}

static void printArgs(char *name, int pos, unsigned int flags) {
    int state = ((flags & (pos << 1)) ? 1 : 0);
    fprintf(stdout, "%s=%d\n", name, state);
}

/*
 * Runs the program.
 */
int main(int argc, char **argv) {

    char *path;
    int (*comparator)(void *, void *);
    int status;

    /* Parse command line arguments */
    if ((status = prog_args_parse(argc, argv, &args)) != 0)
        return status;

    /* Instantiate the necessary ADTs */
    comparator = str_comparison;//(args->reverse == 0) ? str_comparison : str_comparison_reverse;//FIXME
    if (treeset_new(&results, comparator) != OK)
        error("ERROR: Failed to allocate enough memory from heap.");
    if (queue_new(&paths) != OK)
        error("ERROR: Failed to allocate enough memory from heap.");

    /* Adds each of the specified search directories into the list */
    if (args->nPaths == 0) {
        /* If user has not specified any paths, add current working directory */
        if ((path = strdup("./")) == NULL) {
            error("ERROR: Failed to allocate enough memory from heap.");
        }
        if (queue_add(paths, path) != OK) {
            free(path);
            error("ERROR: Failed to allocate enough memory from heap.");
        }
    } else {
        /* Otherwise, add each of the specified paths into the list */
        int i;
        for (i = 0; i < args->nPaths; i++) {
            if ((path = strdup(args->searchPaths[i])) == NULL) {
                error("ERROR: Failed to allocate enough memory from heap.");
            }
            if (queue_add(paths, path) != OK) {
                free(path);
                error("ERROR: Failed to allocate enough memory from heap.");
            }
        }
    }

    /*int i;
    for (i = 0; i < args->nPaths; i++) {
        struct dirent *pDir;
        DIR *dir = opendir(args->searchPaths[i]);
        while ((pDir = readdir(dir)) != NULL) {
            printf("[%s]\n", pDir->d_name);
        }
        closedir(dir);
    }*/

    /*
    while paths list os not empty:
      pop path from list
      open the path
      if path cannot be opened:
        print error and continue
      for each F in path:
        if F is a file:
          test F against regex
          if F is a match:
            add F to results
          else:
            add F to list of paths
 */

    char *currPath;
    DIR *currDir;
    struct dirent *dent;

    while (queue_isEmpty(paths) == FALSE) {

        (void)queue_poll(paths, (void **)&currPath);
        if ((currDir = opendir(currPath)) == NULL) {
            fprintf(stderr, "ERROR: Failed to open the directory - %s\n", currPath);
            continue;
        }

        while ((dent = readdir(currDir)) != NULL) {

            if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
                continue;
            /*
             * check dent->d_name against regex
             * if is a match:
             *   add currPath + dent->d_name to results
             *
             */
            fprintf(stdout, "%s%s\n", currPath, dent->d_name);
        }
    }

    cleanUp();

    return 0;
}

