# cfc

Short for 'Concurrent File Crawler' - A crawler that searches a file system for files matching a specified bash pattern.

## Table of Contents

* [General Info](#general.info)
* [Setup](#setup)
* [Usage](#usage)
  * [About Bash Patterns](#about.bash.patterns)
  * [Command Line Arguments](#command.line.arguments)
* [Contributing](#contributing)
* [License](#license)

<a name="general.info"></a>
## General Info

In today's world, computer storage is becoming larger and cheaper. For laptops, storage can vary from several gigabytes to a couple terabytes, and that range can extends much larger for PCs, up to several terabytes of storage. This can make searching and managing older files and directories very difficult. In Linux, there is the ```find``` command that can assist with this, but the program itself is single-threaded and the syntax is cumbersome.

This application nicknamed ```cfc``` is a more simplified version of the ```find``` command. It will crawl through a file system and search for files & directories that match a given bash pattern specified by the user. It is designed to be easier to use and high-performance through PThreads.

**Examples**

Searching for Makefiles:

![doc/usage_1.png](https://github.com/cvikupitz/cfc/blob/master/doc/usage_1.png)

Counting C source and header files:

![doc/usage_2.png](https://github.com/cvikupitz/cfc/blob/master/doc/usage_2.png)

<a name="setup"></a>
## Setup

* Clone the repository: 
  * ```https://github.com/cvikupitz/cfc.git```
* Build the executable via the Makefile:
  * ```make clean cfc```
* Once built, you can run the executable as such:
  * ```./cfc '*.(c|h)' -I/home -X2 -a```
* Other options can be viewed with the help command:
  * ```make help```

<a name="usage"></a>
## Usage

The file crawler program requires one argument which is the bash pattern to be used for matching files and directories. This is also the only argument that is not preceded by dashes like the command line arguments. Since the crawler requires a bash pattern for matching, it is critical that you pass it in quotes in order to prevent bash itself from interpreting the pattern. For example:

```bash
$ ./cfc '*.txt'
```

is the correct way to run the program. However, let's say you have *test.txt* and *example.txt* in your current working directory. If you run the program without the quotes like this: 

```bash
$ ./cfc *.txt
```

then bash will run the program like this:

```bash
$ ./cfc example.txt test.txt
```

in which case the pattern used for matching becomes *test.txt*. So make sure to always enclose the pattern with quotes!

If you are rusty on bash patterns, see the below section for a brief refresher.

<a name="about.bash.patterns"></a>
### About Bash Patterns

'\*'     - Matches all strings, including the empty/null string.

'?'     - Matches a single character.

'[...]' - Matches any of the enclosed characters. A pair of characters separated by a hyphen denotes a range expression; any character that sorts between those two characters (inclusive) is a match. If the first character following the opening bracket ('[') is a carat ('^'), then any characters NOT in the set is a match.

**Examples**:

'**file?.txt**' matches '**file1.txt**', '**file2.txt**', and '**file8.txt**', but not '**fil1.txt**', '**file.txt**', or '**file10.txt**'.

'***.txt**' matches '**test.txt**', '**.t.txt**', and '**.txt**', but not '**test.pdf**', '**test.png**', or '  '.

'**[0-9]+**' matches '**0**', '**001**', and '**99**', but not '**1A**', '**A1**', or '**TEST**'.

'**\[^0-9]+**' matches '**TEST**', '**(A)**', and '**-B**', but not '**A1**', '**100**', or '**TEST__9**'.

<a name="command.line.arguments"></a>
### Command Line Arguments

Below is an overview of all the available command line arguments supported by the crawler program.

| Argument                     | Default   | Description                                                  |
| ---------------------------- | --------- | ------------------------------------------------------------ |
| ```-a, --all```              |           | The crawler will not ignore 'hidden' files and directories, that is, if the entry starts with '.'. If the entry is a file, the crawler will match the file against the pattern and include in the results if it's a match. If the entry is a directory, then the crawler will traverse down into that folder. |
| ```-c, --conflict```         |           | Performs a 'conflicting' search, that is, all files that do not match the specified bash pattern are considered matches, while entries that do match the bash pattern are ignored. |
| ```-F, --check-folders```    |           | Includes folders in the search. In addition to traversing into sub-folders, the bash pattern will also be applied to the folder names and included in the results if found as a match. |
| ```-I<DIR>, --include=DIR``` | "./"      | Include ```DIR``` in the search path. You may specify multiple search paths by giving multiple flags. If no flags are specified, only the current working directory is crawled. |
| ```-i, --ignore-case```      |           | Performs a case-insensitive search. If the pattern specified is '*\*.txt*', then this flag will cause the files *test.txt* and '*test.TXT*' to match. |
| ```--max-depth=N```          | Unbounded | Does not crawl more than N sub-directories from each directory in the search path. If there exists a directory */home/users/foobar/tests* and this path is included in the search path, and max depth specified is 2, then the crawler will stop searching within */home/users/foobar*. If max depth is set to 0, then that means no sub-folders in */home* will be searched. |
| ```--min-depth=N```          | 0         | The crawler will crawl N number of sub-directories before it will start matching files and folders. If there exists a directory */home/users/foobar/tests* and this path is included in the search path, and min depth specified is 2, then the crawler will only start checking entries in */home/users/foobar*. |
| ```-M<N>, --max-results=N``` | Unbounded | Sets the number of maximum results to display. Since the output is in alphabetical order, this means that the first N results in alphabetical order is displayed. |
| ```-q, --quiet```            |           | Does not display any of the matched results, only the total number of matches. |
| ```-r, --reverse```          |           | Reverses the output ordering of the matched results. By default, all paths are output in alphabetical order. This flag will reverse the alphabetical ordering. |
| ```-W, --no-warn```          |           | All warning messages during the crawling phase are muted. Mostly includes messages related to failing to open additional directories. |
| ```-X<N>, --threads=N```     | 1         | Sets the number of threads to run in the file crawling phase. Note that this does not apply to argument parsing or displaying the matched results. |
| ```-?, --help```             |           | Displays a helpful message along with a list of all arguments, then exits. |
| ```--usage```                |           | Displays the full usage message, then exits.                 |
| ```-V, --version```          |           | Displays the current version, then exits.                    |

<a name="contributing"></a>
## Contributing

If there is a bug or a new feature you think would be useful/cool, feel free to contribute to the project by opening your own branch and pull request:

* Fork the project: https://github.com/cvikupitz/cfc
* Create your own branch:
  * ```git checkout -b feature/foobar```
* Commit your changes:
  * ```git commit -am 'Add some foobar'```
* Push to your new branch:
  * ```git push origin feature/foobar```
* Create a new pull request

Kindly name your new features with the branch name 'feature/*name*' where *name* is a brief name of the feature. For fixes, name the branch 'fix/*name*' with *name* being the name/descriptor of the issue.

<a name="license"></a>
## License

[MIT License](https://github.com/cvikupitz/cfc/blob/master/LICENSE)