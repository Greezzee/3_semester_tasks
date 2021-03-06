#include "PolygonCollider.h"
#include <cmath>
PolygonCollider::PolygonCollider()
{
	Init();
	_type = tge::ColliderType::POLYGON_COLLIDER;
}
PolygonCollider::PolygonCollider(const SquareCollider& that)
{
	Init(that);
	_type = tge::ColliderType::POLYGON_COLLIDER;
}

void PolygonCollider::Init()
{
	_pos = { 0, 0 };
	_angle = 0;
	_points.resize(0);
}

void PolygonCollider::Init(const SquareCollider& that)
{
	_pos = that.GetPos();
	_angle = that.GetAngle();
	_points.resize(4);
	for (unsigned i = 0; i < 4; i++)
		_points[i] = that.GetPoint(i);
}

void PolygonCollider::Init(const Vector2F& pos, std::vector<Vector2F>& points)
{
	_pos = pos;
	_angle = 0;
	_points = points;
	for (unsigned i = 0; i < _points.size(); i++)
		_points[i] += _pos;
}


void PolygonCollider::Init(const Vector2F& pos, std::vector<Vector2F>& points, float angle)
{
	_pos = pos;
	_angle = angle;
	_points = points;
	for (unsigned i = 0; i < _points.size(); i++)
		_points[i] += _pos;

	for (unsigned i = 0; i < _points.size(); i++) {
		auto p = _points[i];
		p = { p.x * cosf(_angle) + p.y * sinf(_angle), -p.x * sinf(_angle) + p.y * cosf(_angle) };
	}
}

float PolygonCollider::GetAngle() const
{
	return _angle;
}
void PolygonCollider::SetAngle(float angle)
{
	float a = angle - _angle;
	if (fabsf(a) < EPS)
		return;
	for (unsigned i = 0; i < _points.size(); i++) {
		auto p = _points[i] - _pos;
		p = { p.x * cosf(a) - p.y * sinf(a), p.x * sinf(a) + p.y * cosf(a) };
		_points[i] = p + _pos;
	}
	_angle = angle;
}

Vector2F PolygonCollider::GetPos() const
{
	return _pos;
}

void PolygonCollider::SetPos(Vector2F pos)
{
	auto p = pos - _pos;
	for (unsigned i = 0; i < _points.size(); i++)
		_points[i] += p;
	_pos = pos;
}
unsigned PolygonCollider::GetPointsCount() const
{
	return _points.size();
}
Vector2F PolygonCollider::GetPoint(unsigned i) const
{
	return _points[i];
}