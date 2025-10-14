#include <entities/player.hpp>

Player::Player()
{
  d = 20;
  near = 0.0f;
  far = 100.0f;
  up = {0.0f, 1.0f, 0.0f};
  position = {0.0f, 0.0f, 20.0f};
  target = {0.0f, 0.0f, 0.0f};
  yaw = 0.0f;
  pitch = 0.0f;
}

Player::~Player() = default;

// ========================
// VETORES DIRECIONAIS
// ========================

Vec3f Player::getPlayerForward()
{
  Vec3f forward;
  forward.x = cos(pitch) * sin(yaw);
  forward.y = sin(pitch);
  forward.z = cos(pitch) * cos(yaw);
  return Vector3Normalize(forward);
}

Vec3f Player::getPlayerRight()
{
  Vec3f forward = getPlayerForward();
  Vec3f right = Vector3CrossProduct(forward, {0.0f, 1.0f, 0.0f});
  return Vector3Normalize(right);
}

Vec3f Player::getPlayerUp()
{
  Vec3f right = getPlayerRight();
  Vec3f forward = getPlayerForward();
  return Vector3Normalize(Vector3CrossProduct(right, forward));
}

// ========================
// MOVIMENTAÇÃO LINEAR
// ========================

void Player::PlayerMoveForward(float distance, bool moveInWorldPlane)
{
  Vec3f forward = getPlayerForward();
  if (moveInWorldPlane)
  {
    forward.y = 0;
    forward = Vector3Normalize(forward);
  }
  Vec3f delta = forward * distance;
  position = position + delta;
  target = target + delta;
}

void Player::PlayerMoveRight(float distance, bool moveInWorldPlane)
{
  Vec3f right = getPlayerRight();
  if (moveInWorldPlane)
  {
    right.y = 0;
    right = Vector3Normalize(right);
  }
  Vec3f delta = right * distance;
  position = position + delta;
  target = target + delta;
}

void Player::PlayerMoveUp(float distance)
{
  Vec3f delta = {0.0f, distance, 0.0f};
  position = position + delta;
  target = target + delta;
}

// ========================
// SIMULAÇÃO DE MOVIMENTO
// ========================

Vec3f Player::PlayerPretendingPosition(char key, float distance, bool moveInWorldPlane)
{
  Vec3f pretendPos = position;

  switch (key)
  {
  case 'w':
  {
    Vec3f f = getPlayerForward();
    if (moveInWorldPlane)
    {
      f.y = 0;
      f = Vector3Normalize(f);
    }
    pretendPos = pretendPos + f * distance;
    break;
  }
  case 's':
  {
    Vec3f f = getPlayerForward();
    if (moveInWorldPlane)
    {
      f.y = 0;
      f = Vector3Normalize(f);
    }
    pretendPos = pretendPos - f * distance;
    break;
  }
  case 'a':
  {
    Vec3f r = getPlayerRight();
    if (moveInWorldPlane)
    {
      r.y = 0;
      r = Vector3Normalize(r);
    }
    pretendPos = pretendPos - r * distance;
    break;
  }
  case 'd':
  {
    Vec3f r = getPlayerRight();
    if (moveInWorldPlane)
    {
      r.y = 0;
      r = Vector3Normalize(r);
    }
    pretendPos = pretendPos + r * distance;
    break;
  }
  case 'u': // subir
    pretendPos = pretendPos + Vec3f{0.0f, distance, 0.0f};
    break;
  case 'j': // descer
    pretendPos = pretendPos - Vec3f{0.0f, distance, 0.0f};
    break;
  default:
    break;
  }

  return pretendPos;
}

// ========================
// ROTAÇÃO (FPS)
// ========================

void Player::PlayerRotateYaw(float angle)
{
  // Rotaciona em torno do vetor up
  Vec3f targetOffset = target - position;                           // vetor da posição até o target
  targetOffset = Vector3RotateByAxisAngle(targetOffset, up, angle); // rotaciona
  target = position + targetOffset;                                 // atualiza target
}

void Player::PlayerRotatePitch(float angle)
{
  // Rotaciona em torno do vetor right
  Vec3f forward = target - position;
  Vec3f right = Vector3CrossProduct(forward, up);
  right = Vector3Normalize(right);

  // Limitar pitch para evitar inversão
  const float maxPitch = 89.0f * (PI / 180.0f);
  const float currentPitch = asin(Vector3Normalize(forward).y);
  float clampedAngle = angle;
  if (currentPitch + angle > maxPitch)
    clampedAngle = maxPitch - currentPitch;
  if (currentPitch + angle < -maxPitch)
    clampedAngle = -maxPitch - currentPitch;

  forward = Vector3RotateByAxisAngle(forward, right, clampedAngle);
  target = position + forward;

  // opcional: rotacionar o vetor up também (para câmera livre)
  up = Vector3RotateByAxisAngle(up, right, clampedAngle);
}

void Player::updateTargetFromAngles()
{
  Vec3f forward;
  forward.x = cos(pitch) * sin(yaw);
  forward.y = sin(pitch);
  forward.z = cos(pitch) * cos(yaw);
  target = position + Vector3Normalize(forward);
}

// ========================
// BOUNDING BOX
// ========================

AABB Player::getBounds() const
{
  Vec3f half = {collisionSize / 2.0f, collisionSize / 2.0f, collisionSize / 2.0f};
  return {position - half, position + half};
}