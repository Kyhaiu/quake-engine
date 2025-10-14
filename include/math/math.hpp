#pragma once

#include <core/types.hpp>

//----------------------------------------------------------------------------------
// Defines and macros
//----------------------------------------------------------------------------------
#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#ifndef DEG2RAD
#define DEG2RAD (PI / 180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f / PI)
#endif

//----------------------------------------------------------------------------------
// Module Functions Declaration - Utils math functions
//----------------------------------------------------------------------------------
float Clamp(float value, float min, float max);
float Lerp(float start, float end, float amount);
float Normalize(float value, float start, float end);
float Remap(float value, float inputStart, float inputEnd, float outputStart, float outputEnd);
float Wrap(float value, float min, float max);

//----------------------------------------------------------------------------------
// Module Functions Declaration - Vector2 math functions
//----------------------------------------------------------------------------------
Vec2f Vector2Normalize(Vec2f a);
float Vector2DotProduct(Vec2f a, Vec2f b);
float Vector2Distance(Vec2f a, Vec2f b);
float Vector2Angle(Vec2f a, Vec2f b);

//----------------------------------------------------------------------------------
// Module Functions Declaration - Vector3 math functions
//----------------------------------------------------------------------------------
Vec3f Vector3Normalize(Vec3f a);
float Vector3DotProduct(Vec3f a, Vec3f b);
Vec3f Vector3CrossProduct(Vec3f a, Vec3f b);
float Vector3Distance(Vec3f a, Vec3f b);
float Vector3Angle(Vec3f a, Vec3f b);
Vec3f Vector3Transform(Vec3f a, Matrix mat);
Vec3f Vector3RotateByAxisAngle(Vec3f a, Vec3f axis, float angle);

//----------------------------------------------------------------------------------
// Module Functions Declaration - Matrix math functions
//----------------------------------------------------------------------------------

Matrix MatrixAdd(Matrix a, Matrix b);
Matrix MatrixSubtract(Matrix a, Matrix b);
Matrix MatrixMultiply(Matrix a, Matrix b);
Vec4f MatrixMultiplyVector(Matrix mat, Vec4f vec);
Matrix MatrixMultiplyValue(Matrix mat, float scalar);
float MatrixDeterminant(Matrix mat);
Matrix MatrixInvert(Matrix mat);
Matrix MatrixTranspose(Matrix mat);

// Matrix transformations
Matrix MatrixTranslate(Vec3f vec);
Matrix MatrixRotate(Vec3f axis, float angle);
Matrix MatrixScale(Vec3f vec);

Matrix RotationMatrixX(float angle);
Matrix RotationMatrixY(float angle);
Matrix RotationMatrixZ(float angle);

Matrix MatrixRotateXYZ(Vec3f angle);