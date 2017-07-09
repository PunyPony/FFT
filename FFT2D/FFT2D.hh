#pragma once
#include <complex>
#include <iostream>
#include <valarray>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <atomic>
#include <thread>
#include "image.hh"
#define PI 3.141592653589793238460
static std::atomic<int> thread_number(8);

typedef std::complex<double> Complex;
void RecursiveFFT(std::valarray<Complex*>& ComplexCoef);
void FFT2DInverse(im& image);
void linesFFT2D(im& image);
void linesFFT2DInverse(im& image);
void FFT2D(im& image);
size_t sup_pow_two(size_t n);