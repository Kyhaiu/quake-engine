#pragma once

#include <SDL.h>

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
  bool moveForward, moveBackward;
  bool turnLeft, turnRight;
  bool strafeLeft, strafeRight;
  bool toggleMoveLook;
};