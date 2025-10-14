#include <models/mesh.hpp>

/**
 * @brief Cria uma malha representando um chão plano (plano XZ)
 *
 * @param size Metade do tamanho do chão (ex: size = 50 cria um chão 100x100)
 * @param y Altura (posição Y do chão)
 * @return Mesh* Ponteiro para a malha do chão
 */
Mesh *ground(float size = 50.0f, float y = 0.0f)
{
  // Vértices do chão (plano XZ)
  Vertex *v0 = new Vertex(-size, y, -size, 1.0f, nullptr, "v0");
  Vertex *v1 = new Vertex(size, y, -size, 1.0f, nullptr, "v1");
  Vertex *v2 = new Vertex(size, y, size, 1.0f, nullptr, "v2");
  Vertex *v3 = new Vertex(-size, y, size, 1.0f, nullptr, "v3");

  // Duas faces triangulares
  std::vector<std::vector<int>> faces = {
      {0, 1, 2},
      {0, 2, 3}};

  // Cria a malha
  Mesh *plane = new Mesh({v0, v1, v2, v3}, faces, "ground");

  return plane;
}
