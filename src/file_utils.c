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

#include <string.h>
#include "file_utils.h"

#define BYTE 'B'
#define KILO 'k'
#define MEGA 'M'
#define GIGA 'G'
#define TERA 'T'

#define KILOBYTE 1000
#define MEGABYTE (1000 * KILOBYTE)
#define GIGABYTE (1000 * MEGABYTE)
#define TERABYTE (1000 * GIGABYTE)

char *file_path_append(char path[], char sep) {

    int len = strlen(path);
    if (path[len - 1] != sep) {
        path[len] = sep;
        path[len + 1] = '\0';
    }

    return path;
}

char *file_path_deduct(char path[], char sep) {

    int len = strlen(path);
    if (path[len - 1] == sep) {
        path[len - 1] = '\0';
    }

    return path;
}

#define UNUSED __attribute__((unused))
char *file_size_readable_format(UNUSED unsigned long size, UNUSED char dest[]) {

    return "";
}
