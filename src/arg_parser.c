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

#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "arg_parser.h"
#include "file_utils.h"

static ProgArgs *prog_args = NULL;

/* Program version */
const char *argp_program_version = "cfc 1.0";
/* Address to send bugs */
const char *argp_program_bug_address = "https://github.com/cvikupitz/cfc/issues";
/* Documentation for usage */
static char doc[] =
"Recursively scans for files that match the specified pattern 'REGEX'.\n\
The regex specified must be a bash pattern that will be used to match against the files (e.g., 'test??.txt', 't*.txt', '[a-z].txt'). \
The bash pattern must also be passed inside quotes to prevent bash itself from interpreting the expression before passing it as an argument.\n\n\
You can specify which paths on the system to search in with the -I flag. If no paths are specified, the current working directory ('./') will \
only be searched.\n\n\
A quick refresher on bash patterns:\n\
  '*'     - Matches all strings, including the empty/null string.\n\
  '?'     - Matches a single character.\n\
  '[...]' - Matches any of the enclosed characters. A pair of characters\n\
            separated by a hyphen denotes a range expression; any character\n\
            that sorts between those two characters (inclusive) is a match.\n\
            If the first character following the opening bracket ('[') is a\n\
            carat ('^'), then any characters NOT in the set is a match.\n\n\
Examples:\n\
  'file?.txt' matches 'file1.txt', 'file2.txt', and 'file8.txt', but not\n\
      'fil1.txt', 'file.txt', or 'file10.txt'.\n\
  '*.txt' matches 'test.txt', '.t.txt', and '.txt', but not\n\
      'test.pdf', 'test.png', or '  '.\n\
  '[0-9]+' matches '0', '001', and '99', but not\n\
      '1A', 'A1', or 'TEST'.\n\
  '[^0-9]+' matches 'TEST', '(A)', and ' -B', but not\n\
      'A1', '100', or 'TEST__9'.\v\
All results and other informational output will be printed to standard output, and all error messages (e.g., directory fails to open) will be \
printed to standard error.\n\n\
Exit Status:\n\
  0 if successful\n\
  1 for minor issues (e.g., argument(s) parsed were invalid)\n\
  2 for major issues (e.g., dynamic memory allocation failed)\n";

/*
 * Converts a bash pattern to a regex to be compiled. Stores the result into 'dest'.
 *
 * Performs the following conversions:
 *    - Adds a '^' to the start
 *    - Converts a '.' to a '\.'
 *    - Converts a '?' to a '.'
 *    - Converts a '*' to a '.*'
 *    - Adds a '$' to the end
 */
static void convert_to_bash(char *regex, char dest[]) {

    char *curr = regex;
    int i = 0;

    dest[i++] = '^';
    while (*curr != '\0') {
        switch (*curr) {
            case '.':
                dest[i++] = '\\';
                dest[i++] = '.';
                break;
            case '?':
                dest[i++] = '.';
                break;
            case '*':
                dest[i++] = '.';
                dest[i++] = '*';
                break;
            default:
                dest[i++] = *curr;
                break;
        }
        curr++;
    }
    dest[i++] = '$';
    dest[i] = '\0';
}

/*
 * Function used to parse the program arguments. Iterates through the flags and sets the flags &
 * properties in the struct as needed.
 */
static int parse_options(int key, char *arg, struct argp_state *state) {

    char *after;
    int *arg_count = state->input;

    switch (key) {
        case 'a':
            prog_args->progFlags |= (1 << SHOW_ALL);
            break;
        case 'c':
            prog_args->progFlags |= (1 << CONFLICT);
            break;
        case 'D':
            {
                int temp = strtol(arg, &after, 10);
                if (temp < 0) {
                    argp_failure(state, 1, 0, "invalid max depth: %s - must be an int greater than or equal to 0.", arg);
                } else {
                    prog_args->maxDepth = temp;
                }
                break;
            }
        case 'F':
            prog_args->progFlags |= (1 << CHECK_FOLDERS);
            break;
        case 'I':
            {
                int i = prog_args->nPaths;
                if (i >= MAX_DIRS)
                    break;
                strcpy(prog_args->searchPaths[i], arg);
                file_path_append(prog_args->searchPaths[i], '/');
                prog_args->nPaths++;
                break;
            }
        case 'i':
            prog_args->progFlags |= (1 << IGNORE_CASE);
            break;
        case 'X':
            {
                int temp = strtol(arg, &after, 10);
                if (temp <= 0) {
                    argp_failure(state, 1, 0, "invalid thread count: %s - must be an int greater than 0.", arg);
                } else {
                    prog_args->nThreads = temp;
                }
                break;
            }
        case 'M':
            {
                long temp = strtol(arg, &after, 10);
                if (temp <= 0) {
                    argp_failure(state, 1, 0, "invalid max results: %s - must be an int greater than 0.", arg);
                } else {
                    prog_args->maxResults = temp;
                }
                break;
            }
        case 'q':
            prog_args->progFlags |= (1 << QUIET);
            break;
        case 'r':
            prog_args->progFlags |= (1 << REVERSE);
            break;
        case ARGP_KEY_ARG:
            {
                char buffer[BUFFER_SIZE];
                (*arg_count)--;
                convert_to_bash(arg, buffer);
                strcpy(prog_args->regex, buffer);
                break;
            }
        case ARGP_KEY_END:
            if ((*arg_count) > 0) {
                argp_failure(state, 1, 0, "Pattern 'REGEX' is undefined, please specify the pattern for matching.");
            }
            break;
    }
    return 0;
}

/* List of program options parsable by argp library */
static struct argp_option options[] = {
    {0, 0, 0, 0, "Search Options", 1},
    {"all", 'a', 0, 0, "Does not ignore entries starting with '.'", 0},
    {"conflict", 'c', 0, 0, "Performs a conflicting search; matches all that do NOT match the specified pattern", 0},
    {"max-depth", 'D', "N", 0, "Recursively searches no more than N subdirectories for each directory in the search path", 0},
    {"check-folders", 'F', 0, 0, "Includes folders in the search", 0},
    {"include", 'I', "DIR", 0, "Adds DIR to the search path", 0},
    {"ignore-case", 'i', 0, 0, "Performs a case-insensitive search", 0},
    {"threads", 'X', "N", 0, "Performs the search with N number of PThreads", 0},
    {0, 0, 0, 0, "Output Options", 2},
    {"max-results", 'M', "N", 0, "Display no more than N results", 0},
    {"quiet", 'q', 0, 0, "Prints only the number of matches, not the matches themselves", 0},
    {"reverse", 'r', 0, 0, "Reverses the sorting when displaying the matches", 0},
    { 0 }
};

static struct argp argps = {options, parse_options, "'REGEX'", doc, 0, 0, 0};

int prog_args_parse(int argc, char **argv, ProgArgs **progArgs) {

    int result, arg_count = 1;

    if ((prog_args = (ProgArgs *)malloc(sizeof(ProgArgs))) == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate enough emmory from heap.\n");
        return 1;
    } else {
        prog_args->nPaths = 0;
        prog_args->maxDepth = -1;
        prog_args->maxResults = 0;
        prog_args->nThreads = 1;
        prog_args->progFlags = 0;
    }

    if ((result = argp_parse(&argps, argc, argv, 0, 0, &arg_count)) == 0) {
        *progArgs = prog_args;
    } else {
        free(prog_args);
    }

    return result;
}
