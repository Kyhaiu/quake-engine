#pragma once

#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui-sdl2/imgui_impl_sdl2.h>
#include <imgui-sdl2/imgui_impl_sdlrenderer2.h>

#include <iostream>
#include <vector>
#include <memory>

#include <core/inputHandler.hpp>
#include <scene/scene.hpp>

class Game
{
public:
  Game();
  ~Game();

  bool initialize();
  void run();
  void shutdown();

private:
  void processInput();
  void update();
  void render();

  SDL_Window *window;
  SDL_Renderer *sdlRenderer;
  std::unique_ptr<Scene> scene;
  InputHandler inputHandler;
  Player player;

  bool isRunning;

  // Constants
  static const int SCREEN_WIDTH = 160;
  static const int SCREEN_HEIGHT = 120;
  static const int PIXEL_SCALE = 4;
  static const int WINDOW_WIDTH = SCREEN_WIDTH * PIXEL_SCALE;
  static const int WINDOW_HEIGHT = SCREEN_HEIGHT * PIXEL_SCALE;
  static const int NUM_SECTORS = 4;
  static const int NUM_WALLS = 16;
};