#include <models/mesh.hpp>

Mesh::Mesh()
{
  vertexes = std::vector<Vertex *>();
  faces = std::vector<Face *>();
  halfedges = std::vector<HalfEdge *>();
  num_faces = 0;

  material.ambient = {0.5f, 0.0f, 0.0f};
  material.diffuse = {0.7f, 0.5f, 0.0f};
  material.specular = {0.9f, 0.5f, 0.0f};
  material.shininess = 32.0f;
}

/**
 * @brief Construtor da classe Mesh
 *
 * @param vertexes Vetor de ponteiros para objetos da classe Vertex
 * @param faces Vetor de vetores de inteiros que representam as faces da malha
 * @param id Identificador da malha
 */
Mesh::Mesh(std::vector<Vertex *> vertexes, std::vector<std::vector<int>> faces, std::string id)
{
  this->vertexes = vertexes;
  this->num_faces = faces.size();
  this->id = id;
  this->createMesh(vertexes, faces);

  this->material.ambient = {0.5f, 0.0f, 0.0f};
  this->material.diffuse = {0.7f, 0.5f, 0.0f};
  this->material.specular = {0.9f, 0.5f, 0.0f};
  this->material.shininess = 32.0f;
}

/**
 * @brief Destrutor padrão da classe Mesh
 */
Mesh::~Mesh()
{
}

/**
 * @brief Método que retorna o centroide da malha
 *
 * @note O centroide é calculado pela soma dos pontos extremos da malha dividido por 2
 * @return Vec3f Centroide da malha
 */
Vec3f Mesh::getCentroid()
{
  std::vector<Vec3f> box = this->getBox3D(false);

  Vec3f min = box[0];
  Vec3f max = box[1];

  Vec3f centroid = {0.0f, 0.0f, 0.0f};

  centroid.x = (min.x + max.x) / 2;
  centroid.y = (min.y + max.y) / 2;
  centroid.z = (min.z + max.z) / 2;

  return centroid;
}

/**
 * @brief Método que retorna o box envolvente da malha
 *
 * @param screen_coordinates Flag que indica se as coordenadas são em relação à tela
 *
 * @note O vector contém 2 pontos, o primeiro é o ponto mínimo e o segundo é o ponto máximo
 * @note vector[0] = min_x, min_y, min_z
 * @note vector[1] = max_x, max_y, max_z
 * @return std::vector<Vec3f> Box envolvente da malha
 */
std::vector<Vec3f> Mesh::getBox3D(bool screen_coordinates)
{
  float min_x = std::numeric_limits<float>::max();
  float min_y = std::numeric_limits<float>::max();
  float min_z = std::numeric_limits<float>::max();
  float max_x = std::numeric_limits<float>::min();
  float max_y = std::numeric_limits<float>::min();
  float max_z = std::numeric_limits<float>::min();

  float x, y, z;

  for (auto v : vertexes)
  {
    x = screen_coordinates ? v->vertex_screen.x : v->vertex.x;
    y = screen_coordinates ? v->vertex_screen.y : v->vertex.y;
    z = screen_coordinates ? v->vertex_screen.z : v->vertex.z;

    if (x < min_x)
    {
      min_x = x;
    }
    if (y < min_y)
    {
      min_y = y;
    }
    if (z < min_z)
    {
      min_z = z;
    }
    if (x > max_x)
    {
      max_x = x;
    }
    if (y > max_y)
    {
      max_y = y;
    }
    if (z > max_z)
    {
      max_z = z;
    }
  }

  std::vector<Vec3f> result = {Vec3f{min_x, min_y, min_z}, Vec3f{max_x, max_y, max_z}};

  return result;
}

void Mesh::createMesh(const std::vector<Vertex *> _vertexes, std::vector<std::vector<int>> index_faces)
{
  vertexes = _vertexes;

  if (vertexes.size() == 0)
  {
    std::cout << "No vertices to create mesh" << std::endl;
    return;
  }
  else if (vertexes.size() < 3)
  {
    std::cout << "Not enough vertices to create mesh. At least 3 vertices are needed" << std::endl;
    return;
  }

  if (num_faces == 0)
  {
    std::cout << "No faces to create mesh" << std::endl;
    return;
  }

  std::vector<Vertex *> vertices = vertexes;

  // Cria as faces da malha
  for (auto face : index_faces)
  {
    std::vector<Vertex *> face_vertices;
    for (auto index : face)
    {
      face_vertices.push_back(vertices[index]);
    }

    Face *f = addFaceByVertices(face_vertices);

    f->vertexes = face_vertices;
  }

  for (auto edge : halfedges)
  {
    if (edge->twin == nullptr)
    {
      Vertex *v1 = edge->origin;
      Vertex *v2 = edge->next->origin;

      HalfEdge *twin = findEdge(v2, v1);
      if (twin != nullptr)
      {
        edge->twin = twin;
        twin->twin = edge;
      }
    }
  }

  for (auto edge : halfedges)
  {
    if (edge->incident_face == nullptr)
    {
      HalfEdge *next = edge->twin;

      do
      {
        next = next->prev->twin;
      } while (next->incident_face != nullptr);

      edge->next = next;
      next->prev = edge;
    }
  }

  // material.ambient = {0.5f, 0.0f, 0.0f};
  // material.diffuse = {0.7f, 0.5f, 0.0f};
  // material.specular = {0.9f, 0.5f, 0.0f};
  // material.shininess = 32.0f;

  num_faces = faces.size();

  // pré computa a bounding box
  computeBounds();
}

HalfEdge *Mesh::addEdge(Vertex *vertex1, Vertex *vertex2)
{
  HalfEdge *he = new HalfEdge();
  he->id = "e" + std::to_string(halfedges.size());

  std::string id = "e" + vertex1->id + "-" + vertex2->id;
  halfedges_map.insert(std::pair<std::string, HalfEdge *>(id, he));
  halfedges.push_back(he);

  he->origin = vertex1;
  if (vertex1->incident_edge == nullptr)
    vertex1->incident_edge = he;

  // Associação da aresta com o seu par, se ele existir
  HalfEdge *twin_he = findEdge(vertex2, vertex1);
  if (twin_he != nullptr)
  {
    he->twin = twin_he;
    twin_he->twin = he;
  }

  return he;
}

/**
 * @brief Método que adiciona uma face à malha a partir de um vetor de arestas
 *
 * @param halfedges Vetor de ponteiros para objetos da classe HalfEdge
 * @return Face* Ponteiro para a face criada
 */
Face *Mesh::addFaceByHalfEdges(std::vector<HalfEdge *> halfedges)
{
  Face *face = new Face();
  face->id = "f" + std::to_string(faces.size());
  // Adiciona a face a malha
  faces.push_back(face);

  // Associa as meias arestas a face
  for (int i = 0; i < halfedges.size(); i++)
  {
    halfedges[i]->incident_face = face;
  }

  // Associa a primeira meia aresta a face
  face->he = halfedges[0];

  // Conecta as meias arestas ao redor da face
  int len = halfedges.size();
  for (int i = 0; i < halfedges.size(); i++)
  {
    halfedges[i]->next = halfedges[(i + 1) % len];
    halfedges[i]->prev = halfedges[(i - 1 + len) % len];
  }

  return face;
}

Face *Mesh::addFaceByVertices(std::vector<Vertex *> vertices)
{
  std::vector<HalfEdge *> halfedges;
  int len = vertices.size();
  for (int i = 0; i < len; i++)
  {
    Vertex *v1 = vertices[i];
    Vertex *v2 = vertices[(i + 1) % len];

    HalfEdge *he = findEdge(v1, v2);
    // Se a aresta não existir, cria uma nova
    if (he == nullptr)
      he = addEdge(v1, v2);

    halfedges.push_back(he);
  }

  return addFaceByHalfEdges(halfedges);
}

/**
 * @brief Método que busca uma aresta no mapa de arestas da malha
 *
 * @param vertex1 Vértice 1
 * @param vertex2 Vértice 2
 * @return HalfEdge* Ponteiro para a aresta encontrada
 *
 * @note Se a aresta não for encontrada, retorna nullptr
 */
HalfEdge *Mesh::findEdge(Vertex *vertex1, Vertex *vertex2)
{
  std::string key = "e" + vertex1->id + "-" + vertex2->id;
  if (halfedges_map.find(key) != halfedges_map.end())
    return halfedges_map[key];

  return nullptr;
}

/**
 * @brief Calcula as extremidades da bounding box (caixa de colisão)
 *
 * @note toda que o objeto for criado ou movido, é necessário re-computar
 */
void Mesh::computeBounds()
{
  if (vertexes.empty())
    return;
  Vec3f minV = vertexes[0]->vertex.to_vec3();
  Vec3f maxV = vertexes[0]->vertex.to_vec3();

  for (auto v : vertexes)
  {
    minV.x = std::min(minV.x, v->vertex.x);
    minV.y = std::min(minV.y, v->vertex.y);
    minV.z = std::min(minV.z, v->vertex.z);

    maxV.x = std::max(maxV.x, v->vertex.x);
    maxV.y = std::max(maxV.y, v->vertex.y);
    maxV.z = std::max(maxV.z, v->vertex.z);
  }

  bounds.min = minV;
  bounds.max = maxV;
}

/**
 * @brief Método que calcula as normais dos vértices da malha
 *
 * @note este método é o método descrito por Foley para se determinar o vetor unitário normal a um vértice
 */
void Mesh::determineVertexNormals()
{

  for (auto v : this->vertexes)
  {
    HalfEdge *start_he = v->incident_edge;
    HalfEdge *he = start_he;

    Vec3f normal = {0.0f, 0.0f, 0.0f};

    while (true)
    {

      Vec3f face_normal = he->incident_face->normal;

      normal.x += face_normal.x;
      normal.y += face_normal.y;
      normal.z += face_normal.z;

      he = he->twin->next;

      if (he == start_he)
        break;
    }

    float length = sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    v->normal = normal;
  }
}