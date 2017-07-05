#pragma once
#include <complex>
#include <iostream>
#include <valarray>
#include <vector>
#include <stdexcept>
#include <cmath>
#define PI 3.141592653589793238460

typedef std::complex<double> Complex;
void RecursiveFFT(std::valarray<Complex*>& ComplexCoef);
void FFT2D(std::valarray<std::valarray<Complex*>>& ComplexCoef);
void FFT2DInverse(std::valarray<std::valarray<Complex*>>& ComplexCoef);
void InverseFFT(std::valarray<Complex*>& ComplexCoef);
size_t sup_pow_two(size_t n);
