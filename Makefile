# Author: Mauricio Giordano <mauricio.c.giordano@gmail.com>
# Date: 2/24/2015
# License: GPLv2

CC = g++
CFLAGS = -g -c -Wall
LDFLAGS = -std=c++0x
SOURCES = main.cc state.cc regex.cc regex_factory.cc infix_parse.cc
OBJECTS = $(SOURCES:.cc=.o)
EXEC = regex

all: infix_parse.o regex_factory.o state.o regex.o main.o
	@$(CC) infix_parse.o regex_factory.o state.o regex.o main.o -o $(EXEC)

run: all
	@./$(EXEC)

clear:
	@rm -rf *.o

main.o: main.cc
	@$(CC) $(CFLAGS) $(LDFLAGS) main.cc

state.o: state.cc
	@$(CC) $(CFLAGS) $(LDFLAGS) state.cc

regex.o: regex.cc
	@$(CC) $(CFLAGS) $(LDFLAGS) regex.cc

regex_factory.o: regex_factory.cc
	@$(CC) $(CFLAGS) $(LDFLAGS) regex_factory.cc

infix_parse.o: infix_parse.cc
	@$(CC) $(CFLAGS) $(LDFLAGS) infix_parse.cc
