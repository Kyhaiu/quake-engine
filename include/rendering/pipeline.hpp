#pragma once

#include <imgui/imgui.h>

#include <core/types.hpp>
#include <math/math.hpp>
#include <models/color.hpp>
#include <models/light.hpp>
#include <algorithm>

#include <iostream>

namespace pipeline
{
  Matrix sru_to_src(const Vec3f &vrp, const Vec3f focal_point);
  Matrix projection(const Vec3f &vrp, const Vec3f p, const float dist_proj_plane);
  Matrix src_to_srt(const Vec2f min_window, const Vec2f min_viewport, const Vec2f max_window, const Vec2f max_viewport, bool reflected);

  // Recorte

#define INSIDE 0b000000
#define LEFT 0b000001
#define RIGHT 0b000010
#define BOTTOM 0b000100
#define TOP 0b001000
#define NEAR 0b010000
#define FAR 0b100000

  bool is_inside(Vec3f p, Vec2f min, Vec2f max, unsigned int edge);
  // usada no flat
  Vec3f compute_intersection(Vec3f p1, Vec3f p2, Vec2f min, Vec2f max, unsigned int edge);
  // usada no gouraud
  std::pair<Vec3f, models::Color> compute_intersection(std::pair<Vec3f, models::Color> p1, std::pair<Vec3f, models::Color> p2, Vec2f min, Vec2f max, unsigned int edge);
  // usada no phong
  std::pair<Vec3f, Vec3f> compute_intersection(std::pair<Vec3f, Vec3f> p1, std::pair<Vec3f, Vec3f> p2, Vec2f min, Vec2f max, unsigned int edge);

  // mesma coisa ocorre aqui, uma das funções de clip é usada no flat, outra no gouraud e phong
  std::vector<Vec3f> clip_2D_polygon(const std::vector<Vec3f> &polygon, const Vec2f &min, const Vec2f &max);
  std::vector<std::pair<Vec3f, models::Color>> clip_2D_polygon(const std::vector<std::pair<Vec3f, models::Color>> &polygon, const Vec2f &min, const Vec2f &max);
  std::vector<std::pair<Vec3f, Vec3f>> clip_2D_polygon(const std::vector<std::pair<Vec3f, Vec3f>> &polygon, const Vec2f &min, const Vec2f &max);

  // Desenhos pixel-a-pixel
  void setPixel(const Vec3f pixel, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void DrawBuffer(ImDrawList *draw_list, const std::vector<std::vector<float>> &z_buffer, const std::vector<std::vector<models::Color>> &color_buffer, Vec2f min_window_size);
  void DrawVertexBuffer(const Vec3f point, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer, const int size = 3);
  void DrawLineBuffer(const std::vector<Vec3f> &vertexes, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);

  // Rasterização
  void z_buffer(const Vec3f pixel, const models::Color &color, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void fill_polygon_flat(const std::vector<Vec3f> &vertexes, const models::GlobalLight &global_light, const std::vector<models::Omni> &omni_lights, const Vec3f &eye, const Vec3f &face_centroid, const Vec3f &face_normal, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void fill_polygon_gourand(const std::vector<std::pair<Vec3f, models::Color>> &vertexes, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);
  void fill_polygon_phong(const std::vector<std::pair<Vec3f, Vec3f>> &vertexes, const Vec3f &centroid, const models::GlobalLight &global_light, const std::vector<models::Omni> &omni_lights, const Vec3f &eye, const models::Material &object_material, std::vector<std::vector<float>> &z_buffer, std::vector<std::vector<models::Color>> &color_buffer);

  // Outras funções
  std::vector<Vec3f> BresenhamLine(Vec3f start, Vec3f end);

};
