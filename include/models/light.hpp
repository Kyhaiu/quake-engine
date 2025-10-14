#pragma once

#include <core/types.hpp>
#include <models/color.hpp>
#include <models/common.hpp>
#include <math/math.hpp>

#include <vector>
#include <tuple>
#include <string>

namespace models
{
  typedef struct Omni
  {
    // posição da luz
    Vec3f position;
    // Intensidade da luz (RGB)
    ColorChannels intensity;
    // Id
    std::string id;
  } Omni;

  typedef struct GlobalLight
  {
    Color intensity;

    GlobalLight() : intensity(models::WHITE) {}
  } GlobalLight;

#define FLAT_SHADING 0
#define GOURAUD_SHADING 1
#define PHONG_SHADING 2

  void LightOrbital(Omni *omni, float orbitalSpeed);

  Color FlatShading(const GlobalLight &globalLight, const std::vector<Omni> &omni, const Vec3f &centroid, const Vec3f &face_normal, const Vec3f &eye, const Material &material);
  Color GouraudShading(const GlobalLight &globalLight, const std::vector<Omni> &omni, const std::pair<Vec3f, Vec3f> &vertex, const Vec3f &eye, const Material &material);
  Color PhongShading(const GlobalLight &globalLight, const std::vector<Omni> &omni, const Vec3f &centroid, const Vec3f &pixel, const Vec3f &pixel_normal, const Vec3f &eye, const Material &material);

}