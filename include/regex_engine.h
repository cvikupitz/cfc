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

/**
 * Interface for the Regex engine ADT.
 */
typedef struct regex_engine RegexEngine;

/**
 *
 */
typedef struct regex_match RegexMatch;

/**
 * Creates a new instance of the RegexEngine and returns a pointer to the new instance,
 * or NULL if allocation failed.
 *
 * Params:
 *    None
 * Returns:
 *    A RegexEngine* to the new instance, or NULL if allocation failed.
 */
RegexEngine *regex_engine_new(void);

/**
 * Compiles the specified regular expression pattern '*pattern' for the regex engine to use
 * in subsequent calls to 'isMatch()' and 'execute()'. Returns 1 if the pattern compiles successfully,
 * or 0 if not. FIXME
 */
int regex_engine_compile_pattern(RegexEngine *regex, const char *pattern);

/**
 * Checks the specified string '*str' against the last compiled regular expression, but does not save any results.
 */
int regex_engine_isMatch(RegexEngine *regex, const char *str);

// return 1 if matches, 0 if not -- saves results
int regex_engine_execute(RegexEngine *regex, const char *str);

// get error from last run, if applicable
int regex_engine_error(RegexEngine *regex, char[] buffer, int size);

// destroy the struct
void destroy_regex_engine(RegexEngine *regex);

/*
   int regcomp(regex_t *restrict, const char *restrict, int)
   size_t regerror(int, const regex_t *restrict, char *restrict, size_t)
   int regexec(const regex_t *restrict, const char *restrict, size_t, regmatch_t[restrict], int)
   void regfree(regex_t *)
 */

#endif  /* _REGEX_ENGINE_H__ */
