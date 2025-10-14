#pragma once

#include <core/types.hpp>

struct AABB
{
  Vec3f min;
  Vec3f max;

  bool intersects(const AABB &other) const
  {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
  }

  bool contains(const Vec3f &point) const
  {
    return (point.x >= min.x && point.x <= max.x) &&
           (point.y >= min.y && point.y <= max.y) &&
           (point.z >= min.z && point.z <= max.z);
  }
};