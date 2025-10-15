#include <core/inputHandler.hpp>

InputHandler::InputHandler()
    : moveForward(false), moveBackward(false),
      turnLeft(false), turnRight(false),
      strafeLeft(false), strafeRight(false),
      toggleMoveLook(false) {}

void InputHandler::handleEvent(const SDL_Event *event)
{
  if (event->type == SDL_EVENT_KEY_DOWN)
  {
    switch (event->key.key)
    {
    case SDLK_W:
      moveForward = true;
      break;
    case SDLK_S:
      moveBackward = true;
      break;
    case SDLK_A:
      turnLeft = true;
      break;
    case SDLK_D:
      turnRight = true;
      break;
    case SDLK_M:
      toggleMoveLook = true;
      break;
    case SDLK_COMMA:
      strafeRight = true;
      break;
    case SDLK_PERIOD:
      strafeLeft = true;
      break;
    }
  }
  else if (event->type == SDL_EVENT_KEY_UP)
  {
    switch (event->key.key)
    {
    case SDLK_W:
      moveForward = false;
      break;
    case SDLK_S:
      moveBackward = false;
      break;
    case SDLK_A:
      turnLeft = false;
      break;
    case SDLK_D:
      turnRight = false;
      break;
    case SDLK_M:
      toggleMoveLook = false;
      break;
    case SDLK_COMMA:
      strafeRight = false;
      break;
    case SDLK_PERIOD:
      strafeLeft = false;
      break;
    }
  }
}

void InputHandler::update()
{
  // Pode ser usado para processamento de input baseado em frames
}
