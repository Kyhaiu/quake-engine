#pragma once

#include <core/types.hpp>
#include <math/math.hpp>
#include <models/colision.hpp>

#include <iostream>

class Player
{
public:
  // Vetor 3D que contém as coordenadas da posição da câmera
  Vec3f position;

  // Vetor 3D que contém as coordenadas do ponto para onde a câmera está olhando
  Vec3f target;

  // Vetor 3D que contém as coordenadas do vetor que representa a direção para cima da câmera
  Vec3f up;

  // Distância do plano de projeção
  float d;

  // Distância do plano de projeção mais próximo
  float near;

  // Distância do plano de projeção mais distante
  float far;

  // Tamanho da caixa de colisão
  float collisionSize = 1.0f;

  // Angulo na vertical
  float pitch = 0.0f;

  // Angulo na horizontal
  float yaw = 0.0f;

  // Construtor e Destrutor
  Player();
  ~Player();

  // ========================
  // Vetores de direção
  // ========================
  Vec3f getPlayerForward();
  Vec3f getPlayerRight();
  Vec3f getPlayerUp();

  // ========================
  // Movimentação
  // ========================
  void PlayerMoveForward(float distance, bool moveInWorldPlane = true);
  void PlayerMoveRight(float distance, bool moveInWorldPlane = true);
  void PlayerMoveUp(float distance);

  // Retorna posição simulada caso o player se mova
  Vec3f PlayerPretendingPosition(char key, float distance, bool moveInWorldPlane = true);

  // ========================
  // Rotação
  // ========================
  void PlayerRotateYaw(float angle);   // gira esquerda/direita
  void PlayerRotatePitch(float angle); // gira cima/baixo
  void updateTargetFromAngles();       // Ajusta a camera com base no pitch e yaw

  // ========================
  // Colisão
  // ========================
  AABB getBounds() const;
};
