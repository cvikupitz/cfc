#### Version 1.0

* Initial release of CFC
  * Successfully crawls and displays matched file and directory names based on a user-provided regular expression.
  * Ability to perform case-sensitive and case-insensitive searches.
  * Provides a multi-threaded implementation with some number of threads specified by the user.
  * Provides ability to print a maximum number of matches or none at all, also specified by user.
  * Provides ability to crawl up to a maximum depth of sub-folders, specified by user.

#### Version 1.1

* Added *-W*, *--min-depth*, & *--max-depth* arguments.
  * *-W, --no-warn*: User can silence all error/warning messages during the crawling phase (e.g. directories fail to open).
  * *--max-depth*: Renamed from the *-D* flag.
  * *--min-depth*: User can now specify a minimum depth of sub-directories to traverse before results will be matched by the pattern.