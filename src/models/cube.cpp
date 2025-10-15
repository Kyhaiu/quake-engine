#include <models/mesh.hpp>
#include <models/texture.hpp>

#include <utils/bmp_reader.hpp>

/**
 * @brief Retorna a malha de um cubo
 *
 * @note O cubo tem tamanho unitário(-1 à 1) e está centrado na origem
 *
 * @return Mesh* Ponteiro para a malha do cubo
 */
Mesh *cube(Vec3f shift, std::string filename)
{
  // Vértices do cubo com UVs
  Vertex *v0 = new Vertex(-1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v0", 0.0f, 0.0f);
  Vertex *v1 = new Vertex(1.0f + shift.x, -1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v1", 1.0f, 0.0f);
  Vertex *v2 = new Vertex(1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v2", 1.0f, 1.0f);
  Vertex *v3 = new Vertex(-1.0f + shift.x, -1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v3", 0.0f, 1.0f);
  Vertex *v4 = new Vertex(-1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v4", 0.0f, 0.0f);
  Vertex *v5 = new Vertex(1.0f + shift.x, 1.0f + shift.y, -1.0f + shift.z, 1.0f, nullptr, "v5", 1.0f, 0.0f);
  Vertex *v6 = new Vertex(1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v6", 1.0f, 1.0f);
  Vertex *v7 = new Vertex(-1.0f + shift.x, 1.0f + shift.y, 1.0f + shift.z, 1.0f, nullptr, "v7", 0.0f, 1.0f);

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

  // Carrega a textura BMP
  models::Texture tex;
  if (!models::loadTexture(filename, tex))
  {
  }
  else
  {
    cube->texture = tex;
  }

  // Marca que todos os vértices têm UVs
  for (auto v : cube->vertexes)
    v->has_uv = true;

  return cube;
}