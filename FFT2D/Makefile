CC = clang++
FLAGS = -Wall -Wextra -pedantic -std=c++14 -lpthread -O3 -fopenmp
FILES = FFT2D.cc image.cc

all:
	$(CC) $(FLAGS) $(FILES) -o fft

clean:
	rm -rf *.o
	rm -rf fftoutput.ppm output.ppm

check: all
