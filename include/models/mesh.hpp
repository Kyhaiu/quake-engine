#pragma once

#include <core/halfedge.hpp>
#include <models/colision.hpp>
#include <models/common.hpp>

#include <models/texture.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <map>

class Mesh
{
public:
  // Vetor que contém todos os vertices da malha
  std::vector<Vertex *> vertexes;

  // Vetor que contém todas as faces da malha
  std::vector<Face *> faces;

  // Vetor que contém todas as meias arestas da malha
  std::vector<HalfEdge *> halfedges;

  // Numero de faces
  int num_faces;

  // Mapa de vértices (Usado apenas na criação da malha)
  std::map<std::string, HalfEdge *> halfedges_map;

  // Flag para indicar se o objeto está entre o plano near e far
  bool is_visible;

  // Material do objeto
  models::Material material;

  // Textura associada a malha
  models::Texture texture;

  // Bounding box do modelo
  AABB bounds;

  // Id
  std::string id;

  // Construtor e Destrutor
  Mesh();
  Mesh(std::vector<Vertex *> vertexes, std::vector<std::vector<int>> faces, std::string id);
  ~Mesh();

  // Obtém o centroid do objeto
  Vec3f getCentroid();

  // Obtém o box envolvente da malha
  std::vector<Vec3f> getBox3D(bool screen_coordinates);

  // Função usada para criar a Mesh
  // Ela recebe a lista de vertices e como as faces estão ligadas
  void createMesh(const std::vector<Vertex *> vertexes, std::vector<std::vector<int>> index_faces);

  HalfEdge *addEdge(Vertex *vertex1, Vertex *vertex2);
  Face *addFaceByHalfEdges(std::vector<HalfEdge *> half_edges);
  Face *addFaceByVertices(std::vector<Vertex *> vertexes);
  HalfEdge *findEdge(Vertex *vertex1, Vertex *vertex2);

  // Colisão
  // Sempre que você criar um objeto ou mover, você deve re-calcular a bound box dele
  void computeBounds();

  // Determina o vetor unitário médio da face
  void determineVertexNormals();
};