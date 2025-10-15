#include <scene/scene.hpp>

/**
 * @brief Construtor padrão da classe Scene
 *
 * @note Este construtor cria uma câmera(Player) padrão e uma lista vazia de objetos
 * @note A câmera(player) padrão é criada com os seguintes parâmetros:
 * @note - posição: (0.0f, 0.0f, 20.0f)
 * @note - alvo: (0.0f, 0.0f, 0.0f)
 * @note - vetor up: (0.0f, 1.0f, 0.0f)
 * @note - d: 20.0f
 * @note - viewport: min(0.0f, 0.0f) e max(640.0f, 480.0f)
 * @note - window: min(-3.0f, -3.0f) e max(3.0f, 3.0f)
 */
Scene::Scene()
{
  player = new Player();
  objects = std::vector<Mesh *>();
  min_viewport = {0.0f, 0.0f};
  max_viewport = {640.f, 480.0f};
  min_window = {-3.0f, -3.0f};
  max_window = {3.0f, 3.0f};

  global_light = models::GlobalLight();

  models::Omni omni;

  omni.id = "omni_1";
  omni.intensity = models::ColorToChannels(models::WHITE);
  omni.position = Vec3f{10.0f, 10.0f, 10.0f};

  omni_lights.push_back(omni);
}

/**
 * @brief Destrutor da classe Scene
 *
 * @note Este destrutor libera a memória alocada para a câmera e para os objetos
 */
Scene::~Scene()
{
  delete this->player;
  for (auto object : this->objects)
  {
    delete object;
  }
}

void Scene::initialize_buffers()
{
  int width = static_cast<int>(this->max_viewport.x + 1);
  int height = static_cast<int>(this->max_viewport.y + 1);

  // Limpa os buffers
  this->z_buffer.clear();
  this->color_buffer.clear();

  // Redimensiona e inicializa os buffers
  this->z_buffer = std::vector<std::vector<float>>(width, std::vector<float>(height, std::numeric_limits<float>::infinity()));
  this->color_buffer = std::vector<std::vector<models::Color>>(width, std::vector<models::Color>(height, models::TRANSPARENT));
}

/**
 * @brief Adiciona um objeto à cena
 *
 * @param object Ponteiro para o objeto a ser adicionado
 */
void Scene::add_objects(Mesh *object)
{
  object->computeBounds();
  objects.push_back(object);
}

/**
 * @brief Remove um objeto da cena
 *
 * @param object Ponteiro para o objeto a ser removido
 */
void Scene::remove_object(Mesh *object)
{
  for (auto it = this->objects.begin(); it != this->objects.end(); it++)
  {
    if (*it == object)
    {
      this->objects.erase(it);
      break;
    }
  }
}

/**
 * @brief Pré computação da visibilidade dos objetos
 *
 * @note Se o objeto estiver dentro do volume de visualização (near e far) ele será exibido
 * @note Nos pipelines convencionais o recorte é feito no espaço 3D, porém
 *       como o pipeline utilizado é uma simplificação ele faz o recorte no 2D após
 *       mapear as coordenadas para tela (SRT). Em suma, como o pipeline é simplificado
 *       ele não utiliza o volume de visualização, mas precisamos lidar com os objetos que
 *       estão fora do volume de visualização (costas do jogador).
 *
 */
void Scene::clipping()
{

  for (auto object : objects)
  {
    Vec3f centroid = object->getCentroid();
    // VRP (View referece point)posição do jogador
    Vec3f vrp = player->position;
    // lugar para onde ele está olhando
    Vec3f target = player->target;

    // vetor direção da camera
    Vec3f player_forward = Vector3Normalize(target - vrp);
    // vetor de direção do centroid
    Vec3f centroid_to_camera = centroid - vrp;

    // Retorna o angulo entre esses dois vetores
    float depth = Vector3DotProduct(player_forward, centroid_to_camera);

    // Verifica se está fora do near/far
    if (depth < player->near || depth > player->far)
      object->is_visible = false;
    else
      object->is_visible = true;
  }
}

void Scene::apply_pipeline()
{
  switch (illumination_mode)
  {
  case IlluminationMode::FLAT:
    apply_pipeline_flat();
    break;
  case IlluminationMode::GOURAUD:
    apply_pipeline_gouraud();
    break;
  case IlluminationMode::PHONG:
    apply_pipeline_phong();
    break;
  case IlluminationMode::TEXTURED:
    apply_pipeline_texture();
    break;
  case IlluminationMode::NO_ILLUMINATION:
    // pode chamar flat com cores neutras ou aplicar apenas wireframe
    break;
  }

  if (wireframe)
  {
    // Aqui você pode percorrer os objetos e chamar pipeline::DrawLineBuffer ou similar
    for (auto obj : objects)
    {
      for (auto face : obj->faces)
      {
        if (!face->visible)
          continue;

        std::vector<Vec3f> vertexes;
        HalfEdge *he = face->he;
        do
        {
          vertexes.push_back(he->origin->vertex_screen);
          he = he->next;
        } while (he != face->he);

        pipeline::DrawLineBuffer(vertexes, models::WHITE, z_buffer, color_buffer);
      }
    }
  }
}

void Scene::apply_pipeline_flat()
{

  // Faz a pré computação do que está dentro da visão do jogador
  clipping();

  // Obtém as matrizes de transformação
  // Matriz que converte o sistema do universo (SRU) para o sistema de camera (SRC, Visão do player)
  Matrix sru_src_matrix = pipeline::sru_to_src(player->position, player->target);
  // Aplica a projeção (Efeito de perspectiva)
  Matrix projection_matrix = pipeline::projection(player->position, player->target, player->d);
  // Mapeia as coordenadas da camera para a tela
  Matrix viewport_matrix = pipeline::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  // Observação, como o pipeline é simplificado, não é possível inverter as matrizes
  // Isso acontece devido ao volume de visualização não ser normalizado
  // Ou seja, não podemos pegar uma coordenada de tela e voltar para o Universo
  // o máximo que podemos fazer é pegar as coordenadas de tela e voltar para o sistema o SRC (sem projeção)

  // Multiplicação de matrizes

  // Obs.: Como estamos concatenando as matrizes precisamos aplicar na ordem inversa
  // SRC_TO_SRT -> Projeção -> SRU_TO_SRC
  Matrix pipeline_matrix = MatrixMultiply(viewport_matrix, projection_matrix);
  pipeline_matrix = MatrixMultiply(pipeline_matrix, sru_src_matrix);

  // Vetor utilizado na aplicação do pipeline
  Vec4f vectorResult = Vec4f();

  // Rasterização de todos os objetos presentes na cena
  for (auto object : objects)
  {

    // aqui ignoramos os objetos que foram recortados no clipping logo acima!
    if (!object->is_visible)
      continue;

    // aplica o pipeline em todos os vértices do objeto
    for (auto v : object->vertexes)
    {
      vectorResult = MatrixMultiplyVector(pipeline_matrix, v->vertex);

      // Esse fator W (Fator homogêneo) é a perspectiva, quando dividimos X e Y por W
      // colocamos o objeto em perspectiva
      // Como Z é a profundidade, não precismos fazer nada, "já está em perspectiva".
      v->vertex_screen = {vectorResult.x / vectorResult.w,
                          vectorResult.y / vectorResult.w,
                          vectorResult.z};
    }

    // Determina a visibilidade de cada face
    // A visibilidade é determinada por back culling (faces voltados para longe da câmera)
    for (auto face : object->faces)
    {
      face->visible = face->is_visible(player->position);
    }
  }

  // Inicializa os buffers
  initialize_buffers();

  for (auto object : objects)
  {
    // Ignora objetos não visíveis
    if (!object->is_visible)
      continue;

    for (auto face : object->faces)
    {
      if (!face->visible)
        continue;

      HalfEdge *he = face->he;

      // coordenadas de tela
      std::vector<Vec3f> vertexes;

      // percore os vértices no sentido anti-horário
      while (true)
      {
        vertexes.push_back(he->origin->vertex_screen);

        he = he->next;
        if (he == face->he)
          break;
      }

      // O vetor normal da face é calculado na ocultação de faces
      // precisa recortar o vetor normal do vértice também (assim simplifica o calculo da interpolação)
      vertexes = pipeline::clip_2D_polygon(vertexes, min_viewport, max_viewport);

      // Se o vetor de vertices for menor que 3, não é possível formar um polígono, então não rasteriza.
      if (vertexes.size() < 3)
        continue;

      pipeline::fill_polygon_flat(vertexes, global_light, omni_lights, player->position, face->centroid, face->normal, object->material, z_buffer, color_buffer);
    }
  }

  // Resetar a clipping flag de cada vértice para a próxima iteração
  for (auto object : objects)
    object->is_visible = true;
}

void Scene::apply_pipeline_gouraud()
{
  // Faz a pré computação do que está dentro da visão do jogador
  clipping();

  // Obtém as matrizes de transformação
  // Matriz que converte o sistema do universo (SRU) para o sistema de camera (SRC, Visão do player)
  Matrix sru_src_matrix = pipeline::sru_to_src(player->position, player->target);
  // Aplica a projeção (Efeito de perspectiva)
  Matrix projection_matrix = pipeline::projection(player->position, player->target, player->d);
  // Mapeia as coordenadas da camera para a tela
  Matrix viewport_matrix = pipeline::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  // Observação, como o pipeline é simplificado, não é possível inverter as matrizes
  // Isso acontece devido ao volume de visualização não ser normalizado
  // Ou seja, não podemos pegar uma coordenada de tela e voltar para o Universo
  // o máximo que podemos fazer é pegar as coordenadas de tela e voltar para o sistema o SRC (sem projeção)

  // Multiplicação de matrizes

  // Obs.: Como estamos concatenando as matrizes precisamos aplicar na ordem inversa
  // SRC_TO_SRT -> Projeção -> SRU_TO_SRC
  Matrix pipeline_matrix = MatrixMultiply(viewport_matrix, projection_matrix);
  pipeline_matrix = MatrixMultiply(pipeline_matrix, sru_src_matrix);

  // Vetor utilizado na aplicação do pipeline
  Vec4f vectorResult = Vec4f();

  // Rasterização de todos os objetos presentes na cena
  for (auto object : objects)
  {

    // aqui ignoramos os objetos que foram recortados no clipping logo acima!
    if (!object->is_visible)
      continue;

    // aplica o pipeline em todos os vértices do objeto
    for (auto v : object->vertexes)
    {
      vectorResult = MatrixMultiplyVector(pipeline_matrix, v->vertex);

      // Esse fator W (Fator homogêneo) é a perspectiva, quando dividimos X e Y por W
      // colocamos o objeto em perspectiva
      // Como Z é a profundidade, não precismos fazer nada, "já está em perspectiva".
      v->vertex_screen = {vectorResult.x / vectorResult.w,
                          vectorResult.y / vectorResult.w,
                          vectorResult.z};
    }

    // Determina a visibilidade de cada face
    // A visibilidade é determinada por back culling (faces voltados para longe da câmera)
    for (auto face : object->faces)
    {
      face->visible = face->is_visible(player->position);
    }

    // Nos sombreamentos Gouraud e Phong, precisamos calcular uma normal unitária em cada vértice.
    // Para isso, pegamos as normais das faces que compartilham o mesmo vértice e calculamos sua média.
    // Essa média define a orientação "suave" da superfície naquele ponto.
    // Diferente do sombreamento Flat, onde a cor é calculada por face, aqui a cor depende das normais
    // de cada vértice (Gouraud) ou de cada pixel (Phong), permitindo transições suaves entre as faces.
    object->determineVertexNormals();
  }

  // Inicializa os buffers
  initialize_buffers();

  for (auto object : objects)
  {
    // Ignora objetos não visíveis
    if (!object->is_visible)
      continue;

    for (auto face : object->faces)
    {
      if (!face->visible)
        continue;

      HalfEdge *he = face->he;

      // first: coordenadas de tela
      // second: normal do vértice
      std::vector<std::pair<Vec3f, Vec3f>> vertexes;

      // percore os vértices no sentido anti-horário
      while (true)
      {
        vertexes.push_back(std::make_pair(he->origin->vertex_screen, he->origin->normal));

        he = he->next;
        if (he == face->he)
          break;
      }

      // No gouraud a cor é calculada antes do recorte, pois é determinada em cada vértice
      // pois quando formos recortar, precisaremos interpolar corretamente a cor para o ponto do recorte
      std::vector<std::pair<Vec3f, models::Color>> vertexes_gouraud;

      // Posição da camera (player)
      Vec3f eye = player->position;
      // Material do objeto
      models::Material object_material = object->material;

      // Calcula a cor para cada vértice do objeto
      for (auto vertex : vertexes)
      {
        Vec3f vert = vertex.first;
        Vec3f normal_vert = vertex.second;
        models::Color color = models::GouraudShading(global_light, omni_lights, std::make_pair(vert, normal_vert), eye, object_material);
        vertexes_gouraud.push_back(std::make_pair(vert, color));
      }

      // O vetor normal da face é calculado na ocultação de faces
      // precisa recortar o vetor normal do vértice também (assim simplifica o calculo da interpolação)
      vertexes_gouraud = pipeline::clip_2D_polygon(vertexes_gouraud, min_viewport, max_viewport);

      // Se o vetor de vertices for menor que 3, não é possível formar um polígono, então não rasteriza.
      if (vertexes_gouraud.size() < 3)
        continue;

      pipeline::fill_polygon_gourand(vertexes_gouraud, z_buffer, color_buffer);
    }
  }

  // Resetar a clipping flag de cada vértice para a próxima iteração
  for (auto object : objects)
    object->is_visible = true;
}

void Scene::apply_pipeline_phong()
{
  // Faz a pré computação do que está dentro da visão do jogador
  clipping();

  // Obtém as matrizes de transformação
  // Matriz que converte o sistema do universo (SRU) para o sistema de camera (SRC, Visão do player)
  Matrix sru_src_matrix = pipeline::sru_to_src(player->position, player->target);
  // Aplica a projeção (Efeito de perspectiva)
  Matrix projection_matrix = pipeline::projection(player->position, player->target, player->d);
  // Mapeia as coordenadas da camera para a tela
  Matrix viewport_matrix = pipeline::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  // Observação, como o pipeline é simplificado, não é possível inverter as matrizes
  // Isso acontece devido ao volume de visualização não ser normalizado
  // Ou seja, não podemos pegar uma coordenada de tela e voltar para o Universo
  // o máximo que podemos fazer é pegar as coordenadas de tela e voltar para o sistema o SRC (sem projeção)

  // Multiplicação de matrizes

  // Obs.: Como estamos concatenando as matrizes precisamos aplicar na ordem inversa
  // SRC_TO_SRT -> Projeção -> SRU_TO_SRC
  Matrix pipeline_matrix = MatrixMultiply(viewport_matrix, projection_matrix);
  pipeline_matrix = MatrixMultiply(pipeline_matrix, sru_src_matrix);

  // Vetor utilizado na aplicação do pipeline
  Vec4f vectorResult = Vec4f();

  // Rasterização de todos os objetos presentes na cena
  for (auto object : objects)
  {

    // aqui ignoramos os objetos que foram recortados no clipping logo acima!
    if (!object->is_visible)
      continue;

    // aplica o pipeline em todos os vértices do objeto
    for (auto v : object->vertexes)
    {
      vectorResult = MatrixMultiplyVector(pipeline_matrix, v->vertex);

      // Esse fator W (Fator homogêneo) é a perspectiva, quando dividimos X e Y por W
      // colocamos o objeto em perspectiva
      // Como Z é a profundidade, não precismos fazer nada, "já está em perspectiva".
      v->vertex_screen = {vectorResult.x / vectorResult.w,
                          vectorResult.y / vectorResult.w,
                          vectorResult.z};
    }

    // Determina a visibilidade de cada face
    // A visibilidade é determinada por back culling (faces voltados para longe da câmera)
    for (auto face : object->faces)
    {
      face->visible = face->is_visible(player->position);
    }

    // Nos sombreamentos Gouraud e Phong, precisamos calcular uma normal unitária em cada vértice.
    // Para isso, pegamos as normais das faces que compartilham o mesmo vértice e calculamos sua média.
    // Essa média define a orientação "suave" da superfície naquele ponto.
    // Diferente do sombreamento Flat, onde a cor é calculada por face, aqui a cor depende das normais
    // de cada vértice (Gouraud) ou de cada pixel (Phong), permitindo transições suaves entre as faces.
    object->determineVertexNormals();
  }

  // Inicializa os buffers
  initialize_buffers();

  for (auto object : objects)
  {
    // Ignora objetos não visíveis
    if (!object->is_visible)
      continue;

    for (auto face : object->faces)
    {
      if (!face->visible)
        continue;

      HalfEdge *he = face->he;

      // first: coordenadas de tela
      // second: normal do vértice
      std::vector<std::pair<Vec3f, Vec3f>> vertexes;

      // percore os vértices no sentido anti-horário
      while (true)
      {
        vertexes.push_back(std::make_pair(he->origin->vertex_screen, he->origin->normal));

        he = he->next;
        if (he == face->he)
          break;
      }

      // Posição da camera (player)
      Vec3f eye = player->position;
      // Material do objeto
      models::Material object_material = object->material;

      // O vetor normal da face é calculado na ocultação de faces
      // precisa recortar o vetor normal do vértice também (assim simplifica o calculo da interpolação)
      vertexes = pipeline::clip_2D_polygon(vertexes, min_viewport, max_viewport);

      // Se o vetor de vertices for menor que 3, não é possível formar um polígono, então não rasteriza.
      if (vertexes.size() < 3)
        continue;

      pipeline::fill_polygon_phong(vertexes, object->getCentroid(), global_light, omni_lights, eye, object_material, z_buffer, color_buffer);
    }
  }

  // Resetar a clipping flag de cada vértice para a próxima iteração
  for (auto object : objects)
    object->is_visible = true;
}

/**
 @todo Está com problema na estrutura

 O mapeamento de textura é feito por face. Por isso cada vértice de cada face tem seu par (u, v).

 As malhas poligonais esta triangularizando um cubo, mas usando UVs únicas por vértice global.
 Isso funciona para a primeira face, mas para as outras faces os vértices compartilhados têm UVs diferentes
 para cada face — ou seja, o mesmo vértice não pode ter múltiplas UVs.
 Isso é o que causa as distorções nas outras faces.

 O que precisa fazer:

 1. Cada face deve ter seus próprios vértices se usar UVs diferentes, mesmo que a posição 3D seja a mesma.

 2. Não dá para simplesmente atribuir UVs nos vértices globais do cubo, pois um vértice pode estar na face da frente (UV = 0,0) e na face do topo (UV = 1,0) ao mesmo tempo — isso não funciona no pipeline atual.

 Alternativamente pode se fazer:

 1. O que você pode fazer é colocar as tuplas (u, v)
    na mesma sequência com a qual se percorrem os vértices da face.
    Assim, fica uma relação implícita.
    O primeiro par u, v corresponde ao primeiro vértice da face e assim sucessivamente.
 */
void Scene::apply_pipeline_texture()
{
  // Pré-computação do que está dentro da visão do jogador
  clipping();

  // Matrizes de transformação
  Matrix sru_src_matrix = pipeline::sru_to_src(player->position, player->target);
  Matrix projection_matrix = pipeline::projection(player->position, player->target, player->d);
  Matrix viewport_matrix = pipeline::src_to_srt(min_window, min_viewport, max_window, max_viewport, true);

  Matrix pipeline_matrix = MatrixMultiply(viewport_matrix, projection_matrix);
  pipeline_matrix = MatrixMultiply(pipeline_matrix, sru_src_matrix);

  Vec4f vectorResult = Vec4f();

  // Aplica pipeline em todos os objetos
  for (auto object : objects)
  {
    if (!object->is_visible)
      continue;

    // Aplica o pipeline em todos os vértices
    for (auto v : object->vertexes)
    {
      vectorResult = MatrixMultiplyVector(pipeline_matrix, v->vertex);

      v->vertex_screen = {vectorResult.x / vectorResult.w,
                          vectorResult.y / vectorResult.w,
                          vectorResult.z};

      // Garantir que u, v já estão definidos (normalizados entre 0 e 1)
      // Para cubo simples ou UV planar
      if (!v->has_uv)
      {
        v->u = (v->vertex.x + 1.0f) / 2.0f; // mapeia -1..1 -> 0..1
        v->v = (v->vertex.y + 1.0f) / 2.0f;
        v->has_uv = true;
      }
    }

    // Determina visibilidade das faces
    for (auto face : object->faces)
    {
      face->visible = face->is_visible(player->position);
    }
  }

  // Inicializa buffers
  initialize_buffers();

  // Rasterização de cada face
  for (auto object : objects)
  {
    if (!object->is_visible)
      continue;

    for (auto face : object->faces)
    {
      if (!face->visible)
        continue;

      HalfEdge *he = face->he;
      std::vector<Vertex *> vertexes;

      while (true)
      {
        vertexes.push_back(he->origin);
        he = he->next;
        if (he == face->he)
          break;
      }

      // vertexes = pipeline::clip_2D_polygon(vertexes, min_viewport, max_viewport);

      if (vertexes.size() < 3)
        continue;

      // Desenha linhas para depuração
      std::vector<Vec3f> vertex_positions;
      for (auto v : vertexes)
        vertex_positions.push_back(v->vertex_screen);

      pipeline::DrawLineBuffer(vertex_positions, models::CYAN, z_buffer, color_buffer);

      // Preenchimento da face com textura
      pipeline::fill_polygon_texture(vertexes, object->texture, global_light, omni_lights, player->position,
                                     face->centroid, face->normal, object->material, z_buffer, color_buffer);
    }
  }

  // Resetar flag de visibilidade
  for (auto object : objects)
    object->is_visible = true;
}

bool Scene::checkPlayerCollision(const Vec3f &newPos)
{
  AABB playerBox = player->getBounds();
  Vec3f offset = newPos - player->position;
  playerBox.min = playerBox.min + offset;
  playerBox.max = playerBox.max + offset;

  for (auto obj : objects)
  {
    if (obj->bounds.intersects(playerBox))
      return true;
  }
  return false;
}
