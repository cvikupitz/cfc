##############################################
# Makefile for cfc Utility
# Author: Cole Vikupitz
##############################################

##### Directories for header, source, and test files
INCLUDE=./include
SRC=./src
TEST=./test

##### Macros used for compilation & linking stages
CC=gcc
CFLAGS=-W -Wall -Wextra -g
IFLAGS=-I$(INCLUDE)
#LIBS=-lm -lpthread
#LFLAGS=-L. -lcds -lcunit $(LIBS)
COMPILE=$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $^
LINK=$(CC) $(CFLAGS) -o $@ $^

$(SRC)/driver: $(SRC)/driver.o $(SRC)/arg_parser.o $(SRC)/queue.o $(SRC)/treeset.o \
        $(SRC)/iterator.o $(SRC)/file_utils.o $(SRC)/regex_engine.o $(SRC)/crawler.o
	$(LINK)

$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)

clean:
	rm -f $(SRC)/*.o $(SRC)/driver
