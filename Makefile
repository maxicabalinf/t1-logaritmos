# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#  -Werror
#  -pedantic
#  -Wall
CFLAGS  = -g -O3

default: test_tree_construction

test_tree_construction: test_tree_construction.o
	$(CC) $(CFLAGS) -o test_tree_construction test_tree_construction.o r_tree.o nearest_x.o exec.o sort_tile_recursive.o hilbert.o

test_tree_construction.o:
	$(CC) $(CFLAGS) -c ./test/test_tree_construction.cpp ./src/r_tree.cpp ./src/nearest_x.cpp ./src/exec.cpp ./src/sort_tile_recursive.cpp ./src/hilbert.cpp

# r_tree.o:
# 	$(CC) $(CFLAGS) -c ./src/r_tree.cpp ./src/nearest_x.cpp

# exec.o: ./src/exec.cpp
# 	$(CC) $(CFLAGS) -c ./src/exec.cpp

TO_DELETE = *.o *.exe test*
clean:
	@echo OS: $(OS)
ifeq ($(OS), Windows_NT)
	del $(TO_DELETE)
else
	$(RM) $(TO_DELETE)
endif