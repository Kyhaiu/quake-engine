#include <SDL3/SDL.h>
#include <core/game.hpp>

int main(int argc, char *argv[])
{
  std::cout << "Iniciando aplicação...\n";

  // SDL3 usa SDL_InitSubSystem() ou apenas SDL_Init() da mesma forma
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    std::cerr << "FALHA: SDL_Init: " << SDL_GetError() << std::endl;
    return -1;
  }

  std::cout << "SDL inicializada com sucesso\n"
            << std::endl;

  Game game;
  if (!game.initialize())
  {
    std::cerr << "FALHA: Game::initialize" << std::endl;
    SDL_Quit();
    return -1;
  }

  game.run();
  game.shutdown();

  SDL_Quit();
  return 0;
}
