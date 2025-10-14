#pragma once

#include <core/types.hpp>

namespace models
{

  /**
   * @brief Cor no espaço RGB, separadas por canal
   *
   * @param r Vermelho
   * @param g Verde
   * @param b Azul
   *
   */
  typedef struct
  {
    float r;
    float g;
    float b;
  } ColorChannels;

  /**
   * @brief Propriedades de um material
   *
   * @param ambient Cor ambiente (separa por canal)
   * @param diffuse Cor difusa (separa por canal)
   * @param specular Cor especular (separa por canal)
   *
   * @note usado apenas na iluminação (Flat, Gouraud, Phong)
   * @note cada componente possui um índice K associado Ka, Kd e Ks
   * @note esses indices são divididos em 3 canais (RGB)
   * @note eles determinam os efeitos de iluminação difusa e especular (opacidade e brilho)
   */
  struct Material
  {

    models::ColorChannels ambient;
    models::ColorChannels diffuse;
    models::ColorChannels specular;

    float shininess;
  };
}