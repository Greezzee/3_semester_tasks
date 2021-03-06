#pragma once
#include "EllipseCollider.h"
#include "SquareCollider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"

class Collider
{
public:
	/*!
	* Получает указатели на 2 эллиптических коллайдера
	* Вернёт false, если данные эллипсы не пересекаются
	* Вернёт true, еслии пересекаются
	*
	* Функция НЕ 100% ТОЧНАЯ, использует приближения. Степени точности определяет COLLIDE_ITERATIONS
	*/
	static bool IsCollide(EllipseCollider* a, EllipseCollider* b);

	//! Проверяет, пересекаются ли 2 данных коллайдера
	static bool IsCollide(SquareCollider* a, SquareCollider* b);

	//! Проверяют, пересекаются ли 2 данных круга
	static bool IsCollide(CircleCollider* a, CircleCollider* b);

	//! Проверяет, пересекаются ли данный круг и данный прямоугольник
	static bool IsCollide(CircleCollider* a, SquareCollider* b);

	//! Проверяет, пересекаются ли данный круг и данный прямоугольник
	static bool IsCollide(SquareCollider* a, CircleCollider* b);

	//! Проверяет, пересекаются ли два данных многоугольника
	static bool IsCollide(PolygonCollider* a, PolygonCollider* b);

	//! Проверяет, пересекается ли данный произвольный соллайдер с данным кругом
	static bool IsCollide(UniversalCollider* a, CircleCollider* b);

	//! Проверяет, пересекается ли данный произвольный соллайдер с данным эллипсом
	static bool IsCollide(UniversalCollider* a, EllipseCollider* b);

	//! Проверяет, пересекается ли данный произвольный соллайдер с данным многоугольником
	static bool IsCollide(UniversalCollider* a, PolygonCollider* b);

	//! Проверяет, пересекается ли данный произвольный соллайдер с данным прямоугольником
	static bool IsCollide(UniversalCollider* a, SquareCollider* b);

	//! Проверяет, пересекаются ли два данных произвольных коллайдера
	//! Если произвольные коллайдеры на самом деле являются несовместимой парой, выведет ошибку и вернёт false
	//! Несовместимые пары на данный момент:
	//! EllipseCollider с любыми другими
	//! CircleCollider с PolygonCollider и с EllipseCollider
	//! SquareCollider с EllipseCollider
	//! PolygonCollider с EllipseCollider и CircleCollider
	static bool IsCollide(UniversalCollider* a, UniversalCollider* b);

	//! Выпускает из центра коллайдера a вектор в направлении direction
	//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
	//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
	//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
	static float DistanceBetween(SquareCollider* a, SquareCollider* b, const Vector2F& direction);

	//! Выпускает из центра коллайдера a вектор в направлении direction
	//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
	//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
	//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
	//! НЕ РАБОТАЕТ
	static float DistanceBetween(CircleCollider* a, SquareCollider* b, const Vector2F& direction);

	//! Выпускает из центра коллайдера a вектор в направлении direction
	//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
	//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
	//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
	static float DistanceBetween(CircleCollider* a, CircleCollider* b, const Vector2F& direction);

	//! Выпускает из центра коллайдера a вектор в направлении direction
	//! Определяет, сколько должен пройти коллайдер a в направлении данного вектора, чтобы столкнуться в b
	//! Вернёт искомое расстояние в тех же единицах, в которых заданы коллайдеры
	//! Вернёт NAN, если при движении в данном направлении, a никогда не встретит b
	static float DistanceBetween(PolygonCollider* a, PolygonCollider* b, const Vector2F& direction);

	static float DistanceBetween(UniversalCollider* a, UniversalCollider* b, const Vector2F& direction);

private:

	//! Проверяет пересечение двух коллайдеров по оси norm
	static bool AxisIntersect(SquareCollider* a, SquareCollider* b, const Vector2F& norm);

	//! Лучше не спрашивайте... И НИКОГДА НЕ ТРОГАЙТЕ
	static Vector2F AxisDistance(SquareCollider* a, SquareCollider* b, const Vector2F& norm, const Vector2F& dir);

	//! Проверяет пересечение двух коллайдеров по оси norm
	static bool AxisIntersect(PolygonCollider* a, PolygonCollider* b, const Vector2F& norm);

	//! Лучше не спрашивайте... И НИКОГДА НЕ ТРОГАЙТЕ
	static Vector2F AxisDistance(PolygonCollider* a, PolygonCollider* b, const Vector2F& norm, const Vector2F& dir);
};
