#include "image.hh"
#include "FFT.hh"

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
  {
    for (unsigned x = 0; x < width; ++x)
    {
      file.read(&tartiflette, 1);
      image.red[y][x].real(static_cast<uint8_t>(tartiflette));
      file.read(&tartiflette, 1);
      image.green[y][x].real(static_cast<uint8_t>(tartiflette));
      file.read(&tartiflette, 1);
      image.blue[y][x].real(static_cast<uint8_t>(tartiflette));

    }
  }
  file.close();


  return image;
}


void im::transpose()
{
 std::valarray<std::valarray<Complex>> redt(std::valarray<Complex>(Complex(0,0), width), height);
 std::valarray<std::valarray<Complex>> bluet(std::valarray<Complex>(Complex(0,0), width), height);
 std::valarray<std::valarray<Complex>> greent(std::valarray<Complex>(Complex(0,0), width), height);

  for (unsigned y = 0; y < height; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
      redt[x][y] = red[y][x];
      bluet[x][y] = blue[y][x];
      greent[x][y] = green[y][x];
    }
  }
  red = redt;
  blue = bluet;
  green = greent;

}

void im::shift()
{
  std::valarray<std::valarray<Complex>> reds(std::valarray<Complex>(Complex(0,0), width), height);
  std::valarray<std::valarray<Complex>> blues(std::valarray<Complex>(Complex(0,0), width), height);
  std::valarray<std::valarray<Complex>> greens(std::valarray<Complex>(Complex(0,0), width), height);
  
  for (unsigned y = 0; y < height; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
    
      reds[(x + width / 2) % width][(y + height / 2) % height] = red[y][x];
      blues[(x + width / 2) % width][(y + height / 2) % height] = blue[y][x];
      greens[(x + width / 2) % width][(y + height / 2) % height] = green[y][x];
    }
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
  double max = 0;
  for (unsigned y = 0; y < height; ++y)
    for (unsigned x = 0; x < width; ++x)
      if (std::abs(red[y][x]) > max)
        max = std::abs(red[y][x]);

  for (unsigned y = 0; y < height; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
      for (int i = 0; i < 3; ++i) {
        compress((*components[i])[y][x], max, ratio));
      }
    }
  }
}

im compres_fftim(im image, double r)
{

  if (r > 1.0)
  {
    std::cout << "r > 1.0" << std::endl;
    exit(1);
    //return image;
  }
  unsigned height = image.height * r;
  //unsigned width = (image.width * r)/2;
  unsigned width = image.width * r;
  im reduc(height , width);

  int dw = (image.width - image.width*r) / 2;
  int dh = (image.height - image.height*r) / 2;


  for (unsigned y = 0; y < height ; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
      for (int i = 0; i < 3; ++i) 
      {
          auto r_comp = reduc.components;
          auto i_comp = image.components;
          (*r_comp[i])[y][x] = (*i_comp[i])[y + dh][x + dw];
      }
    }
  }
  return reduc;
}


im decompres_fftim(im reduc)
{
  //size_t width_size = sup_pow_two(reduc.width*2);
  size_t width_size = sup_pow_two(reduc.width);
  size_t height_size = sup_pow_two(reduc.height);
  std::cout << width_size << std::endl;
  std::cout << height_size << std::endl;
  im decompressed(height_size,width_size);
  
  for(unsigned y = 0; y < reduc.height; y++)
  {
    for(unsigned x = 0; x < reduc.width; x++)
    {
      for (int i = 0; i < 3; ++i) 
      {
          auto r_comp = reduc.components;
          auto d_comp = decompressed.components;
          (*d_comp[i])[y + (decompressed.height - reduc.height) / 2][x + (decompressed.width - reduc.width) / 2] = (*r_comp[i])[y][x];
          //(*d_comp[i])[y + (decompressed.height - reduc.height) / 2][- x + decompressed.width - (decompressed.width - reduc.width) / 2] = (*r_comp[i])[y][x];
      }
    }
  }
  return decompressed;
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

  //double c =  255/log10(max + 1);
  for (unsigned y = 0; y < height; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
      /*
        file << (uint8_t) c * log10(1 + std::abs(red[y][x]))
          << (uint8_t) c * log10(1 + std::abs(green[y][x]))
          << (uint8_t) c * log10(1 + std::abs(blue[y][x]));
      */
      for (int i = 0; i < 3; ++i) {
        //std::cout << compress((*components[i])[y][x], max, 1/5000.0).real() << std::endl;
        file << (uint8_t) clamp((*components[i])[y][x].real());
      }
    }
  }
  file.close();
}


