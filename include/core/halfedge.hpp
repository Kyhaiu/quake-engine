#pragma once

#include <core/halfedge.hpp>

#include <math/math.hpp>

class Vertex;
class Face;

class HalfEdge
{
public:
  // Ponteiro para a próxima meia aresta
  HalfEdge *next;

  // Ponteiro para a meia aresta anterior
  HalfEdge *prev;

  // Ponteiro para a meia aresta gêmea
  HalfEdge *twin;

  // Ponteiro para o vértice de origem da meia aresta
  Vertex *origin;

  // Ponteiro para a face que contém a meia aresta
  Face *incident_face;

  // Identificador univoco
  std::string id;

  // Constructors and destructors
  HalfEdge();
  ~HalfEdge();
};

class Face
{
public:
  // Vetor para os vértices da face
  std::vector<Vertex *> vertexes;

  // Ponteiro para uma meia aresta da face
  HalfEdge *he;

  // Flag para indicar se a face é visível
  bool visible;

  // Vetor normal da face (usado na ocultação de faces e calculo de iluminação)
  Vec3f normal;

  // Ponto central da face
  Vec3f centroid;

  // Identificador univoco
  std::string id;

  /**
   * @brief Vetor que armazena os vértices da face após o clipping
   *
   * @note O clipping é feito em coordenadas de tela
   * @note A ordem no sentido anti-horário é preservada neste vetor
   */
  std::vector<Vec3f> clipped_vertexes;

  // Constructors and destructors
  Face();
  ~Face();

  bool is_visible(Vec3f player_position);

  void determine_face_normal();
  void determine_face_centroid(bool screen_coords = false);
};

class Vertex
{
public:
  // Vértice 3D + Coordenada homogênea
  Vec4f vertex;

  // Posição do vértice convertido para o sistema de coordenadas de tela
  Vec3f vertex_screen;

  // flag que indica se o vértice já foi recortado
  bool clipped;

  // Meia aresta que incide sob o vértice
  HalfEdge *incident_edge;

  // Identificador univoco
  std::string id;

  /**
   * @brief Vetor normal médio do vértice
   *
   * @note Utilizado para o calculo de iluminação do Gouraud e Phong
   *
   * @note Este vetor é calculado a partir das normais das faces que compartilham o vértice
   * @note O vetor já está em sua forma normalizada
   */
  Vec3f normal;

  // Constructors and destructors
  Vertex();
  Vertex(float x, float y, float z, float w, HalfEdge *half_edge, std::string id);
  ~Vertex();
};
