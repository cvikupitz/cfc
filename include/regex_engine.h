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

/* Status returned when the regex fails to compile */
#define CMP_FAIL 1
/* Status returned when caller attempts to match an expression before compiling regex */
#define NO_CMP   2
/* Status returned when no matches are found */
#define NO_MATCH 3
/* Status returned when there is no previous error message to fetch */
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
 *    max - Maximum number of matches that can be returned from execution.
 * Returns:
 *    A RegexEngine* to the new instance, or NULL if allocation failed.
 */
RegexEngine *regex_engine_new(int max);

/**
 * Compiles the specified regular expression pattern 'pattern' to be used in subsequent
 * calls to 'regex_engine_isMatch()' and 'regex_engine_execute()'. The 'flags' argument are
 * the flags accepted by the internal call to regcomp (e.g., REG_EXTENDED, REG_ICASE, etc).
 * Returns 0 if compilation is successful.
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    pattern - The regular expression to compile.
 *    flags - Flags to pass to the inner 'regcomp()' call.
 * Returns:
 *    0 if successful.
 *    CMP_FAIL if compilation failed for any reason.
 */
int regex_engine_compile_pattern(RegexEngine *regex, const char *pattern, int flags);

/**
 * Compares the string 'str' against the last compiled regex pattern, and returns 1 if a
 * match is found, 0 if not; 0 can also be returned if no regex was successfully compiled prior.
 * Unlike 'regex_engine_execute()', this search will not save any matches found.
 *
 * Params:
 *     regex - The RegexEngine to operate on.
 *     str - The string to search.
 * Returns:
 *     1 if a match is found.
 *     0 if no matches found or no previous regex has been compiled.
 */
int regex_engine_isMatch(RegexEngine *regex, const char *str);

/**
 * Compares the string 'str' against the last compiled regex pattern, then saves all the matched
 * results that can be fetched ina subsequent call to 'regex_engine_getMatches()'. Returns 0 if at
 * least one match was found.
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    str - The string to search.
 * Returns:
 *    0 if successful.
 *    NO_CMP if no previous regex has been compiled.
 *    NO_MATCH if no matches were found.
 */
int regex_engine_execute(RegexEngine *regex, const char *str);

/**
 * Fetches all the matches from the last call to 'regex_engine_execute()' and loads the results
 * into '*matches'.
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    matches - Pointer to array where to load matches.
 *    len - The number of matches.
 * Returns:
 *    0 if successful.
 *    NO_MATCH if no previous matches exist.
 */
int regex_engine_getMatches(RegexEngine *regex, RegexMatch **matches, int *len);

/**
 * Analyzes the last failed status code returned from 'regcomp()' or 'regexec()' and loads a
 * description string into the char array 'buffer' that can be used for error printing.
 *
 * Params:
 *    regex - The RegexEngine to operate on.
 *    buffer - The char array to store the error message.
 *    size - The max size of the buffer.
 * Returns:
 *    0 if successful.
 *    NO_ERROR if there is no previous error.
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

#endif  /* _REGEX_ENGINE_H__ */
