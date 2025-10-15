#pragma once
#include <string>
#include <vector>
#include <stdexcept>

#include <models/color.hpp>
#include <SDL3/SDL.h>

struct BMPImage
{
  int width;
  int height;
  std::vector<models::Color> data; // Pixels RGBA
};

namespace bmp
{
  BMPImage load(const std::string &filename);
  SDL_Texture *createTextureFromBMP(SDL_Renderer *renderer, const BMPImage &img);
}
