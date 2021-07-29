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

/* Maximum number of directories included in search path */
#define MAX_DIRS 128
/* Maximum length of inner char buffers - used for storing the pattern and directories */
#define BUFFER_SIZE 4096

typedef enum prog_flags {
    SHOW_ALL            = (1 << 0),     /* Flag to enable checks for entries starting with '.' */
    CONFLICT            = (1 << 1),     /* Flag to enable opposite search */
    CHECK_FOLDERS       = (1 << 2),     /* Flag to include folders in search */
    HUMAN_READABLE      = (1 << 3),     /* Flag to enable human-readable file sizes */
    IGNORE_CASE         = (1 << 4),     /* Flag to enable case-insensitive searches */
    LIST_FORMAT         = (1 << 5),     /* Flag to enable the listing format (similar to ls -l) */
    QUIET               = (1 << 6),     /* Flag to disable all logs and results */
    REVERSE             = (1 << 7)      /* Flag to enable reverse ordering when displaying results */
} ProgFlags;

/**
 * A container used for storing all of the program arguments.
 * When argp parses the command line arguments, the results will be stored here.
 */
typedef struct prog_args {
    char regex[BUFFER_SIZE];                    /* The REGEX used for searching file/directory patterns */
    char searchPaths[MAX_DIRS][BUFFER_SIZE];    /* List of directories to recursively search in */
    int nPaths;                                 /* Number of paths in search paths array */
    int maxDepth;                               /* Max depth for recursive calls to sub-folders */
    long maxResults;                            /* The max number of results to display */
    int nThreads;                               /* Number of PThreads to use */
    int progFlags;                              /* Holds all the boolean-style flags */
} ProgArgs;

/**
 * Parses the program arguments and stores the results into 'progArgs'. The caller is
 * responsible for freeing the struct when finished, and can be done with a single call
 * to 'free()'.
 *
 * Params:
 *    argc - The number of arguments.
 *    argv - The argument array.
 *    progArgs - The 'ProgArgs' struct to store the results in.
 * Returns:
 *    The returning value of the inner call to 'argp_parse()'.
 */
int prog_args_parse(int argc, char **argv, ProgArgs **progArgs);

#endif  /* _ARG_PARSER_H__ */
