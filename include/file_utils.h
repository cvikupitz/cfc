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
 * Converts the specified file size into human readable format, then stores the result
 * into 'dest'. Assumes 'dest' is large enough to store the result.
 *
 * Example:
 *     399 = 399B
 *     1137 = 1.1k
 *     5698304 = 5.7M
 *
 * The supported metrics are bytes (B), kilobytes (k), megabytes (M), gigabytes (G),
 * and terabytes (T). No larger metrics are supported and any ones that are larger will
 * display as terabytes by default (e.g. 2000 terabytes won't convert to petabytes, it will
 * just convert to '2000T').
 *
 * Params:
 *    size - The size to convert (in bytes).
 *    dest - The char array to store the result in (assumed to be large enough).
 * Returns:
 *    The char array containing the result.
 */
char *file_size_readable_format(unsigned long size, char[] dest);

#endif  /* _FILE_UTILS_H__ */
