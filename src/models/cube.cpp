#include <models/mesh.hpp>

/**
 * @brief Retorna a malha de um cubo
 *
 * @note O cubo tem tamanho unitário(-1 à 1) e está centrado na origem
 *
 * @return Mesh* Ponteiro para a malha do cubo
 */
Mesh *cube(Vec3f shift)
{
  // Vértices com deslocamento aplicado corretamente
  Vertex *v0 = new Vertex(-1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v0");
  Vertex *v1 = new Vertex(1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v1");
  Vertex *v2 = new Vertex(1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v2");
  Vertex *v3 = new Vertex(-1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v3");
  Vertex *v4 = new Vertex(-1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v4");
  Vertex *v5 = new Vertex(1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v5");
  Vertex *v6 = new Vertex(1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v6");
  Vertex *v7 = new Vertex(-1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v7");

  std::vector<std::vector<int>> edges = {
      // Back Face
      {2, 6, 7},
      {7, 3, 2},
      // Left Face
      {0, 4, 5},
      {5, 1, 0},
      // Bottom Face
      {3, 7, 4},
      {4, 0, 3},
      // Right Face
      {2, 3, 0},
      {0, 1, 2},
      // Top Face
      {6, 2, 1},
      {1, 5, 6},
      // Front Face
      {6, 5, 4},
      {4, 7, 6}};

  Mesh *cube = new Mesh({v0, v1, v2, v3, v4, v5, v6, v7}, edges, "cube");

  return cube;
}