#include <rendering/pipeline.hpp>

/**
 * @brief Obtém a matriz de transformação de SRU para SRC.
 * @note SRU: Sistema de Referência do Universo
 * @note SRC: Sistema de Referência da Câmera
 *
 * A matriz de transformação obtida é dada por:
 *
 * | u.x  u.y  u.z  -u . vrp |
 * | v.x  v.y  v.z  -v . vrp |
 * | n.x  n.y  n.z  -n . vrp |
 * | 0    0    0    1        |
 *
 * @param vrp Vetor de posição da câmera
 * @param focal_point Ponto para onde a câmera está olhando
 * @return Matriz de transformação de SRU para SRC
 */
Matrix pipeline::sru_to_src(const Vec3f &vrp, const Vec3f focal_point)
{
  // Define the n vector.
  Vec3f n = vrp - focal_point;

  Vec3f n_normalized = Vector3Normalize(n);

  // Define the v vector.
  Vec3f up_vec = {0, 1, 0};

  float y1 = Vector3DotProduct(up_vec, n_normalized);

  Vec3f _y1 = n_normalized * y1;

  Vec3f v = up_vec - _y1;

  Vec3f v_normalized = Vector3Normalize(v);

  // Define the u vector.
  Vec3f u = Vector3CrossProduct(v_normalized, n_normalized);

  float m[16] = {u.x, u.y, u.z, -Vector3DotProduct(u, vrp),
                 v_normalized.x, v_normalized.y, v_normalized.z, -Vector3DotProduct(v_normalized, vrp),
                 n_normalized.x, n_normalized.y, n_normalized.z, -Vector3DotProduct(n_normalized, vrp),
                 0, 0, 0, 1};

  Matrix result = Matrix::fromList(m);

  return result;
}

/**
 * @brief Obtém a matriz de projeção.
 *
 * A matriz de projeção obtida é dada por:
 *
 * | 1  0  0  0 |
 * | 0  1  0  0 |
 * | 0  0  -z_vp/dp  z_vp*z_prp/dp |
 * | 0  0  -1/d  z_prp/dp |
 *
 * @param vrp Vetor de posição da câmera
 * @param focal_point Vetor 3D que representa o ponto focal da câmera
 * @param dist_proj_plane Distância do VRP ao plano ponto focal
 * @return Matriz de projeção
 */
Matrix pipeline::projection(const Vec3f &vrp, const Vec3f focal_point, const float dist_proj_plane)
{

  // Definição do plano de projeção
  Vec3f projection_plane = {
      vrp.x + (focal_point.x - vrp.x) * (dist_proj_plane / (vrp.z - focal_point.z)),
      vrp.y + (focal_point.y - vrp.y) * (dist_proj_plane / (vrp.z - focal_point.z)),
      vrp.z + (focal_point.z - vrp.z) * (dist_proj_plane / (vrp.z - focal_point.z))};

  // Distância do VRP até o plano de projeção.
  // Como fizemos a transformação so SRU para SRC, a camera está no centro do universo.
  // Logo a distância é simplesmente o valor.
  float dp = dist_proj_plane;
  // Distância do VRP (View Reference Point, Posição da camera) até o ponto focal.
  float z_vp = -dp;

  // A coordenada Z do ponto onde a linha de projeção se intersecciona no plano de projeção.
  // Neste caso, z_prp é 0. Pois, este ponto coincide com a origem do sistema (0, 0, 0).
  // Por conta dessa simplificação uma vez projetado o ponto, não é possível desfazer a projeção
  float z_prp = 0;

  float m[16] = {1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, (-z_vp) / dp, z_vp * (z_prp) / dp,
                 0, 0, -1 / dist_proj_plane, z_prp / dp};

  Matrix result = Matrix::fromList(m);

  return result;
}

/**
 * @brief Obtém a matriz de transformação de SRC para SRT.
 *
 * A matriz de transformação obtida é dada por:
 *
 * | (u_max - u_min)/(x_max - x_min)  0  0  -x_min *((u_max - u_min)/(x_max - x_min)) + u_min |
 * | 0  (v_min - v_max)/(y_max - y_min)  0  y_min * ((v_max - v_min)/(y_max - y_min)) + v_max |
 * | 0  0  1  0 |
 * | 0  0  0  1 |
 *
 * @param min_window Vetor 2D que representa o canto inferior esquerdo da janela
 * @param min_viewport Vetor 2D que representa o canto inferior esquerdo da viewport
 * @param max_window Vetor 2D que representa o canto superior direito da janela
 * @param max_viewport Vetor 2D que representa o canto superior direito da viewport
 * @param reflected Flag que indica se a transformação é refletida
 * @return Matriz de transformação de SRC para SRT
 */
Matrix pipeline::src_to_srt(const Vec2f min_window, const Vec2f min_viewport, const Vec2f max_window, const Vec2f max_viewport, bool reflected = false)
{
  float u_min = min_viewport.x;
  float u_max = max_viewport.x;
  float v_min = min_viewport.y;
  float v_max = max_viewport.y;

  float x_min = min_window.x;
  float x_max = max_window.x;
  float y_min = min_window.y;
  float y_max = max_window.y;

  Matrix result;

  // Nem todo sistema de telas pode ter o (0,0) começando no canto superior esquerdo
  if (reflected)
  {
    // | (u_max - u_min)/(x_max - x_min), 0, 0, -x_min *((u_max - u_min)/(x_max - x_min)) + u_min |\n
    // | 0, (v_min - v_max)/(y_max - y_min), 0, y_min * ((v_max - v_min)/(y_max - y_min)) + v_max |\n
    // | 0, 0, 1, 0 |\n
    // | 0, 0, 0, 1 |
    float m[16] = {(u_max - u_min) / (x_max - x_min), 0, 0, -x_min * ((u_max - u_min) / (x_max - x_min)) + u_min,
                   0, (v_min - v_max) / (y_max - y_min), 0, y_min * ((v_max - v_min) / (y_max - y_min)) + v_max,
                   0, 0, 1, 0,
                   0, 0, 0, 1};
    result = Matrix::fromList(m);
  }
  else
  {
    // | (u_max - u_min)/(x_max - x_min), 0, 0, -x_min * ((u_max - u_min)/(x_max - x_min)) + u_min |\n
    // | 0, (v_max - v_min)/(y_max - y_min), 0, -y_min * ((v_max - v_min)/(y_max - y_min)) + v_min) |\n
    // | 0, 0, 1, 0 |\n
    // | 0, 0, 0, 1 |
    float m[16] = {(u_max - u_min) / (x_max - x_min), 0, 0, -x_min * ((u_max - u_min) / (x_max - x_min)) + u_min,
                   0, (v_max - v_min) / (y_max - y_min), 0, -y_min * ((v_max - v_min) / (y_max - y_min)) + v_min,
                   0, 0, 1, 0,
                   0, 0, 0, 1};
    result = Matrix::fromList(m);
  }

  return result;
}

/**
 * @brief Verifica se um ponto está antes ou depois de uma borda de uma janela de recorte.
 *
 * @param p Ponto a ser verificado
 * @param min Canto inferior esquerdo da borda de recorte
 * @param max Canto superior direito da borda de recorte
 * @param edge Borda da janela de recorte a ser verificada
 *
 * @return true Se o ponto está dentro da janela de recorte;
 * @return false Se o ponto está fora da janela de recorte
 */
bool pipeline::is_inside(Vec3f p, Vec2f min, Vec2f max, unsigned int edge)
{
  switch (edge)
  {
  case LEFT:
    return p.x >= min.x; // Limite da borda da esquerda da janela de recorte é x_min
  case RIGHT:
    return p.x <= max.x; // Limite da borda da direita da janela de recorte é x_max
  case BOTTOM:
    return p.y >= min.y; // Limite da borda inferior da janela de recorte é y_min
  case TOP:
    return p.y <= max.y; // Limite da borda superior da janela de recorte é y_max
  default:
    return false; // Caso não seja nenhuma das bordas
  }
}

/**
 * @brief Calcula o ponto de interseção de uma linha com uma janela de recorte.
 *
 * @param p1 Ponto inicial da linha
 * @param p2 Ponto final da linha
 * @param min Canto inferior esquerdo da janela de recorte
 * @param max Canto superior direito da janela de recorte
 * @param edge Borda da janela de recorte
 * @return Vec3f
 *
 * @note Utilizado no recorte de linhas 2D
 */
Vec3f pipeline::compute_intersection(Vec3f p1, Vec3f p2, Vec2f min, Vec2f max, unsigned int edge)
{
  float u = 0.0f;
  Vec3f intersection;

  if (edge == LEFT)
  {
    u = (min.x - p1.x) / (p2.x - p1.x);
    intersection.x = min.x;
    intersection.y = Lerp(p1.y, p2.y, u);
    intersection.z = Lerp(p1.z, p2.z, u);
  }
  else if (edge == RIGHT)
  {
    u = (max.x - p1.x) / (p2.x - p1.x);
    intersection.x = max.x;
    intersection.y = Lerp(p1.y, p2.y, u);
    intersection.z = Lerp(p1.z, p2.z, u);
  }
  else if (edge == BOTTOM)
  {
    u = (min.y - p1.y) / (p2.y - p1.y);
    intersection.x = Lerp(p1.x, p2.x, u);
    intersection.y = min.y;
    intersection.z = Lerp(p1.z, p2.z, u);
  }
  else if (edge == TOP)
  {
    u = (max.y - p1.y) / (p2.y - p1.y);
    intersection.x = Lerp(p1.x, p2.x, u);
    intersection.y = max.y;
    intersection.z = Lerp(p1.z, p2.z, u);
  }

  return intersection;
}

/**
 * @brief Calcula o ponto de interseção de uma linha com uma janela de recorte.
 *
 * @param p1 Pontos iniciais da linha (coordenadas e normal/cor)
 * @param p2 Pontos finais da linha (coordenadas e normal/cor)
 * @param min Canto inferior esquerdo da borda de recorte
 * @param max Canto superior direito da borda de recorte
 * @param edge Plano de borda a ser verificada
 * @return std::pair<Vec3f, models::Color> Par de coordenadas e normal/cor
 *
 * @note Utilizado no recorte de polígonos 2D
 */
std::pair<Vec3f, models::Color> pipeline::compute_intersection(std::pair<Vec3f, models::Color> p1, std::pair<Vec3f, models::Color> p2, Vec2f min, Vec2f max, unsigned int edge)
{
  float u = 0.0f;
  std::pair<Vec3f, models::Color> intersection;

  if (edge == LEFT)
  {
    u = (min.x - p1.first.x) / (p2.first.x - p1.first.x);
    intersection.first.x = min.x;
    intersection.first.y = Lerp(p1.first.y, p2.first.y, u);
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == RIGHT)
  {
    u = (max.x - p1.first.x) / (p2.first.x - p1.first.x);
    intersection.first.x = max.x;
    intersection.first.y = Lerp(p1.first.y, p2.first.y, u);
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == BOTTOM)
  {
    u = (min.y - p1.first.y) / (p2.first.y - p1.first.y);
    intersection.first.x = Lerp(p1.first.x, p2.first.x, u);
    intersection.first.y = min.y;
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == TOP)
  {
    u = (max.y - p1.first.y) / (p2.first.y - p1.first.y);
    intersection.first.x = Lerp(p1.first.x, p2.first.x, u);
    intersection.first.y = max.y;
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }

  intersection.second = models::InterpolateColors(p1.second, p2.second, u);

  return intersection;
}

/**
 * @brief Calcula o ponto de interseção de uma linha com uma janela de recorte.
 *
 * @param p1 Pontos iniciais da linha (coordenadas e normal/cor)
 * @param p2 Pontos finais da linha (coordenadas e normal/cor)
 * @param min Canto inferior esquerdo da borda de recorte
 * @param max Canto superior direito da borda de recorte
 * @param edge Plano de borda a ser verificada
 * @return std::pair<Vec3f, Vec3f> Par de coordenadas e normal do vértice
 *
 * @note Utilizado no recorte de polígonos 2D
 */
std::pair<Vec3f, Vec3f> pipeline::compute_intersection(std::pair<Vec3f, Vec3f> p1, std::pair<Vec3f, Vec3f> p2, Vec2f min, Vec2f max, unsigned int edge)
{
  float u = 0.0f;
  std::pair<Vec3f, Vec3f> intersection;

  if (edge == LEFT)
  {
    u = (min.x - p1.first.x) / (p2.first.x - p1.first.x);
    intersection.first.x = min.x;
    intersection.first.y = Lerp(p1.first.y, p2.first.y, u);
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == RIGHT)
  {
    u = (max.x - p1.first.x) / (p2.first.x - p1.first.x);
    intersection.first.x = max.x;
    intersection.first.y = Lerp(p1.first.y, p2.first.y, u);
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == BOTTOM)
  {
    u = (min.y - p1.first.y) / (p2.first.y - p1.first.y);
    intersection.first.x = Lerp(p1.first.x, p2.first.x, u);
    intersection.first.y = min.y;
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }
  else if (edge == TOP)
  {
    u = (max.y - p1.first.y) / (p2.first.y - p1.first.y);
    intersection.first.x = Lerp(p1.first.x, p2.first.x, u);
    intersection.first.y = max.y;
    intersection.first.z = Lerp(p1.first.z, p2.first.z, u);
  }

  intersection.second.x = Lerp(p1.second.x, p2.second.x, u);
  intersection.second.y = Lerp(p1.second.y, p2.second.y, u);
  intersection.second.z = Lerp(p1.second.z, p2.second.z, u);

  return intersection;
}

/**
 * @brief Clipa um polígono 2D
 *
 * @param polygon Lista de vértices do polígono percorridos no sentido anti-horário
 * @param min Limite inferior esquerdo da janela de recorte
 * @param max Limite superior direito da janela de recorte
 * @return std::vector<Vec3f> Lista de vértices do polígono clipado
 *
 * @note O algoritmo de Sutherland-Hodgman é utilizado
 */
std::vector<Vec3f> pipeline::clip_2D_polygon(const std::vector<Vec3f> &polygon, const Vec2f &min, const Vec2f &max)
{
  std::vector<Vec3f> result = polygon;

  // Ordem de recorte
  const std::vector<unsigned int> edges = {LEFT, RIGHT, BOTTOM, TOP};

  // A cada iteração, uma nova lista de vertices é gerada
  for (auto edge : edges)
  {
    if (result.empty())
      break;

    std::vector<Vec3f> input = result;
    result.clear();

    for (int i = 0; i < input.size(); i++)
    {
      int k = (i + 1) % input.size();
      Vec3f p1 = input[i];
      Vec3f p2 = input[k];

      // Testa se os pontos estão dentro da janela de recorte
      bool p1_inside = is_inside(p1, min, max, edge);
      bool p2_inside = is_inside(p2, min, max, edge);

      // Aceitação trivial
      // Ambos os pontos estão dentro da janela, então adiciona o ponto final
      if (p1_inside && p2_inside)
        result.push_back(p2);

      // Rejeição trivial
      // Nenhum dos pontos está dentro da janela, então não adiciona nenhum ponto
      else if (!p1_inside && !p2_inside)
        continue;
      // Não é possível aceitar ou rejeitar trivialmente a linha
      else
      {
        // Calcula o ponto de interseção
        Vec3f intersection = compute_intersection(p1, p2, min, max, edge);

        // Somente o primeiro ponto está fora da janela, então adiciona o ponto de interseção e o ponto final
        if (!p1_inside && p2_inside)
        {
          result.push_back(intersection);
          result.push_back(p2);
        }
        // Somente o segundo ponto está fora da janela, então adiciona o ponto de interseção
        else if (p1_inside && !p2_inside)
          result.push_back(intersection);
      }
    }
  }
  return result;
}

/**
 * @brief Clipa um polígono 2D
 *
 * @param polygon Lista de vértices do polígono percorridos no sentido anti-horário (coordenadas e normal/cor)
 * @param min Limite inferior esquerdo da janela de recorte
 * @param max Limite superior direito da janela de recorte
 * @return std::vector<std::pair<Vec3f, models::Color>> Lista de vértices e normal/cor do polígono clipado
 *
 * @note O algoritmo de Sutherland-Hodgman é utilizado
 * @note Refatorar para utilizar a mesma função de clipagem dos polígonos 2D
 */
std::vector<std::pair<Vec3f, models::Color>> pipeline::clip_2D_polygon(const std::vector<std::pair<Vec3f, models::Color>> &polygon, const Vec2f &min, const Vec2f &max)
{
  std::vector<std::pair<Vec3f, models::Color>> result = polygon;

  const std::vector<unsigned int> edges = {LEFT, RIGHT, BOTTOM, TOP};

  // A cada iteração, uma nova lista de vertices é gerada
  for (auto edge : edges)
  {
    if (result.empty())
      break;

    std::vector<std::pair<Vec3f, models::Color>> input = result;
    result.clear();

    for (int i = 0; i < input.size(); i++)
    {
      int k = (i + 1) % input.size();
      std::pair<Vec3f, models::Color> p1 = input[i];
      std::pair<Vec3f, models::Color> p2 = input[k];

      // Testa se os pontos estão dentro da janela de recorte
      bool p1_inside = is_inside(p1.first, min, max, edge);
      bool p2_inside = is_inside(p2.first, min, max, edge);

      // Ambos os pontos estão dentro da janela, então adiciona o ponto final
      if (p1_inside && p2_inside)
        result.push_back(p2);
      // Nenhum dos pontos está dentro da janela, então não adiciona nenhum ponto
      else if (!p1_inside && !p2_inside)
        continue;
      // Não é possível aceitar ou rejeitar trivialmente a linha
      else
      {
        // Calcula o ponto de interseção
        std::pair<Vec3f, models::Color> intersection = compute_intersection(p1, p2, min, max, edge);

        // Somente o primeiro ponto está fora da janela, então adiciona o ponto de interseção e o ponto final
        if (!p1_inside && p2_inside)
        {
          result.push_back(intersection);
          result.push_back(p2);
        }
        // Somente o segundo ponto está fora da janela, então adiciona o ponto de interseção
        else if (p1_inside && !p2_inside)
          result.push_back(intersection);
      }
    }
  }
  return result;
}

/**
 * @brief Clipa um polígono 2D
 *
 * @param polygon Lista de vértices do polígono percorridos no sentido anti-horário (coordenadas e normal/cor)
 * @param min Limite inferior esquerdo da janela de recorte
 * @param max Limite superior direito da janela de recorte
 * @return std::vector<std::pair<Vec3f, Vec3f>> Lista de vértices e normal/cor do polígono clipado
 *
 * @note O algoritmo de Sutherland-Hodgman é utilizado
 * @note Refatorar para utilizar a mesma função de clipagem dos polígonos 2D
 */
std::vector<std::pair<Vec3f, Vec3f>> pipeline::clip_2D_polygon(const std::vector<std::pair<Vec3f, Vec3f>> &polygon, const Vec2f &min, const Vec2f &max)
{
  std::vector<std::pair<Vec3f, Vec3f>> result = polygon;

  const std::vector<unsigned int> edges = {LEFT, RIGHT, BOTTOM, TOP};

  // A cada iteração, uma nova lista de vertices é gerada
  for (auto edge : edges)
  {
    if (result.empty())
      break;

    std::vector<std::pair<Vec3f, Vec3f>> input = result;
    result.clear();

    for (int i = 0; i < input.size(); i++)
    {
      int k = (i + 1) % input.size();
      std::pair<Vec3f, Vec3f> p1 = input[i];
      std::pair<Vec3f, Vec3f> p2 = input[k];

      // Testa se os pontos estão dentro da janela de recorte
      bool p1_inside = is_inside(p1.first, min, max, edge);
      bool p2_inside = is_inside(p2.first, min, max, edge);

      // Ambos os pontos estão dentro da janela, então adiciona o ponto final
      if (p1_inside && p2_inside)
        result.push_back(p2);
      // Nenhum dos pontos está dentro da janela, então não adiciona nenhum ponto
      else if (!p1_inside && !p2_inside)
        continue;
      // Não é possível aceitar ou rejeitar trivialmente a linha
      else
      {
        // Calcula o ponto de interseção
        std::pair<Vec3f, Vec3f> intersection = compute_intersection(p1, p2, min, max, edge);

        // Somente o primeiro ponto está fora da janela, então adiciona o ponto de interseção e o ponto final
        if (!p1_inside && p2_inside)
        {
          result.push_back(intersection);
          result.push_back(p2);
        }
        // Somente o segundo ponto está fora da janela, então adiciona o ponto de interseção
        else if (p1_inside && !p2_inside)
          result.push_back(intersection);
      }
    }
  }
  return result;
}

/**
 * @brief Desenha um pixel na janela
 *
 * @param pixel pixel da tela com a profundidade associada
 * @param color Cor do pixel
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 *
 * @note Esta função é um wrapper para a função z_buffer
 */
void pipeline::setPixel(const Vec3f pixel, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  pipeline::z_buffer(pixel, color, z_buffer, color_buffer);
}

/**
 * @brief Desenha um vértice no buffer
 *
 * @param point Ponto a ser desenhado
 * @param color Cor do ponto
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 * @param size Tamanho do ponto
 *
 * @todo Ajustar o calculo do z_buffer para esta função
 */
void pipeline::DrawVertexBuffer(const Vec3f point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, const int size)
{
  int x = static_cast<int>(point.x);
  int y = static_cast<int>(point.y);

  for (float i = -2; i < size; i++)
  {
    for (float j = -2; j < size; j++)
    {
      pipeline::setPixel(Vec3f{x + i, y + j, point.z}, color, z_buffer, color_buffer);
    }
  }
}

/**
 * @brief Algoritmo de Bresenham para desenhar uma linha
 *
 * @param start Ponto de início da linha
 * @param end Ponto de fim da linha
 *
 * @todo verificar a interpolação de Z
 *
 * @return Vetor de vértices que compõem a linha
 */
std::vector<Vec3f> pipeline::BresenhamLine(Vec3f start, Vec3f end)
{
  std::vector<Vec3f> line;
  // Se o vértice for válido
  if (start.x != -1 && end.x != -1)
  {
    int x0 = static_cast<int>(start.x);
    int y0 = static_cast<int>(start.y);
    int x1 = static_cast<int>(end.x);
    int y1 = static_cast<int>(end.y);

    float z0 = start.z;
    float z1 = end.z;

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    float dz = (z1 - z0) / std::max(dx, dy); // Step size for Z interpolation

    while (true)
    {

      line.push_back({static_cast<float>(x0), static_cast<float>(y0), z0});

      if (x0 == x1 && y0 == y1)
        break;

      int e2 = 2 * err;
      if (e2 > -dy)
      {
        err -= dy;
        x0 += sx;
        z0 += dz; // Interpolate Z
      }
      if (e2 < dx)
      {
        err += dx;
        y0 += sy;
        z0 += dz; // Interpolate Z
      }
    }
  }

  return line;
}

/**
 * @brief Desenha uma linha no buffer
 *
 * @param vertexes Vetor de vértices que compõem a linha
 * @param color Cor da linha
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 *
 * @note O Algoritmo implementado é o do Bresenham adaptado para interpolação de Z também.
 */
void pipeline::DrawLineBuffer(const std::vector<Vec3f> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  ImDrawList *draw_list = ImGui::GetForegroundDrawList();
  int vertex_length = vertexes.size();
  for (size_t i = 0; i < vertex_length; i++)
  {
    std::vector<Vec3f> line = pipeline::BresenhamLine(vertexes[i], vertexes[(i + 1) % vertexes.size()]);

    for (auto vertex : line)
    {
      pipeline::setPixel(Vec3f{vertex.x, vertex.y, vertex.z}, color, z_buffer, color_buffer);
    }
  }
}

/**
 * @brief Desenha um buffer na janela
 *
 * @param draw_list Referência para a janela onde o buffer será desenhado
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 */
void pipeline::DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<models::Color>> &color_buffer, Vec2f min_window_size)
{
  int width = color_buffer.size();
  int height = color_buffer[0].size();

  // Como o buffer não tem as dimensões da janela, é necessário definir um tamanho mínimo
  int min_x = static_cast<int>(min_window_size.x);
  int min_y = static_cast<int>(min_window_size.y);

  ImGui::SetCursorScreenPos(ImVec2(min_window_size.x, min_window_size.y));

  for (int x = min_x; x < width; x++)
  {
    for (int y = min_y; y < height; y++)
    {
      if (!models::CompareColors(color_buffer[x][y], models::TRANSPARENT))
        draw_list->AddRectFilled({static_cast<float>(x), static_cast<float>(y)}, {static_cast<float>(x + 1), static_cast<float>(y + 1)}, GET_COLOR_UI32(color_buffer[x][y]));
    }
  }
}

/**
 * @brief Atualiza o buffer de profundidade
 *
 * @param z Profundidade do pixel
 * @param color Cor do pixel
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 */
void pipeline::z_buffer(const Vec3f pixel, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  // Arredondamento para o pixel mais próximo
  int x_int = static_cast<int>(pixel.x);
  int y_int = static_cast<int>(pixel.y);

  // Se estiver fora do buffer (por qualquer razão que seja, não faz nada)
  // Em teoria nenhum pixel ficaria fora da janela de visão, mas né...
  if (x_int < 0 || x_int >= z_buffer.size() || y_int < 0 || y_int >= z_buffer[0].size())
    return;

  // Se o pixel atual estiver mais distante que o pixel já desenhado, não atualiza os buffers
  if (z_buffer[x_int][y_int] < pixel.z)
    return;

  // Caso contrário, atualiza o buffer de profundidade e de cor
  z_buffer[x_int][y_int] = pixel.z;
  color_buffer[x_int][y_int] = color;
}

/**
 * @brief Preenche um polígono com sombreamento flat
 *
 * @param vertexes Vertices da face do polígono
 * @param color Cor do polígono
 * @param global_light Luz global
 * @param omni_lights Luzes omni
 * @param eye Posição do observador
 * @param face_centroid Centroide da face
 * @param face_normal Vetor normal da face
 * @param object_material Material do objeto
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 *
 */
void pipeline::fill_polygon_flat(const std::vector<Vec3f> &vertexes, const models::GlobalLight &global_light, const std::vector<models::Omni> &omni_lights, const Vec3f &eye, const Vec3f &face_centroid, const Vec3f &face_normal, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  // Calculamos a cor do objeto
  // Como no nosso pipeline o objeto é homogêneo, não precisamos nos preocupar com variações
  // de materiais de acordo com cada face (Ex.: Objeto metálico com partes de plástico)
  models::Color color = models::FlatShading(global_light, omni_lights, face_centroid, face_normal, eye, object_material);

  // Determina os limites das scanlines
  int y_min = std::numeric_limits<int>::max();
  int y_max = std::numeric_limits<int>::min();

  for (auto vertex : vertexes)
  {
    float y = vertex.y;
    if (y < y_min)
      y_min = static_cast<int>(y);
    if (y > y_max)
      y_max = static_cast<int>(y);
  }

  // Determina o tamanho da area que vamos processar
  std::vector<std::vector<Vec3f>> scanlines(y_max - y_min);

  for (int i = 0; i < vertexes.size(); i++)
  {
    // Quando chegar no final da lista, vai conectar com o 1º vértice.
    int k = (i + 1) % vertexes.size();
    Vec3f start = vertexes[i];
    Vec3f end = vertexes[k];

    // Scanlines horizontais não são processadas
    if (start.y == end.y)
      continue;

    // Ordena as scanlines
    if (start.y > end.y)
    {
      std::swap(start, end);
    }

    // determina as taxes da interpolação
    // Interpolamos X e Z em relação a Y
    // A interpolação é incremental (scanline p/ scanline)
    float m_inv = (end.x - start.x) / (end.y - start.y);
    float mz = (end.z - start.z) / (end.y - start.y);

    float x = start.x;
    float z = start.z;

    for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
    {

      scanlines[y - y_min].push_back({x, static_cast<float>(y), z});

      x += m_inv;
      z += mz;
    }
  }

  for (auto scanline : scanlines)
  {
    // Ordenamos as scanlines para que os valores de X fiquem ordenados
    std::sort(scanline.begin(), scanline.end(), [](Vec3f a, Vec3f b)
              { return a.x < b.x; });

    // Cada scanline possui pares de 2 elementos
    // esse algoritmo pressupõe que as malhas são convexas(fechadas)
    for (int j = 0; j < scanline.size(); j += 2)
    {
      int k = (j + 1) % scanline.size();
      Vec3f start = scanline[j];
      Vec3f end = scanline[k];

      // Mesmo cenário da interpolação anterior (em relação a Y)
      // Porém agora a interpolação é em relação a X
      // E só interpolamos Z
      float mz = (end.z - start.z) / (end.x - start.x);
      float z = start.z + (start.x - ceilf(start.x)) * mz;

      // Esse arredondamento é para evitar descontinuidades
      // durante a interpolação em Y, alguns pixels podem ficar com valor
      // quebrado Ex.: 100.80 pixels, aí entra esse arrendondamento
      // desenhamos um pixel a mais no inicio e um pixel a menos no final
      // Desse modo a face fica continua.
      for (float x = ceilf(start.x); x <= floorf(end.x); x++)
      {
        // nesse caso só interpolamos em Z, pois já sabemos a scanline em X e Y
        pipeline::z_buffer(Vec3f{x, start.y, z}, color, z_buffer, color_buffer);
        z += mz;
      }
    }
  }
}

/**
 * @brief Preenche um polígono com sombreamento de Gourand
 *
 * @param vertexes Vertices da face do polígono
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 *
 */
void pipeline::fill_polygon_gourand(const std::vector<std::pair<Vec3f, models::Color>> &_vertexes, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  // Usando para associar cada vértice com sua cor calculada
  std::vector<std::pair<Vec3f, models::Color>> vertexes = _vertexes;

  int y_min = std::numeric_limits<int>::max();
  int y_max = std::numeric_limits<int>::min();

  for (auto vertex : vertexes)
  {
    float y = vertex.first.y;
    if (y < y_min)
      y_min = static_cast<int>(y);
    if (y > y_max)
      y_max = static_cast<int>(y);
  }

  // Vetor de scanlines
  // 1º parâmetro do par: vetor de coordenadas SRT (coordenadas de tela)
  // 2º parâmetro do par: cor do pixel
  std::vector<std::vector<std::pair<Vec3f, models::Color>>> scanlines(y_max - y_min);

  // Como a lista de vertices é composta pelo inicio e fim da aresta, o incremento é de 2
  for (int i = 0; i < vertexes.size(); i++)
  {
    int k = (i + 1) % vertexes.size();

    Vec3f start = vertexes[i].first;
    Vec3f end = vertexes[k].first;

    models::Color start_color = vertexes[i].second;
    models::Color end_color = vertexes[k].second;

    // Se a linha for horizontal, não faz nada
    if (start.y == end.y)
      continue;

    if (start.y > end.y)
    {
      std::swap(start, end);
      std::swap(start_color, end_color);
    }

    float dy = end.y - start.y;

    float m_inv = (end.x - start.x) / dy;
    float dz = (end.z - start.z) / dy;

    float dr = (end_color.r - start_color.r) / dy;
    float dg = (end_color.g - start_color.g) / dy;
    float db = (end_color.b - start_color.b) / dy;

    float x = start.x;
    float z = start.z;

    float r = start_color.r;
    float g = start_color.g;
    float b = start_color.b;

    for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
    {

      scanlines[y - y_min].push_back(std::make_pair<Vec3f, models::Color>({x, static_cast<float>(y), z}, {models::ChannelsToColor({r, g, b})}));
      x += m_inv;
      z += dz;

      r += dr;
      g += dg;
      b += db;
    }
  }

  for (int i = 0; i < scanlines.size(); i++)
  {
    std::sort(scanlines[i].begin(), scanlines[i].end(),
              [](std::pair<Vec3f, models::Color> a, std::pair<Vec3f, models::Color> b)
              { return a.first.x < b.first.x; });

    for (int j = 0; j < scanlines[i].size(); j = j + 2)
    {

      Vec3f start = scanlines[i][j].first;
      Vec3f end = scanlines[i][j + 1].first;

      models::Color start_color = scanlines[i][j].second;
      models::Color end_color = scanlines[i][j + 1].second;

      float dx = end.x - start.x;

      float dz = (end.z - start.z) / dx;
      float z = start.z + (start.x - ceilf(start.x)) * dz;

      float dr = (end_color.r - start_color.r) / dx;
      float dg = (end_color.g - start_color.g) / dx;
      float db = (end_color.b - start_color.b) / dx;

      float r = start_color.r + (start.x - ceilf(start.x)) * dr;
      float g = start_color.g + (start.x - ceilf(start.x)) * dg;
      float b = start_color.b + (start.x - ceilf(start.x)) * db;

      for (float x = ceilf(start.x); x <= floorf(end.x); x++)
      {
        models::Color current_color = models::ChannelsToColor({r, g, b});

        pipeline::z_buffer(Vec3f{x, start.y, z}, current_color, z_buffer, color_buffer);
        z += dz;
        r += dr;
        g += dg;
        b += db;
      }
    }
  }
}

/**
 * @brief Preenche um polígono com sombreamento de Phong
 *
 * @param _vertexes Lista de vertices e normais dos vertices
 * @param global_light Luz ambiente global
 * @param omni_lights Lista de luzes omnidirecionais
 * @param eye Posição do observador
 * @param object_material Material do objeto
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 */
void pipeline::fill_polygon_phong(const std::vector<std::pair<Vec3f, Vec3f>> &_vertexes, const Vec3f &centroid, const models::GlobalLight &global_light, const std::vector<models::Omni> &omni_lights, const Vec3f &eye, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer)
{
  int y_min = std::numeric_limits<int>::max();
  int y_max = std::numeric_limits<int>::min();

  for (auto vertex : _vertexes)
  {
    float y = vertex.first.y;
    if (y < y_min)
      y_min = static_cast<int>(y);
    if (y > y_max)
      y_max = static_cast<int>(y);
  }

  // Vetor de scanlines
  // 1º parâmetro do par: vetor de coordenadas SRT (coordenadas de tela)
  // 2º parâmetro do par: vetor normal do pixel (interpolado)
  std::vector<std::vector<std::tuple<Vec3f, Vec3f>>> scanlines(y_max - y_min);

  for (int l = 0; l < _vertexes.size(); l++)
  {
    int m = (l + 1) % _vertexes.size();
    Vec3f start = _vertexes[l].first;
    Vec3f end = _vertexes[m].first;

    Vec3f start_normal = _vertexes[l].second;
    Vec3f end_normal = _vertexes[m].second;

    // Se a linha for horizontal, não faz nada
    if (start.y == end.y)
      continue;

    if (start.y > end.y)
    {
      std::swap(start, end);
      std::swap(start_normal, end_normal);
    }

    float dy = end.y - start.y;
    float dx = end.x - start.x;
    float dz = end.z - start.z;
    // i,j,k se referem ao vetor normal do pixel (x, y, z)
    float dn_i = end_normal.x - start_normal.x;
    float dn_j = end_normal.y - start_normal.y;
    float dn_k = end_normal.z - start_normal.z;

    // taxa de variação do x em relação ao y
    float t_x = dx / dy;
    // taxa de variação do z em relação ao y
    float t_z = dz / dy;
    // taxa de variação do i em relação ao y
    float t_i = dn_i / dy;
    // taxa de variação do j em relação ao y
    float t_j = dn_j / dy;
    // taxa de variação do k em relação ao y
    float t_k = dn_k / dy;

    float x = start.x;
    float z = start.z;

    float i = start_normal.x;
    float j = start_normal.y;
    float k = start_normal.z;

    for (int y = static_cast<int>(start.y); y < static_cast<int>(end.y); y++)
    {
      scanlines[y - y_min].push_back(std::make_tuple<Vec3f, Vec3f>({x, static_cast<float>(y), z}, {i, j, k}));
      // incrementa com as taxas de variação (interpolação linear)
      x += t_x;
      z += t_z;
      i += t_i;
      j += t_j;
      k += t_k;
    }
  }

  for (int row = 0; row < scanlines.size(); row++)
  {
    std::sort(scanlines[row].begin(), scanlines[row].end(),
              [](std::tuple<Vec3f, Vec3f> a, std::tuple<Vec3f, Vec3f> b)
              { return std::get<0>(a).x < std::get<0>(b).x; });

    for (int col = 0; col < scanlines[row].size(); col += 2)
    {
      Vec3f start = std::get<0>(scanlines[row][col]);
      Vec3f end = std::get<0>(scanlines[row][col + 1]);

      Vec3f start_normal = std::get<1>(scanlines[row][col]);
      Vec3f end_normal = std::get<1>(scanlines[row][col + 1]);

      float dx = end.x - start.x;
      float dz = (end.z - start.z) / dx;
      float z = start.z + (start.x - ceilf(start.x)) * dz;

      float dn_i = (end_normal.x - start_normal.x) / dx;
      float dn_j = (end_normal.y - start_normal.y) / dx;
      float dn_k = (end_normal.z - start_normal.z) / dx;

      float i = start_normal.x;
      float j = start_normal.y;
      float k = start_normal.z;

      for (float x = ceilf(start.x); x <= floorf(end.x); x++)
      {
        Vec3f v = {x, start.y, z};
        Vec3f n = {i, j, k};

        models::Color color = models::PhongShading(global_light, omni_lights, centroid, v, n, eye, object_material);
        pipeline::z_buffer(Vec3f{x, start.y, z}, color, z_buffer, color_buffer);
        z += dz;
        i += dn_i;
        j += dn_j;
        k += dn_k;
      }
    }
  }
}

/**
 * @brief Preenche um polígono com sombreamento baseado em textura (UV)
 *
 * @param vertexes Vertices da face do polígono
 * @param tex Textura do objeto
 * @param global_light Luz global (mantido caso queira aplicar iluminação multiplicativa)
 * @param omni_lights Luzes omni
 * @param eye Posição do observador
 * @param face_centroid Centroide da face
 * @param face_normal Vetor normal da face
 * @param object_material Material do objeto
 * @param z_buffer Buffer de profundidade
 * @param color_buffer Buffer de cores
 */
void pipeline::fill_polygon_texture(const std::vector<Vertex *> &vertexes, const models::Texture &tex,
                                    const models::GlobalLight &global_light,
                                    const std::vector<models::Omni> &omni_lights,
                                    const Vec3f &eye, const Vec3f &face_centroid, const Vec3f &face_normal,
                                    const models::Material &object_material,
                                    std::vector<std::vector<float>> &z_buffer,
                                    std::vector<std::vector<models::Color>> &color_buffer)
{
  // Determina os limites da scanline
  int y_min = std::numeric_limits<int>::max();
  int y_max = std::numeric_limits<int>::min();

  for (auto v : vertexes)
  {
    float y = v->vertex_screen.y;
    if (y < y_min)
      y_min = static_cast<int>(y);
    if (y > y_max)
      y_max = static_cast<int>(y);
  }

  std::vector<std::vector<Vertex *>> scanlines(y_max - y_min);

  // Construção das scanlines
  for (int i = 0; i < vertexes.size(); i++)
  {
    int k = (i + 1) % vertexes.size();
    Vertex *start = vertexes[i];
    Vertex *end = vertexes[k];

    if (start->vertex_screen.y == end->vertex_screen.y)
      continue;

    Vertex *v1 = start;
    Vertex *v2 = end;
    if (v1->vertex_screen.y > v2->vertex_screen.y)
      std::swap(v1, v2);

    float dy = v2->vertex_screen.y - v1->vertex_screen.y;
    float dx = (v2->vertex_screen.x - v1->vertex_screen.x) / dy;
    float dz = (v2->vertex_screen.z - v1->vertex_screen.z) / dy;
    float du = (v2->u - v1->u) / dy;
    float dv = (v2->v - v1->v) / dy;

    float x = v1->vertex_screen.x;
    float z = v1->vertex_screen.z;
    float u = v1->u;
    float v = v1->v;

    for (int y = static_cast<int>(v1->vertex_screen.y); y < static_cast<int>(v2->vertex_screen.y); y++)
    {
      Vertex *interp = new Vertex();
      interp->vertex_screen = {x, static_cast<float>(y), z};
      interp->u = u;
      interp->v = v;

      scanlines[y - y_min].push_back(interp);

      x += dx;
      z += dz;
      u += du;
      v += dv;
    }
  }

  // Preenchimento das scanlines
  for (auto scanline : scanlines)
  {
    std::sort(scanline.begin(), scanline.end(), [](Vertex *a, Vertex *b)
              { return a->vertex_screen.x < b->vertex_screen.x; });

    for (int j = 0; j < scanline.size(); j += 2)
    {
      int k = (j + 1) % scanline.size();
      Vertex *start = scanline[j];
      Vertex *end = scanline[k];

      float dx = (end->vertex_screen.x - start->vertex_screen.x);
      if (dx == 0)
        dx = 1.0f; // evita divisão por zero

      float dz = (end->vertex_screen.z - start->vertex_screen.z) / dx;
      float du = (end->u - start->u) / dx;
      float dv = (end->v - start->v) / dx;

      float z = start->vertex_screen.z;
      float u = start->u;
      float v = start->v;

      for (float x = ceilf(start->vertex_screen.x); x <= floorf(end->vertex_screen.x); x++)
      {
        // Calcula os índices na textura
        int tex_u = std::min(std::max(int(u * (tex.width - 1)), 0), tex.width - 1);
        int tex_v = std::min(std::max(int(v * (tex.height - 1)), 0), tex.height - 1);

        models::Color color = tex.pixels[tex_v][tex_u];

        // Aplica z-buffer
        pipeline::z_buffer(Vec3f{x, start->vertex_screen.y, z}, color, z_buffer, color_buffer);

        z += dz;
        u += du;
        v += dv;
      }
    }
  }

  // Limpar vértices interpolados criados
  for (auto &line : scanlines)
  {
    for (auto v : line)
      delete v;
  }
}
