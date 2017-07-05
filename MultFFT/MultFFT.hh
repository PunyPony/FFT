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
void InverseFFT(std::valarray<Complex*>& ComplexCoef);
size_t sup_pow_two(size_t n);
std::valarray<Complex> MulWithFFT(std::valarray<Complex>& A,std::valarray<Complex>& B);
std::valarray<Complex> Convolution(std::valarray<Complex>& A, std::valarray<Complex>& B);
std::valarray<Complex> FulfillValarray(std::string& str);
std::vector<int> Base10(std::valarray<Complex> val);
bool inte(double n1, double n2);
void cmp(std::valarray<std::valarray<Complex>> im1, std::valarray<std::valarray<Complex>> im2);

