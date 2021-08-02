##############################################
# Makefile for cfc Utility
# Author: Cole Vikupitz
##############################################

##### Uncomment this to suppress make from echoing the commands
#.SILENT:
##### Collection of phony Makefile targets
.PHONY: dist clean mostlyclean help

##### Project metadata
NAME=cfc
VERSION=v1
DIST=$(NAME)-$(VERSION)

##### Directories for header, source, and test files
INCLUDE=./include
SRC=./src

##### Macros used for compilation & linking stages
CC=gcc
CFLAGS=-W -Wall -Wextra -g
IFLAGS=-I$(INCLUDE)
LIBS=-lpthread
COMPILE=$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $^
LINK=$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

##### List of object files to create for executable
OBJS=$(SRC)/arg_parser.o $(SRC)/crawler.o $(SRC)/driver.o $(SRC)/file_utils.o $(SRC)/iterator.o $(SRC)/queue.o \
     $(SRC)/regex_engine.o $(SRC)/treeset.o $(SRC)/ts_iterator.o $(SRC)/ts_treeset.o $(SRC)/work_queue.o

##### Builds the executable
$(NAME): $(OBJS)
	$(LINK)

##### Target for compiling individual source files
$(SRC)/%.o: $(SRC)/%.c
	$(COMPILE)

##### Creates a distribution tarball of the project
dist:
	mkdir $(DIST)/
	rsync -avz --exclude=$(DIST) * $(DIST)/
	tar -czvf $(DIST).tgz $(DIST)/
	rm -fr $(DIST)/

##### Targets for cleaning up the project
clean:
	rm -f $(OBJS) $(DIST).tgz $(NAME)
mostlyclean:
	rm -f $(OBJS) $(DIST).tgz

##### Target for displaying the usage message
help:
	@echo "Possible targets for this Makefile are the following:"
	@echo "  cfc (default) : Compiles and builds the file crawler executable."
	@echo "  dist          : Creates an archive (.tgz) of the complete repository."
	@echo "  clean         : Cleans all project files."
	@echo "  mostlyclean   : Cleans all project files except for the built libraries."
	@echo "  help          : Displays this help message."
