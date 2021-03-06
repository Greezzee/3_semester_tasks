#include "../Colliders/EllipseCollider.h"
#include <cmath>
const float PI = 3.1415926f;

const unsigned EllipseCollider::COLLIDE_ITERATIONS = 16;

EllipseCollider::EllipseCollider()
{
	_type = tge::ColliderType::ELLIPSE_COLLIDER;
}

void EllipseCollider::Init()
{
	_pos = _size = { 0, 0 };
	_angle = 0;
}

void EllipseCollider::Init(Vector2F pos, float angle, Vector2F size)
{
	_pos = pos;
	_size = size;
	_angle = angle;
}

Vector2F EllipseCollider::GetPos() const
{
	return _pos;
}
Vector2F EllipseCollider::GetSize() const
{
	return _size;
}
float EllipseCollider::GetAngle() const
{
	return _angle;
}

void EllipseCollider::SetPos(const Vector2F& pos)
{
	_pos = pos;
}
void EllipseCollider::SetSize(const Vector2F& size)
{
	_size = size;
}
void EllipseCollider::SetAngle(float angle)
{
	_angle = angle;
}