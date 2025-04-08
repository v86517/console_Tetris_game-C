CC = gcc
OS = $(shell uname)
ifeq ($(OS), Linux)
	LIBS = -lcheck -pthread -lrt -lm -lsubunit
else
	LIBS = -lcheck
endif

CFLAGS = -Wall -Werror -Wextra -std=c11 $(LIBS)

all: install
	./tetris.out 

tetris_backend.a:
	$(CC) -c $(CFLAGS) brick_game/tetris/tetris_backend.c
	ar rc tetris_backend.a tetris_backend.o
	ranlib tetris_backend.a
	rm -rf tetris_backend.o
	rm -rf brick_game/tetris/tetris_backend.h.gch

tetris_frontend.a:
	$(CC) -c $(CFLAGS) gui/cli/tetris_frontend.c
	ar rc tetris_frontend.a tetris_frontend.o
	ranlib tetris_frontend.a
	rm -rf tetris_frontend.o
	rm -rf gui/cli/tetris_frontend.h.gch

gcov_tetris_backend.a:
	rm -rf tetris_backend.a
	$(CC) --coverage -c $(CFLAGS) brick_game/tetris/tetris_backend.c
	ar rc tetris_backend.a tetris_backend.o
	ranlib tetris_backend.a
	rm -rf tetris_backend.o
	rm -rf tetris_backend.h.gch

test: gcov_tetris_backend.a
	$(CC) -c $(CFLAGS) tetris_test.c
	$(CC) --coverage *.o tetris_backend.a $(LIBS) -o test
	./test

gcov_report: test
	lcov -c -d ./ -o tetris_test.info --rc lcov_branch_coverage=1    
	genhtml -o ./report tetris_test.info --rc lcov_branch_coverage=1
	open report/index.html

clean:
	rm -rf *.a
	rm -rf *.o
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.out
	rm -rf brick_game/tetris/*.h.gch
	rm -rf gui/cli/*.h.gch
	rm -rf report
	rm -rf gcov_report.info
	rm -rf test
	rm -rf *.info

install: tetris_backend.a tetris_frontend.a
	$(CC) $(CFLAGS) brick_game/tetris/tetris.c tetris_backend.a tetris_frontend.a -lncurses -o tetris.out

uninstall: clean

dvi:
	open Description.png

dist: 
	tar -cvzf tetris.tar Makefile Description.png brick_game/tetris/tetris_backend.c brick_game/tetris/tetris_backend.h brick_game/tetris/tetris.c tetris_test.c high_score.txt gui/cli/tetris_frontend.c gui/cli/tetris_frontend.h

.PHONY: all test clean gcov_report install uninstall dist
