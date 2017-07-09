#include "FFT2D.hh"
#include "image.hh"
#include "timer.hh"

void RecursiveFFT(std::valarray<Complex>& ComplexCoef)
{
  const size_t N = ComplexCoef.size();
  if (N <= 1) return;
  std::valarray<Complex> even = ComplexCoef[std::slice(0, N/2, 2)];
  std::valarray<Complex> odd = ComplexCoef[std::slice(1, N/2, 2)];
  RecursiveFFT(even);
  if (thread_number > 0)
  {
    thread_number--;
    std::thread(RecursiveFFT, odd);
  }
  else
    RecursiveFFT(odd);
  
  for (size_t k = 0; k < N/2; ++k)
  {
    Complex j = std::polar(1.0, (-2 * PI * k) / N) * odd[k];
    ComplexCoef[k] = even[k] + j;
    ComplexCoef[k+N/2] = even[k] - j;
  }
}

void linesFFT2D(im& image)
{
  for(auto comp_it = std::begin(image.components);
      comp_it != std::end(image.components); comp_it++)
    for(auto it = std::begin(**comp_it);
    it != std::end(**comp_it); it++)
      RecursiveFFT(*it);
}

void FFT2D(im& image)
{
  linesFFT2D(image);
  image.transpose();
  linesFFT2D(image);
}

void linesFFT2DInverse(im& image)
{
for(auto comp_it = std::begin(image.components);
    comp_it != std::end(image.components); comp_it++)
    for(auto it = std::begin(**comp_it);
    it != std::end(**comp_it); it++)
  {
    *it = it->apply(std::conj);
    RecursiveFFT(*it);
    *it = it->apply(std::conj);
    *it /= it->size();
  }
}

void FFT2DInverse(im& image)
{
  linesFFT2DInverse(image);
  image.transpose();
  linesFFT2DInverse(image);
}

void InverseFFT(std::valarray<Complex>& ComplexCoef)
{
  ComplexCoef = ComplexCoef.apply(std::conj);
  RecursiveFFT(ComplexCoef);
  ComplexCoef= ComplexCoef.apply(std::conj);
  ComplexCoef /= ComplexCoef.size();
}

size_t sup_pow_two(size_t n)
{
  size_t m = 1;
  while (m <= n)
    m *= 2;
  return m;
}

void print(std::valarray<std::valarray<Complex>> im)
{
  for (std::size_t i = 0; i < im.size(); i++)
    for (std::size_t j = 0; j < im[i].size(); j++)
      std::cout << im[i][j].real() << " ";
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
  if (argc <= 1) {
    std::cout << "Usage : ./fft file_name" << std::endl;
    exit(-1);
  }
  double time = 0;
  {
    scoped_timer timer(time);
    im image = im::imread(std::string(argv[1]));
    FFT2D(image);
    image.shift();
    image.imsave("fftoutput.ppm");
    image.compression((double) 1/3000); // ! double
    image.shift();
    FFT2DInverse(image);
    image.imsave("output.ppm");
  }
  std::cout << "time : " << time << std::endl;
}