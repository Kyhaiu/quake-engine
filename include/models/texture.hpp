#pragma once
#include <iostream>
#include <vector>
#include <string>

#include <algorithm>
#include <models/color.hpp>
#include <utils/bmp_reader.hpp> // usa seu módulo BMPReader
#include <stdexcept>

namespace models
{
  struct Texture
  {
    int width = 0;
    int height = 0;
    std::vector<std::vector<models::Color>> pixels; // pixels[y][x]

    Color sample(float u, float v) const
    {
      int x = std::clamp(int(u * (width - 1)), 0, width - 1);
      int y = std::clamp(int(v * (height - 1)), 0, height - 1);
      return pixels[y][x];
    }
  };

  // Carrega uma textura BMP via bmp_reader e preenche o Texture
  bool loadTexture(const std::string &filename, Texture &tex);
}
