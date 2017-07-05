#include "MultFFT.hh"
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
  RecursiveFFT(even);
  RecursiveFFT(odd);

  for (size_t k = 0; k < N/2; ++k)
  {
    Complex j = std::polar(1.0, (-2 * PI * k) / N) * odd[k];

    ComplexCoef[k] = even[k] + j;
    ComplexCoef[k+N/2] = even[k] - j;
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

std::valarray<Complex> Convolution(std::valarray<Complex>& A, std::valarray<Complex>& B)
{
  std::valarray<Complex> C(A.size());
  for (std::size_t i = 0; i < A.size(); i++)
    C[i]= A[i]*B[i];
  return C;
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

std::valarray<Complex> MulWithFFT(std::valarray<Complex>& A,std::valarray<Complex>& B)
{
  size_t nsize = sup_pow_two(A.size() + B.size());
  auto a = std::valarray<Complex>(nsize);
  auto b = std::valarray<Complex>(nsize);

  for (size_t len = 0; len < A.size(); len++)
  {
    a[len] = A[len];
  }
  for (size_t len = 0; len < B.size(); len++)
  {
    b[len] = B[len];
  }

  RecursiveFFT(a);
  RecursiveFFT(b);
  std::valarray<Complex> C = Convolution(a,b);
  InverseFFT(C);
  std::valarray<Complex> res = C[std::slice(0, A.size() + B.size() - 1, 1)];
  return res;
}

std::valarray<Complex> FulfillValarray(std::string& str)
{
  std::valarray<Complex> val(str.size());
  for (std::size_t i = 0; i < str.size(); i++)
  {
    if ( str[i] - '0' < 0 || str[i] - '0' > 9)
      throw std::invalid_argument("Invalid number");

    val[i] = str[i] - '0';
  }
  return val;
}

std::vector<int> Base10(std::valarray<Complex> val)
{
  std::vector<int> res;

  for (auto x: val)
    res.push_back(x.real() + 0.5);

  for (size_t i = res.size() - 1; i > 0 ; i--)
  {
    if (res[i] >= 10)
    {
      res[i-1] += res[i] / 10;
      res[i] = res[i] % 10;
    }
  }
  return res;
}

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

bool inte(double n1, double n2)
{
  if(n2 * (1 - 0.1) < n1 && n1 < n2 * (1 + 0.1))
    return true;
  return false;
}

void cmp(std::valarray<std::valarray<Complex>> im1, std::valarray<std::valarray<Complex>> im2)
{
 for (std::size_t i = 0; i < im1.size(); i++)
    for (std::size_t j = 0; j < im1[i].size(); j++)
    {
      if(inte(im1[i][j].real(), im2[i][j].real()))
        std::cout << "im1 = " << im1[i][j].real() <<" im2 " << im2[i][j].real() << std::endl;
    }
}

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
    std::cout << "Please enter a number : n1" << '\n';
    std::cin >> coefA;
    std::cout << "Please enter a second number: n2" << '\n';
    std::cin >> coefB;
  }

  auto A = FulfillValarray(coefA);
  auto B = FulfillValarray(coefB);
  std::valarray<Complex> C = MulWithFFT(A,B);
  std::vector<int> res = Base10(C);
  
  std::cout << "n1*n2 = ";
  for (auto x: res)
    std::cout << x;
  
  std::cout << std::endl;
  return 0;
}