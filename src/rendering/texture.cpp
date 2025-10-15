#include <models/texture.hpp>

// bool models::loadTexture(const std::string &filename, Texture &tex)
// {
//   SDL_Surface *surface = IMG_Load(filename.c_str());
//   if (!surface)
//   {
//     std::cout << "Erro ao carregar textura: %s\n"
//               << IMG_GetError() << std::endl;
//     return false;
//   }

//   tex.width = surface->w;
//   tex.height = surface->h;

//   // Redimensiona o vetor 2D de pixels
//   tex.pixels.resize(tex.height, std::vector<models::Color>(tex.width));

//   SDL_LockSurface(surface);

//   Uint8 *src = (Uint8 *)surface->pixels;
//   int bpp = surface->format->BytesPerPixel;

//   for (int y = 0; y < tex.height; y++)
//   {
//     for (int x = 0; x < tex.width; x++)
//     {
//       Uint8 r, g, b, a = 255;

//       Uint8 *p = src + y * surface->pitch + x * bpp;

//       // Dependendo do formato da superfície SDL
//       SDL_GetRGBA(*(Uint32 *)p, surface->format, &r, &g, &b, &a);

//       tex.pixels[y][x] = {r, g, b, a};
//     }
//   }

//   SDL_UnlockSurface(surface);
//   SDL_FreeSurface(surface);

//   return true;
// }