#include "image.hh"
#include "FFT2D.hh"

im::im(int height, int width)
: red(std::valarray<Complex>(Complex(0,0), width), height),
  blue(std::valarray<Complex>(Complex(0,0), width), height),
  green(std::valarray<Complex>(Complex(0,0), width), height),
  height(height), width(width)
{
  components.push_back(&red);
  components.push_back(&green);
  components.push_back(&blue);
}

im im::imread(const std::string& filename)
{
  std::ifstream file(filename, std::ios::in);
  if(!file)
    throw std::invalid_argument("Can not open " + filename);
  std::string format;
  std::string s_height;
  std::string s_width;
  std::getline(file, format);
  std::getline(file, s_width, ' ');
  std::getline(file, s_height, ' ');
  unsigned width = std::stoi(s_width);
  unsigned height = std::stoi(s_height);
  std::cout << "width : " << width << std::endl;
  std::cout <<  "height : "<< height << std::endl;
  std::getline(file, s_height);
  im image(height, width);
  char tartiflette;
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
    {
      file.read(&tartiflette, 1);
      image.red[y][x].real(static_cast<uint8_t>(tartiflette));
      file.read(&tartiflette, 1);
      image.green[y][x].real(static_cast<uint8_t>(tartiflette));
      file.read(&tartiflette, 1);
      image.blue[y][x].real(static_cast<uint8_t>(tartiflette));
    }
  file.close();
  return image;
}

void im::transpose()
{
 std::cout << "Transpose" << std::endl;
 std::valarray<std::valarray<Complex>> redt(std::valarray<Complex>(Complex(0,0), width), height);
 std::valarray<std::valarray<Complex>> bluet(std::valarray<Complex>(Complex(0,0), width), height);
 std::valarray<std::valarray<Complex>> greent(std::valarray<Complex>(Complex(0,0), width), height);
 #pragma omp parallel for num_threads(8)
 for (unsigned y = 0; y < height; ++y)
  for (unsigned x = 0; x < width; ++x)
    {
      redt[x][y] = red[y][x];
      bluet[x][y] = blue[y][x];
      greent[x][y] = green[y][x];
    }
  red = redt;
  blue = bluet;
  green = greent;
}

void im::shift()
{
  std::cout << "Shift" << std::endl;
  std::valarray<std::valarray<Complex>> reds(std::valarray<Complex>(Complex(0,0), width), height);
  std::valarray<std::valarray<Complex>> blues(std::valarray<Complex>(Complex(0,0), width), height);
  std::valarray<std::valarray<Complex>> greens(std::valarray<Complex>(Complex(0,0), width), height);
  #pragma omp parallel for num_threads(8)
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
    {
      reds[(x + width / 2) % width][(y + height / 2) % height] = red[y][x];
      blues[(x + width / 2) % width][(y + height / 2) % height] = blue[y][x];
      greens[(x + width / 2) % width][(y + height / 2) % height] = green[y][x];
    }
  red = reds;
  blue = blues;
  green = greens;
}

Complex im::compress(Complex& c, double m, double compression)
{
  if (std::abs(c) < m*compression) {
    c = 0;
    return 0;
  }
  return c;
}

void im::compression(double ratio)
{
  std::cout << "Compress" << std::endl;
  double max = 0;

  #pragma omp parallel for num_threads(8)
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
      if (std::abs(red[y][x]) > max)
        max = std::abs(red[y][x]);

  #pragma omp parallel for num_threads(8)
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
      for (int i = 0; i < 3; ++i) 
        compress((*components[i])[y][x], max, (double) ratio); // ! double 
}

uint8_t clamp(double n) {
  if (n < 0)
    return 0;
  if (n > 255)
    return 255;
  return n;
}

void im::imsave(const std::string& filename)
{
  std::ofstream file(filename, std::ios::out);
  if (!file.good())
    throw std::invalid_argument("Can not write to " + filename);
  file << "P6\n" << width << " " << height << " 255\n";
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
      for (int i = 0; i < 3; ++i) 
        file << (uint8_t) clamp((*components[i])[y][x].real());
      
  file.close();
}
