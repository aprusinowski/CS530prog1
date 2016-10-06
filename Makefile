# Makefile for class student

TORM = driver driver.o file_parser.o 
CC = g++
CCFLAGS = -g -O3 -Wall -Wpointer-arith -Wcast-qual -Wwrite-strings -std=c++11

driver:	driver.o file_parser.o 
	${CC} ${CCFLAGS} -o driver driver.o file_parser.o 

driver.o:	driver.cpp
		${CC} ${CCFLAGS} -c driver.cpp 

file_parser.o:	file_parser.cc file_parser.h
		${CC} ${CCFLAGS} -c file_parser.cc


clean:
	rm -f ${TORM}