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
#include "arg_parser.h"

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
static void convert_to_bash(const char *regex, char dest[]) {

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

static int parse_options(int key, char *arg, struct argp_state *state) {

    switch () {}
    return 0;
}

/* List of program options parsable by argp library */
static struct argp_option options[] = {
    {"all", 'a', 0, 0, "Does not ignore entries starting with '.'"},
    {"conflict", 'c', 0, 0, "Performs a conflicting search; matches all that do NOT match the specified pattern"},
    {"max-depth", 'D', "N", 0, "Recursively searches no more than N subdirectories for each directory in the search path"},
    {"check-folders", 'F', 0, 0, "Includes folders in the search"},
    {"human-readable", 'h', 0, 0, "When using the list format, print human readable sizes (e.g. 16K, 8M, 4G)"},
    {"ignore-case", 'i', 0, 0, "Performs a case-insensitive search"},
    {"list", 'l', 0, 0, "Uses a long list format"},
    {"max-results", 'M', "N", 0, "Display no more than N results"},
    {"quiet", 'q', 0, 0, "Prints only the number of matches, not the matches themselves"},
    {"reverse", 'r', 0, 0, "Reverses the sorting when displaying the matches"},
    {"scan", 's', "DIR", 0, "Adds DIR to the search path"},
    {"threads", 'X', "N", 0, "Performs the search with N number of PThreads"},
    { 0 }
};

static struct argp argps = {opts, parse_options};

int prog_args_parse(int argc, char **argv, ProgArgs *progArgs) {

    return argp_parse(&argps, argc, argv, 0, 0, 0);
}

void prog_args_free(ProgArgs *args) {

    if (args != NULL) {
        if (args->searchPaths != NULL) {
            int i;
            for (i = 0; args->searchPaths[i] != NULL; i++)
                free(args->searchPaths[i]);
            free(args->searchPaths);
        }
        if (args->regex != NULL)
            free(args->regex);
        free(args);
    }
}
