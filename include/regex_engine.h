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

#ifndef _REGEX_ENGINE_H__
#define _REGEX_ENGINE_H__

#define CMP_FAIL 1
#define NO_CMP   2
#define NO_MATCH 3
#define NO_ERROR 4

/**
 * Interface for the Regex engine ADT.
 */
typedef struct regex_engine RegexEngine;

/**
 * A dedicated struct to identify each match found during execution.
 */
typedef struct {
    int start;          /* The starting index of the match */
    int end;            /* The ending index of the match */
} RegexMatch;

/**
 * Creates a new instance of the RegexEngine and returns a pointer to the new instance,
 * or NULL if allocation failed.
 *
 * Params:
 *    None
 * Returns:
 *    A RegexEngine* to the new instance, or NULL if allocation failed.
 */
RegexEngine *regex_engine_new(int max);

/**
 * FIXME
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    pattern - The regular expression to compile.
 * Returns:
 */
int regex_engine_compile_pattern(RegexEngine *regex, const char *pattern, int flags);

/**
 * FIXME
 *
 * Params:
 * Returns:
 */
int regex_engine_isMatch(RegexEngine *regex, const char *str);

/**
 * FIXME
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    str - The string to execute the scan on.
 * Returns:
 */
int regex_engine_execute(RegexEngine *regex, const char *str);

/**
 * FIXME
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    matches -
 *    size -
 * Returns:
 *
 */
int regex_engine_getMatches(RegexEngine *regex, RegexMatch *matches, int *len);

/**
 * FIXME
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    buffer - The char array to store the error message.
 *    size - The max size of the buffer.
 * Returns:
 *    0 if successful (the last error was loaded), 1 if not. FIXME
 */
int regex_engine_error(RegexEngine *regex, char buffer[], size_t size);

/**
 * Destroys the specified RegexEngine by returning its allocated heap memory.
 *
 * Params:
 *    regex - The RegexEngine to destroy.
 * Returns:
 *    None
 */
void destroy_regex_engine(RegexEngine *regex);

/*
   int regcomp(regex_t *restrict, const char *restrict, int)
   size_t regerror(int, const regex_t *restrict, char *restrict, size_t)
   int regexec(const regex_t *restrict, const char *restrict, size_t, regmatch_t[restrict], int)
   void regfree(regex_t *)
 */

#endif  /* _REGEX_ENGINE_H__ */
