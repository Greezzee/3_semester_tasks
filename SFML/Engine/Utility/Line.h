#pragma once
#include "Coordinate.h"

/*!
* Структура, задающая прямую линию на плоскости с некоторыми функциями
*/
struct Line
{
	float a, b, c; //! Коэффициенты прямой ax + by + c = 0
	Line();
	Line(const Vector2F& p, const Vector2F& q); //! Конструктор задаёт прямую, проходящую через 2 данные точки

	void Normalize(); //! приводит коэффициенты прямой к виду, что sqrt(a^2 + b^2) = 1

	float Distance(const Vector2F& p); //! Найти нименьшее расстояния от точки до данной прямой
};

/*!
* Проверяет, пересакаются ли 2 отрезка.
* Первый отрезок соединяет точки a и b, второй - точки c и d
*/
bool Intersect(Vector2F a, Vector2F b, Vector2F c, Vector2F d);

/*!
* Проверяет, находится ли число x между числами l и r
*/
inline bool Between(float l, float r, float x);
