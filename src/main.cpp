#include <SDL.h>

#include <core/game.hpp>

#define SDL_main main

int main(int argc, char *argv[])
{
  std::cout << "Iniciando aplicação...\n";

  // Teste básico da SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "FALHA: SDL_Init: %s\n"
              << SDL_GetError() << std::endl;
    return -1;
  }

  std::cout << "SDL inicializada com sucesso\n"
            << std::endl;

  Game game;
  if (!game.initialize())
  {
    std::cout << "FALHA: Game::initialize\n"
              << std::endl;
    SDL_Quit();
    return -1;
  }

  game.run();
  game.shutdown();

  return 0;
}