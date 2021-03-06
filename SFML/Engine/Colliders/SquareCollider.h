#pragma once
#include "../Utility/Coordinate.h"
#include "../Gameplay/GameObject.h"
#include "UniversalCollider.h"
const float PI = 3.1415926f;

class SquareCollider : public UniversalCollider {
public:
	SquareCollider();

	//! Задаёт нулевой коллайдер, никому не принадлежащий
	void Init();

	//! Задаёт прямоугольный коллайдер
	//! Размер сторон size, положение в пространстве pos
	//! Прямоугольник со сторонами, паралелльными осям координат
	//! ЦЕНТР - ВСЕГДА ТОЧКА ПЕРЕСЕЧЕНИЯ ДИАГОНАЛЕЙ
	void Init(const Vector2F& pos, const Vector2F& size);

	//! Задаёт прямоугольный коллайдер
	//! Размер сторон size, положение в пространстве pos
	//! Прямоугольник. Нижняя сторона наклонена под углом angle (В РАДИАНАХ) к оси x, против часовой
	//! ЦЕНТР (относительно центра происходит определение координаты и вращение) - ВСЕГДА ТОЧКА ПЕРЕСЕЧЕНИЯ ДИАГОНАЛЕЙ
	void Init(const Vector2F& pos, const Vector2F& size,
		float angle);

	//! Задаёт прямоугольный коллайдер
	//! Размер сторон size, положение в пространстве pos
	//! Параллелепипед. Нижний левый угол (до прочих поворотов) - shape_angle (в радианах). shape_angle = PI/2 - задаёт прямоугольник
	//! Нижняя сторона наклонена под углом angle (В РАДИАНАХ) к оси x, против часовой
	//! ЦЕНТР (относительно центра происходит определение координаты и вращение) - ВСЕГДА ТОЧКА ПЕРЕСЕЧЕНИЯ ДИАГОНАЛЕЙ
	void Init(const Vector2F& pos, const Vector2F& size,
		float angle, float shape_angle);

	float GetAngle() const;
	void SetAngle(float angle);
	Vector2F GetPos() const;
	void SetPos(Vector2F pos);
	Vector2F GetSize() const;
	void SetSize(Vector2F size);
	Vector2F GetPoint(unsigned i) const;

	virtual ~SquareCollider() {};
protected:

	void SetPoints(); //! Расчёт и установка краевых точек коллайдера

	Vector2F _size; //! ПОЛОВИНЫ Длин сторон
	Vector2F _pos; //! Позиция центра в пространстве (центр - точка пересечения диагоналей)
	float _angle; //! Угол поворота коллайдера в РАДИАНАХ против часовой. При _angle = 0 нижняя грань параллельна оси x
	float _shape_angle; //! Нижний левый угол коллайдера в РАДИАНАХ. По умолчанию pi/2 (прямоугольник)

	Vector2F _points[4]; //! Координаты правой/нижней, правой/верхней, левой/верхней, левой/нижней точек в данном порядке

	friend class Collider;
};

