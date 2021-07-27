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


struct regex_engine {
    regex_t regex;
    int lastStatus;
};

RegexEngine *regex_engine_new(void) {
    return NULL;
}

int regex_engine_compile_pattern(RegexEngine *regex, const char *pattern, int flags) {
    return 0;
}

int regex_engine_isMatch(RegexEngine *regex, const char *str) {
    return 0;
}

int regex_engine_execute(RegexEngine *regex, const char *str) {
    return 0;
}

int regex_engine_getMatches(RegexEngine *regex, RegexMatch **matches, int size) {
    return 0;
}

int regex_engine_error(RegexEngine *regex, char[] buffer, int size) {
    return 0;
}

void destroy_regex_engine(RegexEngine *regex) {

}

/*
 regex_t exp;
    regmatch_t matches[100];
    char buff[1024];
    int res;
    char *regex = "^[0-9]+$";

    if (0!= (res = regcomp(&exp, regex, REG_EXTENDED|REG_NEWLINE))) {
        regerror(res, &exp, buff, sizeof(buff));
        fprintf(stderr, "Failed to compile the regex '%s' - %s\n", argv[1], buff);
        return -1;
    }

    int s = regexec(&exp, argv[1], 100, matches, 0);
    if (!s) {
        fprintf(stdout, "Is a match!\n");
    } else {
        fprintf(stdout, "Not a match.\n");
    }

    regfree(&exp);
 */
