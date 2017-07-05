#include "FFT2D.hh"
#include "image.hh"
#include <thread>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<long, std::ratio<1, 1000000000> > type_Clock;

void RecursiveFFT(std::valarray<Complex>& ComplexCoef/*, unsigned thread_gen*/)
{

  const size_t N = ComplexCoef.size();
  if (N <= 1) return;

  std::valarray<Complex> even = ComplexCoef[std::slice(0, N/2, 2)];
  std::valarray<Complex> odd = ComplexCoef[std::slice(1, N/2, 2)];

  /*if (thread_gen > 0)
    {
    RecursiveFFT(even, thread_gen - 1);
    std::thread ladouille(RecursiveFFT, std_ref(odd), thread_gen - 1);
    }
    else
    {
    RecursiveFFT(even, 0);
    RecursiveFFT(odd, 0);
    }*/
  RecursiveFFT(even);
  RecursiveFFT(odd);

  for (size_t k = 0; k < N/2; ++k)
  {
    Complex j = std::polar(1.0, (-2 * PI * k) / N) * odd[k];

    ComplexCoef[k] = even[k] + j;
    ComplexCoef[k+N/2] = even[k] - j;
  }
}

void FFT2D(im& image)
{
  for(auto comp_it = std::begin(image.components);
      comp_it != std::end(image.components); comp_it++)
    for(auto it = std::begin(**comp_it);
    it != std::end(**comp_it); it++)
    {
      RecursiveFFT(*it);
    }

    image.transpose();

    for(auto comp_it = std::begin(image.components);
      comp_it !=std::end(image.components); comp_it++)
    for(auto it = std::begin(**comp_it);
    it != std::end(**comp_it); it++)
    {
      RecursiveFFT(*it);
    }

}

void FFT2DInverse(im& image)
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

  image.transpose();

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

void InverseFFT(std::valarray<Complex>& ComplexCoef)
{
  // conjugate the complex numbers
  ComplexCoef = ComplexCoef.apply(std::conj);
  RecursiveFFT(ComplexCoef);
  // conjugate the complex numbers again
  ComplexCoef= ComplexCoef.apply(std::conj);
  ComplexCoef /= ComplexCoef.size();
}

/*
void print_clock(type_Clock duration)
{
  std::cout << std::chrono::duration_cast<std::chrono::microseconds>(duration).count() << " microseconds" << std::endl;
}
*/

size_t sup_pow_two(size_t n)
{
  size_t m = 1;
  while (m <= n)
    m *= 2;
  return m;
}

/*
   int main(int argc, char *argv[])
   {
   std::string coefA;
   std::string coefB;
   if (argc == 3)
   {
   coefA = argv[1];
   coefB = argv[2];
   }

   else
   {
   std::cout << "Please enter a number" << '\n';
   std::cin >> coefA;
   std::cout << "Please enter a second number" << '\n';
   std::cin >> coefB;
   }

   auto A = FulfillValarray(coefA);
   auto B = FulfillValarray(coefB);
   std::valarray<Complex> C = MulWithFFT(A,B);
   std::vector<int> res = Base10(C);

   for (auto x: res)
   std::cout << x;
   return 0;
   }
   */

void print(std::valarray<std::valarray<Complex>> im)
{
  for (std::size_t i = 0; i < im.size(); i++)
  {
    for (std::size_t j = 0; j < im[i].size(); j++)
    {
      std::cout << im[i][j].real() << " ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[])
{
  if (argc <= 1) {
    std::cout << "Usage : ./fft file_name" << std::endl;
    exit(-1);
  }

  im image = im::imread(std::string(argv[1]));
  
  /*
  im reduc = compres_fftim(image, 0.5);
  reduc.imsave("test.ppm");
  im decompress = decompres_fftim(reduc);
  decompress.imsave("testde.ppm");
  */

  FFT2D(image);
  image.shift();
  image.imsave("fftoutput.ppm");
  image.compression((double) 1/3000);

  /*
  im reduc = compres_fftim(image, 0.5);
  reduc.imsave("reduc.ppm");
  im decompress =  decompres_fftim(reduc);

  std::cout << image.red[265][265] << std::endl;
  std::cout << decompress.red[265][265] << std::endl;

  std::cout << image.red[140][140] << std::endl;
  std::cout << decompress.red[140][140] << std::endl;

  std::cout << image.red[300][300] << std::endl;
  std::cout << decompress.red[300][300] << std::endl;

  decompress.imsave("decompress.ppm");
  */

  image.shift();
  FFT2DInverse(image);

  image.imsave("output.ppm");

}


