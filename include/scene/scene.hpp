#pragma once

// Tipos
#include <core/types.hpp>
#include <models/color.hpp>
#include <models/light.hpp>
// Objetos
#include <models/mesh.hpp>
// Jogador
#include <entities/player.hpp>
// Pipeline de visualização
#include <rendering/pipeline.hpp>
#include <math/math.hpp>

class Scene
{
public:
  // Vetor que contém todos os objetos da cena
  std::vector<Mesh *> objects;

  // Informações do jogador
  Player *player;

  // Coordenadas minímas da tela
  Vec2f min_viewport;

  // Coordenadas máximas da tela
  Vec2f max_viewport;

  // Coordenadas minímas da janela de visualização
  Vec2f min_window;

  // Coordenadas máximas da janela de visualização
  Vec2f max_window;

  // Buffer de profundidade
  std::vector<std::vector<float>> z_buffer;

  // Buffer de cor
  std::vector<std::vector<models::Color>> color_buffer;

  // Lampadas omni na cena
  std::vector<models::Omni> omni_lights;

  models::GlobalLight global_light;

  // Construtor e destrutor
  Scene();
  ~Scene();

  // Inicialização de buffers (executado no inicio da geração de quadros)
  void initialize_buffers();

  // Funções para gerencia da cena
  void add_objects(Mesh *object);
  void remove_object(Mesh *object);

  // Pipeline de visualização

  // verifica se o objeto está entre o plano near e far
  // Esse clipping é feito no 3D* (Nosso pipeline faz uma simplificação do real recorte 3D)
  // devido a ele não ter o volume de visualização normalizado
  void clipping();

  // aplica o pipeline com o sombreamento flat
  void apply_pipeline_flat();
  // aplica o pipeline com o sombreamento gouraud
  void apply_pipeline_gouraud();
  // aplica o pipeline com o sombreamento phong
  void apply_pipeline_phong();

  // Colisão
  bool checkPlayerCollision(const Vec3f &newPos);
};