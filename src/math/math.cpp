#include <math/math.hpp>

/**
 * @brief Restringe um valor para estar dentro de um intervalo especificado.
 *
 * Esta função recebe um valor de ponto flutuante e garante que ele esteja
 * dentro do intervalo definido pelos valores mínimo e máximo fornecidos. Se o
 * valor for menor que o mínimo, o mínimo é retornado. Se o valor for maior que
 * o máximo, o máximo é retornado. Caso contrário, o valor original é retornado.
 *
 * @param value O valor de ponto flutuante a ser restringido.
 * @param min O valor mínimo permitido.
 * @param max O valor máximo permitido.
 * @return O valor restringido, que estará dentro do intervalo [min, max].
 */
float Clamp(float value, float min, float max)
{
  float result = (value < min) ? min : value;

  if (result > max)
    result = max;

  return result;
}

/**
 * @brief Interpola linearmente entre dois valores.
 *
 * Esta função calcula uma interpolação linear entre dois valores de ponto
 * flutuante, `start` e `end`, com base na proporção `amount`. O valor `amount`
 * deve estar no intervalo [0, 1], onde 0 retorna `start` e 1 retorna `end`.
 * Qualquer valor intermediário de `amount` retorna um ponto entre `start` e `end`.
 *
 * @param start O valor inicial.
 * @param end O valor final.
 * @param amount A proporção para a interpolação, deve estar no intervalo [0, 1].
 * @return O valor interpolado.
 */
float Lerp(float start, float end, float amount)
{
  return start + amount * (end - start);
}

/**
 * @brief Normaliza um valor para um intervalo específico.
 *
 * Esta função recebe um valor de ponto flutuante e normaliza-o para o intervalo
 * definido pelos valores de início e fim fornecidos. O valor retornado estará
 * no intervalo [0, 1] se o valor original estiver dentro do intervalo [start, end].
 *
 * @param value O valor de ponto flutuante a ser normalizado.
 * @param start O valor de início do intervalo.
 * @param end O valor final do intervalo.
 * @return O valor normalizado, que estará no intervalo [0, 1].
 */
float Normalize(float value, float start, float end)
{
  float result = (value - start) / (end - start);

  return result;
}

/**
 * @brief Remapeia um valor de um intervalo de entrada para um intervalo de saída.
 *
 * Esta função remapeia um valor de ponto flutuante, `value`, de um intervalo de entrada
 * definido pelos valores `inputStart` e `inputEnd` para um intervalo de saída definido
 * pelos valores `outputStart` e `outputEnd`.
 *
 * @param value O valor de ponto flutuante a ser remapeado.
 * @param inputStart O início do intervalo de entrada.
 * @param inputEnd O fim do intervalo de entrada.
 * @param outputStart O início do intervalo de saída.
 * @param outputEnd O fim do intervalo de saída.
 * @return O valor remapeado para o intervalo de saída.
 */
float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd)
{
  float result = outputStart + ((outputEnd - outputStart) * ((value - inputStart) / (inputEnd - inputStart)));

  return result;
}

/**
 * @brief Envolve um valor dentro de um intervalo especificado.
 *
 * Esta função envolve um valor de ponto flutuante, `value`, dentro do intervalo
 * definido pelos valores `min` e `max`. Se `value` ultrapassar o limite superior
 * (`max`), ele será "enrolado" de volta para o limite inferior (`min`) e vice-versa.
 *
 * @param value O valor de ponto flutuante a ser envolvido.
 * @param min O valor mínimo do intervalo.
 * @param max O valor máximo do intervalo.
 * @return O valor envolvido dentro do intervalo [min, max].
 */
float Wrap(float value, float min, float max)
{
  float result = value - (max - min) * floorf((value - min) / (max - min));

  return result;
}

//----------------------------------------------------------------------------------
// Module Functions Declaration - Vector2 math functions
//----------------------------------------------------------------------------------

/**
 * @brief Normaliza um vetor 2D.
 *
 * Esta função normaliza um vetor bidimensional, `a`, e retorna o resultado como
 * um novo vetor bidimensional. O vetor resultante terá comprimento 1.
 *
 * @param a O vetor bidimensional a ser normalizado.
 * @return O vetor bidimensional normalizado.
 */
Vec2f Vector2Normalize(Vec2f a)
{
  float length = sqrtf(a.x * a.x + a.y * a.y);

  Vec2f result = {a.x / length, a.y / length};

  return result;
}

/**
 * @brief Calcula o produto escalar de dois vetores 2D.
 *
 * Esta função calcula o produto escalar de dois vetores bidimensionais(Vec2f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor bidimensional.
 * @param b O segundo vetor bidimensional.
 * @return O produto escalar dos vetores.
 */
float Vector2DotProduct(Vec2f a, Vec2f b)
{
  float result = (a.x * b.x) + (a.y * b.y);

  return result;
}

/**
 * @brief Calcula a distância entre dois vetores 2D.
 *
 * Esta função calcula a distância entre dois vetores bidimensionais(Vec2f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor bidimensional.
 * @param b O segundo vetor bidimensional.
 * @return A distância entre os vetores.
 */
float Vector2Distance(Vec2f a, Vec2f b)
{
  float result = sqrtf(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));

  return result;
}

/**
 * @brief Calcula o ângulo entre dois vetores 2D.
 *
 * Esta função calcula o ângulo entre dois vetores bidimensionais(Vec2f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor bidimensional.
 * @param b O segundo vetor bidimensional.
 * @return O ângulo entre os vetores.
 */
float Vector2Angle(Vec2f a, Vec2f b)
{
  float result = 0.0f;

  float dot = a.x * b.x + a.y * b.y;
  float det = a.x * b.y - a.y * b.x;

  result = atan2f(det, dot);

  return result;
}

//----------------------------------------------------------------------------------
// Module Functions Declaration - Vector3 math functions
//----------------------------------------------------------------------------------

/**
 * @brief Normaliza um vetor 3D.
 *
 * Esta função normaliza um vetor tridimensional, `a`, e retorna o resultado como
 * um novo vetor tridimensional. O vetor resultante terá comprimento 1.
 *
 * @param a O vetor tridimensional a ser normalizado.
 * @return O vetor tridimensional normalizado.
 */
Vec3f Vector3Normalize(Vec3f a)
{
  Vec3f result = {0.0f, 0.0f, 0.0f};

  float length = sqrtf((a.x * a.x) + (a.y * a.y) + (a.z * a.z));

  if (length != 0.0f)
  {
    result.x = a.x / length;
    result.y = a.y / length;
    result.z = a.z / length;
  }

  return result;
}

/**
 * @brief Calcula o produto escalar de dois vetores 3D.
 *
 * Esta função calcula o produto escalar de dois vetores tridimensionais(Vec3f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor tridimensional.
 * @param b O segundo vetor tridimensional.
 * @return O produto escalar dos vetores.
 */
float Vector3DotProduct(Vec3f a, Vec3f b)
{
  float result = (a.x * b.x) + (a.y * b.y) + (a.z * b.z);

  return result;
}

/**
 * @brief Calcula o produto vetorial de dois vetores 3D.
 *
 * Esta função calcula o produto vetorial de dois vetores tridimensionais(Vec3f),
 * `a` e `b`, e retorna o resultado como um novo vetor tridimensional.
 *
 * @param a O primeiro vetor tridimensional.
 * @param b O segundo vetor tridimensional.
 * @return O vetor tridimensional resultante do produto vetorial.
 */
Vec3f Vector3CrossProduct(Vec3f a, Vec3f b)
{
  float x = a.y * b.z - a.z * b.y;
  float y = a.z * b.x - a.x * b.z;
  float z = a.x * b.y - a.y * b.x;

  Vec3f result = {x, y, z};

  return result;
}

/**
 * @brief Calcula a distância entre dois vetores 3D.
 *
 * Esta função calcula a distância entre dois vetores tridimensionais(Vec3f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor tridimensional.
 * @param b O segundo vetor tridimensional.
 * @return A distância entre os vetores.
 */
float Vector3Distance(Vec3f a, Vec3f b)
{
  float result = 0.0f;

  float dx = b.x - a.x;
  float dy = b.y - a.y;
  float dz = b.z - a.z;
  result = sqrtf(dx * dx + dy * dy + dz * dz);

  return result;
}

/**
 * @brief Calcula o ângulo entre dois vetores 3D.
 *
 * Esta função calcula o ângulo entre dois vetores tridimensionais(Vec3f),
 * `a` e `b`, e retorna o resultado como um valor de ponto flutuante.
 *
 * @param a O primeiro vetor tridimensional.
 * @param b O segundo vetor tridimensional.
 * @return O ângulo entre os vetores.
 */
float Vector3Angle(Vec3f a, Vec3f b)
{
  float result = 0.0f;

  float x = a.y * b.z - a.z * b.y;
  float y = a.z * b.x - a.x * b.z;
  float z = a.x * b.y - a.y * b.x;

  Vec3f cross = {x, y, z};

  float len = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z);
  float dot = (a.x * b.x + a.y * b.y + a.z * b.z);
  result = atan2f(len, dot);

  return result;
}

/**
 * @brief Transforma um vetor 3D por uma matriz de transformação.
 *
 * Esta função transforma um vetor tridimensional por uma matriz de transformação
 * e retorna o resultado como um novo vetor tridimensional.
 *
 * @param a O vetor tridimensional a ser transformado.
 * @param mat A matriz de transformação.
 * @return O vetor tridimensional resultante da transformação.
 */
Vec3f Vector3Transform(Vec3f a, Matrix mat)
{
  Vec3f result = {0};

  float x = a.x;
  float y = a.y;
  float z = a.z;

  result.x = mat.m0 * x + mat.m4 * y + mat.m8 * z + mat.m12;
  result.y = mat.m1 * x + mat.m5 * y + mat.m9 * z + mat.m13;
  result.z = mat.m2 * x + mat.m6 * y + mat.m10 * z + mat.m14;

  return result;
}

/**
 * @brief Função que rotaciona um vetor 3D por um eixo e um ângulo.
 *
 * Esta função rotaciona um vetor tridimensional por um eixo e um ângulo e retorna o
 * resultado como um novo vetor tridimensional.
 *
 * @param a O vetor tridimensional a ser rotacionado.
 * @param axis O eixo de rotação.
 * @param angle O ângulo de rotação.
 * @return O vetor tridimensional resultante da rotação.
 */
Vec3f Vector3RotateByAxisAngle(Vec3f a, Vec3f axis, float angle)
{

  // Using Euler-Rodrigues Formula
  // Ref.: https://en.wikipedia.org/w/index.php?title=Euler%E2%80%93Rodrigues_formula

  Vec3f result = a;

  // Vector3Normalize(axis);
  float length = sqrtf(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
  if (length == 0.0f)
    length = 1.0f;
  float ilength = 1.0f / length;
  axis.x *= ilength;
  axis.y *= ilength;
  axis.z *= ilength;

  angle /= 2.0f;
  float _a = sinf(angle);
  float b = axis.x * _a;
  float c = axis.y * _a;
  float d = axis.z * _a;
  _a = cosf(angle);
  Vec3f w = {b, c, d};

  // Vector3CrossProduct(w, v)
  Vec3f wv = {w.y * a.z - w.z * a.y, w.z * a.x - w.x * a.z, w.x * a.y - w.y * a.x};

  // Vector3CrossProduct(w, wv)
  Vec3f wwv = {w.y * wv.z - w.z * wv.y, w.z * wv.x - w.x * wv.z, w.x * wv.y - w.y * wv.x};

  // Vector3Scale(wv, 2*a)
  _a *= 2;
  wv.x *= _a;
  wv.y *= _a;
  wv.z *= _a;

  // Vector3Scale(wwv, 2)
  wwv.x *= 2;
  wwv.y *= 2;
  wwv.z *= 2;

  result.x += wv.x;
  result.y += wv.y;
  result.z += wv.z;

  result.x += wwv.x;
  result.y += wwv.y;
  result.z += wwv.z;

  return result;
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Matrix math functions
//----------------------------------------------------------------------------------

/**
 * @brief Função que soma duas matrizes.
 *
 * Esta função calcula a soma de duas matrizes 4x4(Matrix), `a` e `b`, e retorna o resultado como uma nova matriz 4x4.
 *
 * @param a A primeira matriz 4x4.
 * @param b A segunda matriz 4x4.
 * @return A matriz 4x4 resultante da soma.
 */
Matrix MatrixAdd(Matrix a, Matrix b)
{
  Matrix result;

  result.m0 = a.m0 + b.m0;
  result.m1 = a.m1 + b.m1;
  result.m2 = a.m2 + b.m2;
  result.m3 = a.m3 + b.m3;
  result.m4 = a.m4 + b.m4;
  result.m5 = a.m5 + b.m5;
  result.m6 = a.m6 + b.m6;
  result.m7 = a.m7 + b.m7;
  result.m8 = a.m8 + b.m8;
  result.m9 = a.m9 + b.m9;
  result.m10 = a.m10 + b.m10;
  result.m11 = a.m11 + b.m11;
  result.m12 = a.m12 + b.m12;
  result.m13 = a.m13 + b.m13;
  result.m14 = a.m14 + b.m14;
  result.m15 = a.m15 + b.m15;

  return result;
}

/**
 * @brief Subtrai duas matrizes.
 *
 * Esta função calcula a diferença entre duas matrizes 4x4(Matrix), `a` e `b`, e retorna o resultado como uma nova matriz 4x4.
 *
 * @param a A primeira matriz 4x4.
 * @param b A segunda matriz 4x4.
 * @return A matriz 4x4 resultante da subtração.
 */
Matrix MatrixSubtract(Matrix a, Matrix b)
{
  Matrix result;

  result.m0 = a.m0 - b.m0;
  result.m1 = a.m1 - b.m1;
  result.m2 = a.m2 - b.m2;
  result.m3 = a.m3 - b.m3;
  result.m4 = a.m4 - b.m4;
  result.m5 = a.m5 - b.m5;
  result.m6 = a.m6 - b.m6;
  result.m7 = a.m7 - b.m7;
  result.m8 = a.m8 - b.m8;
  result.m9 = a.m9 - b.m9;
  result.m10 = a.m10 - b.m10;
  result.m11 = a.m11 - b.m11;
  result.m12 = a.m12 - b.m12;
  result.m13 = a.m13 - b.m13;
  result.m14 = a.m14 - b.m14;
  result.m15 = a.m15 - b.m15;

  return result;
}

/**
 * @brief Multiplica duas matrizes.
 *
 * Esta função calcula o produto de duas matrizes 4x4(Matrix), `a` e `b`, e retorna o resultado como uma nova matriz 4x4.
 *
 * @param a A primeira matriz 4x4.
 * @param b A segunda matriz 4x4.
 * @return A matriz 4x4 resultante da multiplicação.
 */
Matrix MatrixMultiply(Matrix a, Matrix b)
{
  Matrix result;

  result.m0 = a.m0 * b.m0 + a.m1 * b.m4 + a.m2 * b.m8 + a.m3 * b.m12;
  result.m1 = a.m0 * b.m1 + a.m1 * b.m5 + a.m2 * b.m9 + a.m3 * b.m13;
  result.m2 = a.m0 * b.m2 + a.m1 * b.m6 + a.m2 * b.m10 + a.m3 * b.m14;
  result.m3 = a.m0 * b.m3 + a.m1 * b.m7 + a.m2 * b.m11 + a.m3 * b.m15;
  result.m4 = a.m4 * b.m0 + a.m5 * b.m4 + a.m6 * b.m8 + a.m7 * b.m12;
  result.m5 = a.m4 * b.m1 + a.m5 * b.m5 + a.m6 * b.m9 + a.m7 * b.m13;
  result.m6 = a.m4 * b.m2 + a.m5 * b.m6 + a.m6 * b.m10 + a.m7 * b.m14;
  result.m7 = a.m4 * b.m3 + a.m5 * b.m7 + a.m6 * b.m11 + a.m7 * b.m15;
  result.m8 = a.m8 * b.m0 + a.m9 * b.m4 + a.m10 * b.m8 + a.m11 * b.m12;
  result.m9 = a.m8 * b.m1 + a.m9 * b.m5 + a.m10 * b.m9 + a.m11 * b.m13;
  result.m10 = a.m8 * b.m2 + a.m9 * b.m6 + a.m10 * b.m10 + a.m11 * b.m14;
  result.m11 = a.m8 * b.m3 + a.m9 * b.m7 + a.m10 * b.m11 + a.m11 * b.m15;
  result.m12 = a.m12 * b.m0 + a.m13 * b.m4 + a.m14 * b.m8 + a.m15 * b.m12;
  result.m13 = a.m12 * b.m1 + a.m13 * b.m5 + a.m14 * b.m9 + a.m15 * b.m13;
  result.m14 = a.m12 * b.m2 + a.m13 * b.m6 + a.m14 * b.m10 + a.m15 * b.m14;
  result.m15 = a.m12 * b.m3 + a.m13 * b.m7 + a.m14 * b.m11 + a.m15 * b.m15;

  return result;
}

/**
 * @brief Multiplica uma matriz por um vetor 4D.
 *
 * @param mat A matriz 4x4.
 * @param vec O vetor 4D.
 * @note O vetor 4D é tratado como uma matriz 4x1.
 * @note Esta função é utilizada para transformar um vetor 4D por uma matriz 4x4.
 * @return O vetor 4D resultante da multiplicação.
 */
Vec4f MatrixMultiplyVector(Matrix mat, Vec4f vec)
{
  Vec4f result;

  result.x = mat.m0 * vec.x + mat.m1 * vec.y + mat.m2 * vec.z + mat.m3 * vec.w;
  result.y = mat.m4 * vec.x + mat.m5 * vec.y + mat.m6 * vec.z + mat.m7 * vec.w;
  result.z = mat.m8 * vec.x + mat.m9 * vec.y + mat.m10 * vec.z + mat.m11 * vec.w;
  result.w = mat.m12 * vec.x + mat.m13 * vec.y + mat.m14 * vec.z + mat.m15 * vec.w;

  return result;
}

/**
 * @brief Função para multiplicar uma matriz por um escalar.
 *
 * @param mat Matriz 4x4
 * @param scalar Escalar
 * @return Matrix
 */
Matrix MatrixMultiplyValue(Matrix mat, float scalar)
{
  Matrix result;

  result.m0 = mat.m0 * scalar;
  result.m1 = mat.m1 * scalar;
  result.m2 = mat.m2 * scalar;
  result.m3 = mat.m3 * scalar;
  result.m4 = mat.m4 * scalar;
  result.m5 = mat.m5 * scalar;
  result.m6 = mat.m6 * scalar;
  result.m7 = mat.m7 * scalar;
  result.m8 = mat.m8 * scalar;
  result.m9 = mat.m9 * scalar;
  result.m10 = mat.m10 * scalar;
  result.m11 = mat.m11 * scalar;
  result.m12 = mat.m12 * scalar;
  result.m13 = mat.m13 * scalar;
  result.m14 = mat.m14 * scalar;
  result.m15 = mat.m15 * scalar;

  return result;
}

/**
 * @brief Função que calcula o determinante de uma matriz 4x4.
 *
 * Esta função calcula o determinante de uma matriz 4x4(Matrix) e retorna o resultado como um valor de ponto flutuante.
 *
 * @param mat A matriz 4x4.
 * @return O determinante da matriz.
 */
float MatrixDeterminant(Matrix mat)
{
  float result = 0.0f;

  // Cache the matrix to avoid repeated lookups
  float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
  float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
  float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
  float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

  result = a30 * a21 * a12 * a03 - a20 * a31 * a12 * a03 - a30 * a11 * a22 * a03 + a10 * a31 * a22 * a03 +
           a20 * a11 * a32 * a03 - a10 * a21 * a32 * a03 - a30 * a21 * a02 * a13 + a20 * a31 * a02 * a13 +
           a30 * a01 * a22 * a13 - a00 * a31 * a22 * a13 - a20 * a01 * a32 * a13 + a00 * a21 * a32 * a13 +
           a30 * a11 * a02 * a23 - a10 * a31 * a02 * a23 - a30 * a01 * a12 * a23 + a00 * a31 * a12 * a23 +
           a10 * a01 * a32 * a23 - a00 * a11 * a32 * a23 - a20 * a11 * a02 * a33 + a10 * a21 * a02 * a33 +
           a20 * a01 * a12 * a33 - a00 * a21 * a12 * a33 - a10 * a01 * a22 * a33 + a00 * a11 * a22 * a33;

  return result;
}

/**
 * @brief Transpõe uma matriz.
 *
 * Esta função transpõe uma matriz 4x4(Matrix) e retorna o resultado como uma nova matriz 4x4.
 *
 * @param mat A matriz 4x4.
 * @return A matriz
 */
Matrix MatrixTranspose(Matrix mat)
{
  Matrix result;

  result.m0 = mat.m0;
  result.m1 = mat.m4;
  result.m2 = mat.m8;
  result.m3 = mat.m12;
  result.m4 = mat.m1;
  result.m5 = mat.m5;
  result.m6 = mat.m9;
  result.m7 = mat.m13;
  result.m8 = mat.m2;
  result.m9 = mat.m6;
  result.m10 = mat.m10;
  result.m11 = mat.m14;
  result.m12 = mat.m3;
  result.m13 = mat.m7;
  result.m14 = mat.m11;
  result.m15 = mat.m15;

  return result;
}

/**
 * @brief Inverte uma matriz.
 *
 * Esta função inverte uma matriz 4x4(Matrix) e retorna o resultado como uma nova matriz 4x4.
 *
 * @param mat A matriz 4x4.
 * @return A matriz resultante da inversão.
 */
Matrix MatrixInvert(Matrix mat)
{
  Matrix result;

  // Cache the matrix values (speed optimization)
  float a00 = mat.m0, a01 = mat.m1, a02 = mat.m2, a03 = mat.m3;
  float a10 = mat.m4, a11 = mat.m5, a12 = mat.m6, a13 = mat.m7;
  float a20 = mat.m8, a21 = mat.m9, a22 = mat.m10, a23 = mat.m11;
  float a30 = mat.m12, a31 = mat.m13, a32 = mat.m14, a33 = mat.m15;

  float b00 = a00 * a11 - a01 * a10;
  float b01 = a00 * a12 - a02 * a10;
  float b02 = a00 * a13 - a03 * a10;
  float b03 = a01 * a12 - a02 * a11;
  float b04 = a01 * a13 - a03 * a11;
  float b05 = a02 * a13 - a03 * a12;
  float b06 = a20 * a31 - a21 * a30;
  float b07 = a20 * a32 - a22 * a30;
  float b08 = a20 * a33 - a23 * a30;
  float b09 = a21 * a32 - a22 * a31;
  float b10 = a21 * a33 - a23 * a31;
  float b11 = a22 * a33 - a23 * a32;

  // Calculate the invert determinant (inlined to avoid double-caching)
  float invDet = 1.0f / (b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06);

  result.m0 = (a11 * b11 - a12 * b10 + a13 * b09) * invDet;
  result.m1 = (-a01 * b11 + a02 * b10 - a03 * b09) * invDet;
  result.m2 = (a31 * b05 - a32 * b04 + a33 * b03) * invDet;
  result.m3 = (-a21 * b05 + a22 * b04 - a23 * b03) * invDet;
  result.m4 = (-a10 * b11 + a12 * b08 - a13 * b07) * invDet;
  result.m5 = (a00 * b11 - a02 * b08 + a03 * b07) * invDet;
  result.m6 = (-a30 * b05 + a32 * b02 - a33 * b01) * invDet;
  result.m7 = (a20 * b05 - a22 * b02 + a23 * b01) * invDet;
  result.m8 = (a10 * b10 - a11 * b08 + a13 * b06) * invDet;
  result.m9 = (-a00 * b10 + a01 * b08 - a03 * b06) * invDet;
  result.m10 = (a30 * b04 - a31 * b02 + a33 * b00) * invDet;
  result.m11 = (-a20 * b04 + a21 * b02 - a23 * b00) * invDet;
  result.m12 = (-a10 * b09 + a11 * b07 - a12 * b06) * invDet;
  result.m13 = (a00 * b09 - a01 * b07 + a02 * b06) * invDet;
  result.m14 = (-a30 * b03 + a31 * b01 - a32 * b00) * invDet;
  result.m15 = (a20 * b03 - a21 * b01 + a22 * b00) * invDet;

  return result;
}

/**
 * @brief Obtém uma matriz de translação com deslocamento em x, y e z. Representados por um vetor 3D.
 *
 * @param Vec3f A quantidade de deslocamento em x, y e z.
 * @return A matriz de translação resultante.
 */
Matrix MatrixTranslate(Vec3f translation)
{
  Matrix result;

  // Set the matrix to identity
  result.m0 = 1.0f;
  result.m5 = 1.0f;
  result.m10 = 1.0f;
  result.m15 = 1.0f;

  result.m12 = translation.x;
  result.m13 = translation.y;
  result.m14 = translation.z;

  return result;
}

/**
 * @brief Obtém uma matriz de rotação em torno de um eixo (x, y ou z).
 *
 * @param axis O eixo de rotação.
 * @param angle O ângulo de rotação.
 * @return A matriz de rotação resultante.
 */
Matrix MatrixRotate(Vec3f axis, float angle)
{
  Matrix result;

  float x = axis.x, y = axis.y, z = axis.z;

  float lengthSquared = x * x + y * y + z * z;

  if ((lengthSquared != 1.0f) && (lengthSquared != 0.0f))
  {
    float ilength = 1.0f / sqrtf(lengthSquared);
    x *= ilength;
    y *= ilength;
    z *= ilength;
  }

  float sinres = sinf(angle);
  float cosres = cosf(angle);
  float t = 1.0f - cosres;

  result.m0 = x * x * t + cosres;
  result.m1 = y * x * t + z * sinres;
  result.m2 = z * x * t - y * sinres;
  result.m3 = 0.0f;

  result.m4 = x * y * t - z * sinres;
  result.m5 = y * y * t + cosres;
  result.m6 = z * y * t + x * sinres;
  result.m7 = 0.0f;

  result.m8 = x * z * t + y * sinres;
  result.m9 = y * z * t - x * sinres;
  result.m10 = z * z * t + cosres;
  result.m11 = 0.0f;

  result.m12 = 0.0f;
  result.m13 = 0.0f;
  result.m14 = 0.0f;
  result.m15 = 1.0f;

  return result;
}

/**
 * @brief Obtém uma matriz de escala com fatores de escala em x, y e z.
 *
 * @param scale Vetor 3D com fatores de escala em x, y e z.
 * @return Matrix
 */
Matrix MatrixScale(Vec3f scale)
{
  Matrix result = Matrix(); // MatrixIdentity()

  result.m0 = scale.x;
  result.m5 = scale.y;
  result.m10 = scale.z;
  result.m15 = 1.0f;

  return result;
}

/**
 * @brief Obtém uma matriz de rotação em torno do eixo X.
 *
 * @param angle O ângulo de rotação.
 * @return Matrix A matriz de rotação resultante.
 */
Matrix MatrixRotateX(float angle)
{
  Matrix result;

  float sinres = sinf(angle);
  float cosres = cosf(angle);

  result.m0 = 1.0f;
  result.m5 = cosres;
  result.m6 = -sinres;
  result.m9 = sinres;
  result.m10 = cosres;
  result.m15 = 1.0f;

  return result;
}

/**
 * @brief Obtém uma matriz de rotação em torno do eixo Y.
 *
 * @param angle O ângulo de rotação.
 * @return Matrix A matriz de rotação resultante.
 */
Matrix MatrixRotateY(float angle)
{
  Matrix result;

  float sinres = sinf(angle);
  float cosres = cosf(angle);

  result.m0 = cosres;
  result.m2 = sinres;
  result.m5 = 1.0f;
  result.m8 = -sinres;
  result.m10 = cosres;
  result.m15 = 1.0f;

  return result;
}

/**
 * @brief Obtém uma matriz de rotação em torno do eixo Z.
 *
 * @param angle O ângulo de rotação.
 * @return Matrix A matriz de rotação resultante.
 */
Matrix MatrixRotateZ(float angle)
{
  Matrix result;

  float sinres = sinf(angle);
  float cosres = cosf(angle);

  result.m0 = cosres;
  result.m1 = -sinres;
  result.m4 = sinres;
  result.m5 = cosres;
  result.m10 = 1.0f;
  result.m15 = 1.0f;

  return result;
}

/**
 * @brief Obtém uma matriz de rotação em torno dos eixos X, Y e Z.
 *
 * @param angle O ângulo de rotação.
 *
 * @note Os ângulos são fornecidos em radianos.
 * @return Matrix
 */
Matrix MatrixRotateXYZ(Vec3f angle)
{
  Matrix result = Matrix(); // MatrixIdentity()

  float cosz = cosf(-angle.z);
  float sinz = sinf(-angle.z);
  float cosy = cosf(-angle.y);
  float siny = sinf(-angle.y);
  float cosx = cosf(-angle.x);
  float sinx = sinf(-angle.x);

  result.m0 = cosz * cosy;
  result.m1 = (cosz * siny * sinx) - (sinz * cosx);
  result.m2 = (cosz * siny * cosx) + (sinz * sinx);

  result.m4 = sinz * cosy;
  result.m5 = (sinz * siny * sinx) + (cosz * cosx);
  result.m6 = (sinz * siny * cosx) - (cosz * sinx);

  result.m8 = -siny;
  result.m9 = cosy * sinx;
  result.m10 = cosy * cosx;

  return result;
}