#pragma once

#include "../Utility/Coordinate.h"
#include "../Gameplay/GameObject.h"
#include "SquareCollider.h"
#include "UniversalCollider.h"
#include <vector>

class PolygonCollider : public UniversalCollider 
{
public:

	PolygonCollider();
	PolygonCollider(const SquareCollider& that);

	//! Задаёт нулевой коллайдер
	void Init();

	//! Задаёт многоугольный коллайдер - копию данного прямоугольного
	void Init(const SquareCollider& that);

	//! Задаёт многоугольный коллайдер
	//! points - массив вершин коллайдера относительно точки (0, 0)
	//! pos - положение центра в пространстве
	void Init(const Vector2F& pos, std::vector<Vector2F>& points);

	//! Задаёт многоугольный коллайдер
	//! points - массив вершин коллайдера относительно точки (0, 0)
	//! pos - положение центра в пространстве
	//! angle - поворот против часовой коллайдера относительно центра
	void Init(const Vector2F& pos, std::vector<Vector2F>& points, float angle);

	float GetAngle() const;
	void SetAngle(float angle);
	Vector2F GetPos() const;
	void SetPos(Vector2F pos);
	unsigned GetPointsCount() const;
	Vector2F GetPoint(unsigned i) const;

	virtual ~PolygonCollider() {};

protected:

	Vector2F _pos; //! Позиция центра в пространстве (центр - точка пересечения диагоналей)
	float _angle; //! Угол поворота коллайдера в РАДИАНАХ против часовой. При _angle = 0 нижняя грань параллельна оси x

	std::vector<Vector2F> _points; //! Координаты правой/нижней, правой/верхней, левой/верхней, левой/нижней точек в данном порядке

	friend class Collider;
};
