#include <utils/bmp_reader.hpp>
#include <iostream>
#include <stdexcept>

// ===================================================
// Implementação baseada em SDL3
// ===================================================

BMPImage bmp::load(const std::string &filename)
{
  // Carrega o arquivo BMP
  SDL_Surface *surface = SDL_LoadBMP(filename.c_str());
  if (!surface)
    throw std::runtime_error("Erro ao carregar BMP via SDL: " + std::string(SDL_GetError()));

  // Converter para formato RGBA8888
  SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888;
  SDL_Surface *converted = SDL_ConvertSurface(surface, format);
  SDL_DestroySurface(surface); // SDL3 usa DestroySurface, não FreeSurface

  if (!converted)
    throw std::runtime_error("Falha ao converter BMP para RGBA8888: " + std::string(SDL_GetError()));

  // Cria o objeto BMPImage com os dados da imagem
  BMPImage img;
  img.width = converted->w;
  img.height = converted->h;
  img.data.resize(img.width * img.height);

  const uint8_t *pixels = static_cast<uint8_t *>(converted->pixels);
  const int pitch = converted->pitch;

  // Copia os pixels RGBA para o vetor
  for (int y = 0; y < img.height; ++y)
  {
    const uint8_t *row = pixels + y * pitch;
    for (int x = 0; x < img.width; ++x)
    {
      int i = y * img.width + x;
      img.data[i] = {
          row[x * 4 + 0], // R
          row[x * 4 + 1], // G
          row[x * 4 + 2], // B
          row[x * 4 + 3]  // A
      };
    }
  }

  SDL_DestroySurface(converted);
  return img;
}

// ===================================================
// Criação da textura SDL3 a partir da imagem BMP
// ===================================================
SDL_Texture *bmp::createTextureFromBMP(SDL_Renderer *renderer, const BMPImage &img)
{
  SDL_Texture *texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGBA8888,
      SDL_TEXTUREACCESS_STATIC,
      img.width,
      img.height);

  if (!texture)
    throw std::runtime_error("Falha ao criar textura SDL: " + std::string(SDL_GetError()));

  SDL_UpdateTexture(texture, nullptr, img.data.data(), img.width * sizeof(models::Color));
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

  return texture;
}
