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
#include "crawler.h"

struct crawler_args_t {
    RegexEngine *regex;
    ConcurrentTreeSet *results;
    WorkQueue *paths;
    ProgArgs *args;
};

CrDir *crawler_dir_malloc(char dir[], int depth) {

    CrDir *crDir = NULL;
    char *path;

    if ((crDir = (CrDir *)malloc(sizeof(CrDir))) != NULL) {
        if ((path = strdup(dir)) != NULL) {
            crDir->path = path;
            crDir->depth = depth;
        } else {
            free(crDir);
            crDir = NULL;
        }
    }

    return crDir;
}

void crawler_dir_free(CrDir *dir) {

    if (dir != NULL) {
        free(dir->path);
        free(dir);
    }
}

static void process_directory(DIR *dir, CrDir *crDir, struct crawler_args_t *info) {

    RegexEngine *regex = info->regex;
    ConcurrentTreeSet *results = info->results;
    WorkQueue *paths = info->paths;
    unsigned int flags = info->args->progFlags;
    struct dirent *dent;
    char buffer[BUFFER_SIZE];
    int depth = crDir->depth;

    while ((dent = readdir(dir)) != NULL) {

        /* Ignore current working directory and parent directory */
        if (strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
            continue;

        /* Ignore files & directories starting with '.' if --all is not specified */
        if (strncmp(dent->d_name, ".", 1) == 0 && !GET_BIT(flags, SHOW_ALL))
            continue;

        /* If entry is a directory, add it to list of paths to search */
        if (dent->d_type == DT_DIR) {

            if (depth != 0) {
                sprintf(buffer, "%s%s/", crDir->path, dent->d_name);
                CrDir *newDir = crawler_dir_malloc(buffer, (depth - 1));
                if (newDir != NULL) {
                    if (work_queue_add(paths, newDir) != OK)
                        free(newDir);
                }
            }

            if (GET_BIT(flags, CHECK_FOLDERS)) {

                if ((!(GET_BIT(flags, CONFLICT))) == regex_engine_isMatch(regex, dent->d_name)) {
                    sprintf(buffer, "%s%s", crDir->path, dent->d_name);
                    char *result = strdup(buffer);
                    if (result != NULL) {
                        if (ts_treeset_add(results, result) != OK)
                            free(result);
                    }
                }
            }

        } else if (dent->d_type == DT_REG) {

            if ((!(GET_BIT(flags, CONFLICT))) == regex_engine_isMatch(regex, dent->d_name)) {
                sprintf(buffer, "%s%s", crDir->path, dent->d_name);
                char *result = strdup(buffer);
                if (result != NULL) {
                    if (ts_treeset_add(results, result) != OK)
                        free(result);
                }
            }
        } else {
            /*
             * Ignore all other types of entries
             */
            continue;
        }
    }
}

static void *process_dirs(void *arg) {

    struct crawler_args_t *args = (struct crawler_args_t *)arg;
    CrDir *crDir;
    DIR *dir;
    char buffer[BUFFER_SIZE];

    while (!work_queue_poll(args->paths, (void **)&crDir)) {

        if ((dir = opendir(crDir->path)) == NULL) {
            sprintf(buffer, "ERROR: Failed to open directory %s", crDir->path);
            perror(buffer);
            crawler_dir_free(crDir);
            continue;
        }

        process_directory(dir, crDir, args);
        crawler_dir_free(crDir);
        closedir(dir);
    }

    return NULL;
}

void process(RegexEngine *regex, ConcurrentTreeSet *results, WorkQueue *paths, ProgArgs *progArgs) {

    struct crawler_args_t args = { regex, results, paths, progArgs };
    pthread_t threads[progArgs->nThreads];
    int i;

    for (i = 0; i < progArgs->nThreads; i++) {
        (void)pthread_create(&(threads[i]), NULL, process_dirs, &args);
    }
    for (i = 0; i < progArgs->nThreads; i++) {
        (void)pthread_join(threads[i], NULL);
    }
}

void display_results(ConcurrentTreeSet *results, long max, int flags) {

    ConcurrentIterator *iter = NULL;
    long matches = ts_treeset_size(results);
    char *entry;

    /*
     * If there are no matches found, simply print the appropriate message
     * and return from method.
     */
    if (ts_treeset_isEmpty(results) == TRUE) {
        fprintf(stdout, "\nNo matches found\n");
        return;
    }

    /*
     * Otherwise, if the quiet flag isn't enabled, we will iterate over all the matched
     * entries in the results TreeSet and print out each result.
     */
    if (!GET_BIT(flags, QUIET)) {

        /* Create the iterator of results */
        if (ts_treeset_iterator(results, &iter) != OK) {
            fprintf(stderr, "ERROR: Failed to create the iterator for the results.\n");
            return;
        }

        /* Iterate through each element, print out the file path */
        while (ts_iterator_hasNext(iter) == TRUE) {

            (void)ts_iterator_next(iter, (void **)&entry);
            fprintf(stdout, "%s\n", entry);
            /*
             * If the max flag is specified, we will stop printing results after the
             * Nth element. Otherwise, max is set to -1 so this condition should never
             * be met.
             */
            if ((--max) == 0)
                break;
        }

        ts_iterator_destroy(iter);
    }

    fprintf(stdout, "\nFound %ld match(es)\n", matches);
}
