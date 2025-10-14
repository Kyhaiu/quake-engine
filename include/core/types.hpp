#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <initializer_list>
#include <stdexcept>

typedef struct Vec2f
{
  // Coordenadas x, y
  float x, y;

  // Construtor com valores padrão
  Vec2f(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
} Vec2f;

typedef struct Vec3f
{
  // Coordenadas x, y, z
  float x, y, z;

  // Construtor com valores padrão
  Vec3f(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

  // Operador de soma
  Vec3f operator+(const Vec3f &other) const
  {
    return {x + other.x, y + other.y, z + other.z};
  }

  // Operador de subtração
  Vec3f operator-(const Vec3f &other) const
  {
    return {x - other.x, y - other.y, z - other.z};
  }

  // Operador de multiplicação por escalar
  Vec3f operator*(float scalar) const
  {
    return {x * scalar, y * scalar, z * scalar};
  }

  // Operador de multiplicação com outro vetor (component-wise)
  Vec3f operator*(const Vec3f &other) const
  {
    return {x * other.x, y * other.y, z * other.z};
  }

  // operator << - Overloads the << operator to print the Vector4
  friend std::ostream &operator<<(std::ostream &os, const Vec3f &v)
  {
    os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z;
    return os;
  }
} Vec3f;

typedef struct Vec4f
{
  // Coordenadas x, y, z e w (coordenada homogênea)
  float x, y, z, w;

  // A coordenada homogênea nunca pode ser 0.0f.

  // Construtor com valores padrão
  Vec4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1) : x(x), y(y), z(z), w(w) {}

  Vec3f to_vec3() { return {x, y, z}; }

  // operator << - Overloads the << operator to print the Vector4
  friend std::ostream &operator<<(std::ostream &os, const Vec4f &v)
  {
    os << "x: " << v.x << ", y: " << v.y << ", z: " << v.z << ", w: " << v.w;
    return os;
  }
} Vec4f;

/**
 * - A estrutura Matrix é definida como row-major (layout de memória) mas os nomes dos parâmetros
 *    e todas as operações matemáticas realizadas pela biblioteca consideram a estrutura como se fosse
 *    column-major. É como se versões transpostas das matrizes fossem usadas para toda a matemática
 *
 * - Isso beneficia algumas funções tornando-as cache-friendly e também evita matrizes transpostas
 *    às vezes. Isto é apenas uma camada de abstração.
 *
 * Exemplo: Na ordem da memória, row0 é [m0 m4 m8 m12] mas na matemática semântica row0 é [m0 m1 m2 m3]
 */

typedef struct Matrix
{
  float m0, m4, m8, m12;
  float m1, m5, m9, m13;
  float m2, m6, m10, m14;
  float m3, m7, m11, m15;

  // ============================================================
  // Construtor padrão (identidade)
  // ============================================================
  Matrix()
      : m0(1), m4(0), m8(0), m12(0),
        m1(0), m5(1), m9(0), m13(0),
        m2(0), m6(0), m10(1), m14(0),
        m3(0), m7(0), m11(0), m15(1)
  {
  }

  static Matrix fromList(float list[16])
  {
    Matrix result{};
    result.m0 = list[0];
    result.m1 = list[1];
    result.m2 = list[2];
    result.m3 = list[3];
    result.m4 = list[4];
    result.m5 = list[5];
    result.m6 = list[6];
    result.m7 = list[7];
    result.m8 = list[8];
    result.m9 = list[9];
    result.m10 = list[10];
    result.m11 = list[11];
    result.m12 = list[12];
    result.m13 = list[13];
    result.m14 = list[14];
    result.m15 = list[15];

    return result;
  }

  // ============================================================
  // Operador de acesso não-const
  // Permite acessar e MODIFICAR elementos da matriz usando sintaxe (i, j)
  // Exemplo: m(1, 2) = 5.0f;
  // ============================================================
  float &operator()(int i, int j)
  {
    return ((float *)this)[i + j * 4];
  }

  // ============================================================
  // Operador de acesso const
  // Permite acessar elementos da matriz SEM modificá-los
  // Exemplo: float x = m(1, 2);
  // É usado automaticamente quando o objeto é declarado const.
  // ============================================================
  const float &operator()(int i, int j) const
  {
    return ((float *)this)[i + j * 4];
  }

} Matrix;
