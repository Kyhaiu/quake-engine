#include <core/game.hpp>

#include "../models/cube.cpp"
#include "../models/ground.cpp"

Game::Game()
    : window(nullptr), sdlRenderer(nullptr), isRunning(false) {}

Game::~Game()
{
  shutdown();
}

bool Game::initialize()
{
  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
  {
    printf("Error: %s\n", SDL_GetError());
    return false;
  }

  // Get window size
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  int window_width = dm.w;
  int window_height = dm.h;
  std::cout << "window size: " << window_width << "x" << window_height << std::endl;

  // Setup window
  SDL_WindowFlags window_flags =
      (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);

  window = SDL_CreateWindow("Doom-like Engine",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);

  if (!window)
  {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  sdlRenderer = SDL_CreateRenderer(window, -1,
                                   SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!sdlRenderer)
  {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return false;
  }

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Inicializa o ImGui com SDL
  ImGui_ImplSDL2_InitForSDLRenderer(window, sdlRenderer);
  ImGui_ImplSDLRenderer2_Init(sdlRenderer);

  isRunning = true;

  // Inicializa a cena
  scene = std::make_unique<Scene>();

  scene->min_viewport = {0.0f, 0.0f};
  scene->max_viewport = {static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)};

  scene->player = &player;

  player.position = {10.f, 10.f, 20.0f};
  player.target = {0.0f, 0.0f, -1.0f};

  scene->add_objects(cube(Vec3f()));
  // scene->add_objects(ground(3.0f, -3.0f));

  return true;
}

void Game::run()
{
  while (isRunning)
  {
    processInput();
    update();
    render();
  }
}

void Game::processInput()
{
  SDL_Event event;
  const float moveSpeed = 1.0f;  // Velocidade de deslocamento
  const float rotSpeed = 0.005f; // Velocidade de rotação (radianos)

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    inputHandler.handleEvent(&event);

    if (event.type == SDL_QUIT)
      isRunning = false;

    if (event.type == SDL_WINDOWEVENT &&
        event.window.event == SDL_WINDOWEVENT_CLOSE &&
        event.window.windowID == SDL_GetWindowID(window))
      isRunning = false;

    // ========================
    // Movimento do Player
    // ========================
    if (event.type == SDL_KEYDOWN)
    {
      Vec3f newPos;

      switch (event.key.keysym.sym)
      {
      // Frente
      case SDLK_w:
        newPos = player.PlayerPretendingPosition('w', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveForward(moveSpeed, true);
        break;

      // Trás
      case SDLK_s:
        newPos = player.PlayerPretendingPosition('s', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveForward(-moveSpeed, true);
        break;

      // Esquerda
      case SDLK_a:
        newPos = player.PlayerPretendingPosition('a', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveRight(-moveSpeed, true);
        break;

      // Direita
      case SDLK_d:
        newPos = player.PlayerPretendingPosition('d', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveRight(moveSpeed, true);
        break;

      // Subir
      case SDLK_SPACE:
        newPos = player.PlayerPretendingPosition('u', moveSpeed, false);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveUp(moveSpeed);
        break;

      // Descer
      case SDLK_LCTRL:
        newPos = player.PlayerPretendingPosition('j', moveSpeed, false);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveUp(-moveSpeed);
        break;

      // Rotação com setas
      case SDLK_LEFT:
        player.PlayerRotateYaw(-rotSpeed); // vira para esquerda
        break;
      case SDLK_RIGHT:
        player.PlayerRotateYaw(rotSpeed); // vira para direita
        break;
      case SDLK_UP:
        player.PlayerRotatePitch(rotSpeed); // olha para cima
        break;
      case SDLK_DOWN:
        player.PlayerRotatePitch(-rotSpeed); // olha para baixo
        break;

      default:
        break;
      }
    }
  }
}

void Game::render()
{
  // Início do frame ImGui
  ImGui_ImplSDLRenderer2_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  // Limpa tela
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);

  // ============================
  // Viewport principal da cena
  // ============================
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT));

  ImGui::Begin("Viewport", nullptr,
               ImGuiWindowFlags_AlwaysAutoResize |
                   ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse);

  if (scene)
  {
    ImDrawList *draw_list = ImGui::GetForegroundDrawList();
    pipeline::DrawBuffer(draw_list, scene->z_buffer, scene->color_buffer, scene->min_viewport);
  }

  ImGui::End();

  // ============================
  // HUD de debug com posição do player
  // ============================
  if (scene && scene->player)
  {
    const Vec3f &pos = scene->player->position;

    ImGui::SetNextWindowPos(ImVec2(700, 20), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.4f); // Transparência
    ImGui::Begin("Player Info", nullptr,
                 ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Text("Posição do Player:");
    ImGui::Separator();
    ImGui::Text("X: %.2f", pos.x);
    ImGui::Text("Y: %.2f", pos.y);
    ImGui::Text("Z: %.2f", pos.z);

    ImGui::End();
  }

  // ============================
  // Renderiza o ImGui
  // ============================
  ImGui::Render();
  SDL_SetRenderDrawColor(sdlRenderer, 114, 144, 154, 255);
  SDL_RenderClear(sdlRenderer);
  ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
  SDL_RenderPresent(sdlRenderer);
}

void Game::update()
{
  if (scene)
    // scene->apply_pipeline_flat();
    // scene->apply_pipeline_gouraud();
    scene->apply_pipeline_phong();
}

void Game::shutdown()
{
  ImGui_ImplSDLRenderer2_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  if (sdlRenderer)
  {
    SDL_DestroyRenderer(sdlRenderer);
    sdlRenderer = nullptr;
  }

  if (window)
  {
    SDL_DestroyWindow(window);
    window = nullptr;
  }

  SDL_Quit();
}
