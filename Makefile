CC = clang++
FLAGS = -Wall -Wextra -pedantic -std=c++14 -O3
FILES = FFT.cc pic.cc

all:
	$(CC) $(FLAGS) $(FILES) -o fft

clean:
	rm -rf *.o
	rm -rf fftoutput.ppm output.ppm reduc.ppm decompress.ppm test.ppm

check: all
	python3 check.py
