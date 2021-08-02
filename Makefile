##############################################
# Makefile for cfc Utility
# Author: Cole Vikupitz
##############################################

##### Directories for header, source, and test files
BIN=./bin
INCLUDE=./include
SRC=./src
TEST=./test

##### Macros used for compilation & linking stages
CC=gcc
CFLAGS=-W -Wall -Wextra -g
IFLAGS=-I$(INCLUDE)
LIBS=-lpthread
COMPILE=$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $^
LINK=$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(SRC)/driver: $(SRC)/driver.o $(SRC)/arg_parser.o $(SRC)/queue.o $(SRC)/treeset.o \
        $(SRC)/iterator.o $(SRC)/file_utils.o $(SRC)/regex_engine.o $(SRC)/crawler.o \
        $(SRC)/ts_iterator.o $(SRC)/ts_treeset.o $(SRC)/work_queue.o
	$(LINK)

$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)

clean:
	rm -f $(SRC)/*.o $(SRC)/driver
