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

#ifndef _ARG_PARSER_H__
#define _ARG_PARSER_H__

typedef struct prog_args {
    char *regex;                /* The REGEX used for searching file/directory patterns */
    char **searchPaths;         /* List of directories to recursively search in */
    int all;                    /* Flag to enable checks for entries starting with '.' */
    int conflict;               /* Flag to enable opposite search */
    int maxDepth;               /* Max depth for recursive calls to sub-folders */
    int checkFolders;           /* Flag to include folders in search */
    int humanReadable;          /* Flag to enable human-readable file sizes */
    int ignoreCase;             /* Flag to enable case-insensitive searches */
    int listFormat;             /* Flag to enable the listing format (similar to ls -l) */
    long maxResults;            /* The max number of results to display */
    int quiet;                  /* Flag to disable all logs and results */
    int reverse;                /* Flag to enable reverse ordering when displaying results */
    int nThreads;               /* Number of PThreads to use */
} ProgArgs;

int prog_args_parse(int argc, char **argv, ProgArgs *progArgs);

void prog_args_free(ProgArgs *args);

#endif  /* _ARG_PARSER_H__ */
