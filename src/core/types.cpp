#include <core/halfedge.hpp>

/**
 * @brief Construtor padrão da classe HalfEdge
 */
HalfEdge::HalfEdge()
{
  this->next = nullptr;
  this->prev = nullptr;
  this->twin = nullptr;
  this->origin = nullptr;
  this->incident_face = nullptr;
}

/**
 * @brief Construtor da classe Face
 *
 * Inicializa os ponteiros da classe com nullptr, as flags com false e o id com uma string vazia.
 *
 */
Face::Face()
{
  this->vertexes = {};
  this->clipped_vertexes = {};
  this->he = nullptr;
  this->visible = false;
  this->normal = Vec3f(0.0f, 0.0f, 0.0f);
}

/**
 * @brief Verifica se a face é visível
 *
 * @param player_position Posição do player (coordenadas do mundo)
 *
 * @return bool
 *
 * @note A face é visível se o produto escalar entre o vetor normal da face e o vetor da face para o player for maior que 0
 */
bool Face::is_visible(Vec3f player_position)
{
  determine_face_normal();

  determine_face_centroid();

  Vec3f face2player = Vector3Normalize(player_position - this->centroid);

  return Vector3DotProduct(normal, face2player) > 0;
}

/**
 * @brief Calcula o vetor normal da face
 *
 * @return void - O vetor normal é armazenado no atributo normal da classe
 */
void Face::determine_face_normal()
{
  Vec3f p1, p2, p3;

  HalfEdge *he = this->he;

  // Isso garante que o percurso aconteça no sentido anti-horário
  p1 = this->he->prev->origin->vertex.to_vec3();
  p2 = this->he->origin->vertex.to_vec3();
  p3 = this->he->next->origin->vertex.to_vec3();

  Vec3f a = {p1.x - p2.x, p1.y - p2.y, p1.z - p2.z};
  Vec3f b = {p3.x - p2.x, p3.y - p2.y, p3.z - p2.z};

  // Vetor normal da face = B x A
  this->normal = Vector3Normalize(Vector3CrossProduct(b, a));
}

/**
 * @brief Obtém uma aproximação do centroide da face
 *
 * @param bool screen_coords Flag para indicar se o cálculo deve ser feito em coordenadas de tela
 *
 * @note `screen_coords` por padrão é falso
 * @note A aproximação é pela média dos vértices da face
 *
 * @return Vec3f Vetor com a posição do centroide da face
 */
void Face::determine_face_centroid(bool screen_coords)
{
  this->centroid = Vec3f();

  for (auto v : this->vertexes)
  {
    if (!screen_coords)
    {
      this->centroid.x += v->vertex.x;
      this->centroid.y += v->vertex.y;
      this->centroid.z += v->vertex.z;
    }
    else
    {
      this->centroid.x += v->vertex_screen.x;
      this->centroid.y += v->vertex_screen.y;
      this->centroid.z += v->vertex_screen.z;
    }
  }

  int size = this->vertexes.size();

  this->centroid.x /= size;
  this->centroid.y /= size;
  this->centroid.z /= size;
}

/**
 * @brief Construtor da classe Vertex
 *
 * Inicializa o vetor da classe com o vetor 4D nulo.
 *
 */
Vertex::Vertex()
{
  this->vertex = Vec4f();
  this->vertex_screen = Vec3f();
  this->clipped = false;
  this->incident_edge = nullptr;
}

/**
 * @brief Construtor da classe Vertex
 *
 * @param x Coordenada x do vetor
 * @param y Coordenada y do vetor
 * @param z Coordenada z do vetor
 * @param w Coordenada w do vetor
 * @param half_edge Ponteiro para a meia aresta que tem o vértice como origem
 * @param id Identificador do vértice
 */
Vertex::Vertex(float x, float y, float z, float w, HalfEdge *half_edge, std::string id)
{
  vertex = {x, y, z, w};
  // 1.17549e-38 is the smallest positive float value
  vertex_screen = {1.17549e-38f, 1.17549e-38f, 1.17549e-38f};
  this->id = id;
  incident_edge = half_edge;
  normal = Vec3f();
}