CC = clang++
FLAGS = -Wall -Wextra -pedantic -std=c++14 -O3
FILES = MultFFT.cc

all:
	$(CC) $(FLAGS) $(FILES) -o multFFT

clean:
	rm -rf *.o multFFT


check: all
	python3 check.py
