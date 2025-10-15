#pragma once

#include <SDL3/SDL.h>

class InputHandler
{
public:
  InputHandler();

  void handleEvent(const SDL_Event *event);
  void update();

  // Input state queries
  bool isMovingForward() const { return moveForward; }
  bool isMovingBackward() const { return moveBackward; }
  bool isTurningLeft() const { return turnLeft; }
  bool isTurningRight() const { return turnRight; }
  bool isStrafingLeft() const { return strafeLeft; }
  bool isStrafingRight() const { return strafeRight; }
  bool isToggleMoveLook() const { return toggleMoveLook; }

private:
  bool moveForward = false;
  bool moveBackward = false;
  bool turnLeft = false;
  bool turnRight = false;
  bool strafeLeft = false;
  bool strafeRight = false;
  bool toggleMoveLook = false;
};
