#include <core/inputHandler.hpp>

InputHandler::InputHandler()
    : moveForward(false), moveBackward(false),
      turnLeft(false), turnRight(false),
      strafeLeft(false), strafeRight(false),
      toggleMoveLook(false) {}

void InputHandler::handleEvent(const SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN)
  {
    switch (event->key.keysym.sym)
    {
    case SDLK_w:
      moveForward = true;
      break;
    case SDLK_s:
      moveBackward = true;
      break;
    case SDLK_a:
      turnLeft = true;
      break;
    case SDLK_d:
      turnRight = true;
      break;
    case SDLK_m:
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
  else if (event->type == SDL_KEYUP)
  {
    switch (event->key.keysym.sym)
    {
    case SDLK_w:
      moveForward = false;
      break;
    case SDLK_s:
      moveBackward = false;
      break;
    case SDLK_a:
      turnLeft = false;
      break;
    case SDLK_d:
      turnRight = false;
      break;
    case SDLK_m:
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
  // Could be used for frame-based input processing
}