#pragma once
#include <iostream>
#include <string>
#include <valarray>
#include <complex>
#include <fstream>
#include <vector>
typedef std::complex<double> Complex;

class im
{
  public:
  static im imread(const std::string& filename);
  im(int height, int width);
  void imsave(const std::string& filename);
  void transpose();
  Complex compress(Complex& c, double m, double compression);
  void shift();
  void compression(double ratio);
  void resize_color(double r);

  std::valarray<std::valarray<Complex>> red;
  std::valarray<std::valarray<Complex>> blue;
  std::valarray<std::valarray<Complex>> green;
  std::vector<std::valarray<std::valarray<Complex>>*> components;

  unsigned height;
  unsigned width;
};

//buggy functions do not use
im compres_fftim(im image, double r);
im decompres_fftim(im image);

