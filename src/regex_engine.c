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

#include <regex.h>
#include <stdlib.h>
#include "regex_engine.h"

#define DEFAULT_MAX 128

typedef enum regex_state {
    ALLOCATED,
    COMPILED
} RegexState;

struct regex_engine {
    regex_t exp;
    RegexState state;
    int compStatus;
    int execStatus;
    int len;
    int maxLen;
    RegexMatch *matches;
};

RegexEngine *regex_engine_new(int max) {

    RegexEngine *re;
    RegexMatch *temp;
    int maxMatches;

    if ((re = (RegexEngine *)malloc(sizeof(RegexEngine))) != NULL) {

        maxMatches = (max <= 0) ? DEFAULT_MAX : max;
        if ((temp = (RegexMatch *)malloc(sizeof(RegexMatch) * maxMatches)) != NULL) {
            re->state = ALLOCATED;
            re->compStatus = 0;
            re->execStatus = 0;
            re->len = 0;
            re->maxLen = maxMatches;
            re->matches = temp;
        } else {
            free(re);
            re = NULL;
        }
    }

    return re;
}

int regex_engine_compile_pattern(RegexEngine *regex, const char *pattern, int flags) {

    int status = 0;

    if (regex->state == COMPILED) {
        regfree(&(regex->exp));
        regex->state = ALLOCATED;
    }

    regex->compStatus = regcomp(&(regex->exp), pattern, flags);
    if (regex->compStatus) {
        status = CMP_FAIL;
    } else {
        regex->state = COMPILED;
    }

    return status;
}

int regex_engine_isMatch(RegexEngine *regex, const char *str) {

    int status = 0;
    regmatch_t match[1];

    if (regex->state == COMPILED) {
        regex->execStatus = regexec(&(regex->exp), str, 1, match, 0);
        status = (!regex->execStatus) ? 1 : 0;
    }

    return status;
}

int regex_engine_execute(RegexEngine *regex, const char *str) {

    int status = NO_CMP;

    if (regex->state == COMPILED) {

        regmatch_t matches[regex->maxLen];
        regex->execStatus = regexec(&(regex->exp), str, regex->maxLen, matches, 0);
        if (regex->execStatus) {
            status = NO_MATCH;
        } else {
            int i;
            for (i = 0; i < regex->maxLen; i++) {
                if (matches[i].rm_so == -1)
                    break;
                regex->matches[i].start = matches[i].rm_so;
                regex->matches[i].end = matches[i].rm_eo;
            }
            regex->len = i;
            status = 0;
        }
    }

    return status;
}

int regex_engine_getMatches(RegexEngine *regex, RegexMatch **matches, int *len) {

    int status = NO_MATCH;
    if (!regex->execStatus || regex->len == 0) {
        *matches = regex->matches;
        *len = regex->len;
        status = 0;
    }

    return status;
}

int regex_engine_error(RegexEngine *regex, char buffer[], size_t size) {

    int status = NO_ERROR;
    if (regex->compStatus) {
        regerror(regex->compStatus, &(regex->exp), buffer, size);
        status = 0;
    } else if (regex->execStatus) {
        regerror(regex->execStatus, &(regex->exp), buffer, size);
        status = 0;
    }

    return status;
}

void destroy_regex_engine(RegexEngine *regex) {

    if (regex != NULL) {
        if (regex->state == COMPILED)
            regfree(&(regex->exp));
        free(regex->matches);
        free(regex);
    }
}
