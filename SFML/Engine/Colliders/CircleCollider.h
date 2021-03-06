#pragma once

#include "../Utility/Coordinate.h"
#include "UniversalCollider.h"

class CircleCollider : public UniversalCollider
{
public:
	CircleCollider();
	virtual ~CircleCollider() {};
	CircleCollider(Vector2F pos, float rad);

	void Init();
	void Init(Vector2F pos, float rad);

	void SetRadius(float rad);
	void SetPos(Vector2F pos);

	float GetRadius() const;
	Vector2F GetPos() const;

private:
	Vector2F _pos;
	float _radius;

	friend class Collider;
};
