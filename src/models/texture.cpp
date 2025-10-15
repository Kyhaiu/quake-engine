#include <models/texture.hpp>

namespace models
{
  bool loadTexture(const std::string &filename, Texture &tex)
  {
    try
    {
      // Usa bmp_reader para carregar BMP em 1D
      BMPImage bmp = bmp::load(filename);

      tex.width = bmp.width;
      tex.height = bmp.height;
      tex.pixels.resize(tex.height, std::vector<Color>(tex.width));

      // Converte do vetor 1D (bmp.data) para 2D (pixels[y][x])
      for (int y = 0; y < tex.height; ++y)
      {
        for (int x = 0; x < tex.width; ++x)
        {
          tex.pixels[y][x] = bmp.data[y * tex.width + x];
        }
      }

      return true;
    }
    catch (const std::exception &e)
    {
      std::cerr << "Erro ao carregar textura '" << filename << "': " << e.what() << "\n";
      return false;
    }
  }
}
