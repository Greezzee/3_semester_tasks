#pragma once

namespace tge
{
	enum class ColliderType
	{
		ERROR_COLLIDER,
		SQUARE_COLLIDER,
		ELLIPSE_COLLIDER,
		POLYGON_COLLIDER,
		CIRCLE_COLLIDER
	};
}

class UniversalCollider
{
public:
	UniversalCollider() :
		_type(tge::ColliderType::ERROR_COLLIDER) {}

	virtual ~UniversalCollider() {}
protected:
	tge::ColliderType _type;

	friend class Collider;
};