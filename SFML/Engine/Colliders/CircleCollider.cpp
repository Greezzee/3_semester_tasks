#include "CircleCollider.h"

CircleCollider::CircleCollider()
{
	_type = tge::ColliderType::CIRCLE_COLLIDER;
	_pos = { 0, 0 };
	_radius = 0;
}
CircleCollider::CircleCollider(Vector2F pos, float rad)
{
	_type = tge::ColliderType::CIRCLE_COLLIDER;
	_pos = pos;
	_radius = rad;
}

void CircleCollider::Init()
{
	_pos = { 0, 0 };
	_radius = 0;
}
void CircleCollider::Init(Vector2F pos, float rad)
{
	_pos = pos;
	_radius = rad;
}

void CircleCollider::SetRadius(float rad)
{
	_radius = rad;
}
void CircleCollider::SetPos(Vector2F pos)
{
	_pos = pos;
}

float CircleCollider::GetRadius() const
{
	return _radius;
}
Vector2F CircleCollider::GetPos() const
{
	return _pos;
}