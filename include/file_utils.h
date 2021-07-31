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

#ifndef _FILE_UTILS_H__
#define _FILE_UTILS_H__

/**
 * Adds the specified file separator 'sep' to the end of the specified file path 'path'
 * if it is not present, otherwise does nothing. The path buffer is modified in place and
 * is assumed to be large enough to hold the additional character.
 *
 * Params:
 *    path - The file path to append the separator to.
 *    sep - The separator character to append.
 * Returns:
 *    The resulting path.
 */
char *file_path_append(char path[], char sep);

/**
 * Removes the specified file separator 'sep' from the end of the specified file path
 * 'path' if it is present, otherwise does nothing. The path buffer is modified in place.
 *
 * Params:
 *    path - The file path to remove from.
 *    sep - The separator character to subtract.
 * Returns:
 *    The resulting path.
 */
char *file_path_deduct(char path[], char sep);

#endif  /* _FILE_UTILS_H__ */
