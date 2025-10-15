#include <core/game.hpp>

#include <utils/bmp_reader.hpp>

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
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
  {
    printf("Error: %s\n", SDL_GetError());
    return false;
  }

  // Get display size
  SDL_DisplayID displayID = SDL_GetPrimaryDisplay();
  const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(displayID);

  if (!mode)
  {
    printf("Failed to get display mode: %s\n", SDL_GetError());
    return false;
  }
  int window_width = mode->w;
  int window_height = mode->h;
  std::cout << "window size: " << window_width << "x" << window_height << std::endl;

  // Setup window
  window = SDL_CreateWindow("Doom-like Engine",
                            WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_MAXIMIZED);

  if (!window)
  {
    printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
    SDL_Quit();
    return false;
  }

  sdlRenderer = SDL_CreateRenderer(window, nullptr);
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

  // Inicializa o ImGui com SDL3
  ImGui_ImplSDL3_InitForSDLRenderer(window, sdlRenderer);
  ImGui_ImplSDLRenderer3_Init(sdlRenderer);

  isRunning = true;

  // Inicializa a cena
  scene = std::make_unique<Scene>();

  // @todo
  // Esse 200 é devido ao offset do layout (precisa encontrar um jeito de determinar a viewport)
  // de maneira automática
  // o jeito mais simples é fazer variáveis que controlam isso e atualizar elas
  // e atualizar na cena ao ser redimensionado
  scene->min_viewport = {200.0f, 0.0f};
  scene->max_viewport = {static_cast<float>(WINDOW_WIDTH + 200.0f), static_cast<float>(WINDOW_HEIGHT)};

  scene->player = &player;

  player.position = {0.f, 0.f, 20.0f};
  player.target = {0.0f, 0.0f, -1.0f};

  scene->add_objects(cube(Vec3f(), "../assets/redbrick.bmp"));
  // scene->add_objects(ground(3.0f, -3.0f));

  scene->wireframe = true;
  scene->illumination_mode = Scene::IlluminationMode::FLAT;

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
  const float moveSpeed = 1.0f;
  const float rotSpeed = 0.2f;

  while (SDL_PollEvent(&event))
  {
    ImGui_ImplSDL3_ProcessEvent(&event);
    inputHandler.handleEvent(&event);

    if (event.type == SDL_EVENT_QUIT)
      isRunning = false;

    if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
      isRunning = false;

    if (event.type == SDL_EVENT_KEY_DOWN)
    {
      Vec3f newPos;
      SDL_Keycode key = event.key.key;

      switch (key)
      {
      case SDLK_W:
        newPos = player.PlayerPretendingPosition('w', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveForward(moveSpeed, true);
        break;

      case SDLK_S:
        newPos = player.PlayerPretendingPosition('s', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveForward(-moveSpeed, true);
        break;

      case SDLK_A:
        newPos = player.PlayerPretendingPosition('a', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveRight(-moveSpeed, true);
        break;

      case SDLK_D:
        newPos = player.PlayerPretendingPosition('d', moveSpeed, true);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveRight(moveSpeed, true);
        break;

      case SDLK_SPACE:
        newPos = player.PlayerPretendingPosition('u', moveSpeed, false);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveUp(moveSpeed);
        break;

      case SDLK_LCTRL:
        newPos = player.PlayerPretendingPosition('j', moveSpeed, false);
        if (!scene->checkPlayerCollision(newPos))
          player.PlayerMoveUp(-moveSpeed);
        break;

      case SDLK_LEFT:
        player.PlayerRotateYaw(-rotSpeed);
        break;

      case SDLK_RIGHT:
        player.PlayerRotateYaw(rotSpeed);
        break;

      case SDLK_UP:
        player.PlayerRotatePitch(rotSpeed);
        break;

      case SDLK_DOWN:
        player.PlayerRotatePitch(-rotSpeed);
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
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  // Limpa tela
  SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255);
  SDL_RenderClear(sdlRenderer);

  // ============================
  // Viewport principal
  // ============================
  ImGui::SetNextWindowPos(ImVec2(200, 0)); // desloca a viewport para a direita, deixando espaço para o player e config
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
  // Player Info (lado esquerdo, topo)
  // ============================
  if (scene && scene->player)
  {
    const Vec3f &pos = scene->player->position;
    const Vec3f &target = scene->player->target;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, 200));
    ImGui::SetNextWindowBgAlpha(0.4f);

    ImGui::Begin("Player Info", nullptr,
                 ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Posição do Player:");
    ImGui::Separator();
    ImGui::Text("X: %.2f", pos.x);
    ImGui::Text("Y: %.2f", pos.y);
    ImGui::Text("Z: %.2f", pos.z);

    ImGui::Spacing();
    ImGui::Text("Target (lookAt):");
    ImGui::Separator();
    ImGui::Text("X: %.2f", target.x);
    ImGui::Text("Y: %.2f", target.y);
    ImGui::Text("Z: %.2f", target.z);

    ImGui::End();
  }

  // ============================
  // Configurações da cena (abaixo do Player Info)
  // ============================
  if (scene)
  {
    ImGui::SetNextWindowPos(ImVec2(0, 200), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(200, WINDOW_HEIGHT - 200));
    ImGui::SetNextWindowBgAlpha(0.4f);

    ImGui::Begin("Scene Settings", nullptr,
                 ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoCollapse);

    // Seleção de iluminação
    ImGui::Text("Iluminação:");
    const char *illum_modes[] = {"FLAT", "GOURAUD", "PHONG", "TEXTURED", "NO ILLUMINATION"};
    static int current_mode = static_cast<int>(scene->illumination_mode);
    if (ImGui::Combo("Mode", &current_mode, illum_modes, IM_ARRAYSIZE(illum_modes)))
    {
      scene->illumination_mode = static_cast<Scene::IlluminationMode>(current_mode);
    }

    ImGui::Checkbox("Wireframe", &scene->wireframe);

    // ============================
    // Controles Arcball sem mouse (checkbox + valor fixo)
    // ============================
    ImGui::Separator();
    ImGui::Text("Arcball Control:");

    const float fixedAngleX = 0.05f; // valor fixo de rotação no eixo X
    const float fixedAngleY = 0.05f; // valor fixo de rotação no eixo Y

    // Desenha os checkboxes
    ImGui::Checkbox("Rotate +X", &arcballPosX);
    ImGui::Checkbox("Rotate -X", &arcballNegX);
    ImGui::Checkbox("Rotate +Y", &arcballPosY);
    ImGui::Checkbox("Rotate -Y", &arcballNegY);

    // Calcula rotação acumulada
    float deltaX = 0.0f;
    float deltaY = 0.0f;

    if (arcballPosX)
      deltaX += fixedAngleX;
    if (arcballNegX)
      deltaX -= fixedAngleX;
    if (arcballPosY)
      deltaY += fixedAngleY;
    if (arcballNegY)
      deltaY -= fixedAngleY;

    // Aplica a rotação somente se houver algum delta
    if (deltaX != 0.0f || deltaY != 0.0f)
    {
      scene->player->moveArcball(deltaX, deltaY);
    }
    ImGui::End();
  }

  // ============================
  // Renderiza o ImGui
  // ============================
  ImGui::Render();
  SDL_SetRenderDrawColor(sdlRenderer, 114, 144, 154, 255);
  SDL_RenderClear(sdlRenderer);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), sdlRenderer);
  SDL_RenderPresent(sdlRenderer);
}

void Game::update()
{
  if (scene)
    scene->apply_pipeline();
}

void Game::shutdown()
{
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
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
