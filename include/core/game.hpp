#pragma once

#include <SDL3/SDL.h>
#include <imgui/imgui.h>
#include <imgui-sdl3/imgui_impl_sdl3.h>
#include <imgui-sdl3/imgui_impl_sdlrenderer3.h>

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

  SDL_Window *window = nullptr;
  SDL_Renderer *sdlRenderer = nullptr;
  std::unique_ptr<Scene> scene;
  InputHandler inputHandler;
  Player player;

  bool isRunning = false;

  // ===========================
  // Estados do Arcball Control
  // ===========================
  bool arcballPosX = false; // rotação positiva no eixo X
  bool arcballNegX = false; // rotação negativa no eixo X
  bool arcballPosY = false; // rotação positiva no eixo Y
  bool arcballNegY = false; // rotação negativa no eixo Y

  // Constantes
  static constexpr int SCREEN_WIDTH = 160;
  static constexpr int SCREEN_HEIGHT = 120;
  static constexpr int PIXEL_SCALE = 4;
  static constexpr int WINDOW_WIDTH = SCREEN_WIDTH * PIXEL_SCALE;
  static constexpr int WINDOW_HEIGHT = SCREEN_HEIGHT * PIXEL_SCALE;
  static constexpr int NUM_SECTORS = 4;
  static constexpr int NUM_WALLS = 16;
};
