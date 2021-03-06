#include "Collider.h"

#include <cmath>

#include "../Utility/Coordinate.h"
#include "../Utility/Line.h"
#include "math.h"
float sqr(float x) {
	return x * x;
}

float sign(float x) {
	return x / fabsf(x);
}

bool Collider::IsCollide(EllipseCollider* a, EllipseCollider* b) {

	float myR = fmaxf(a->_size.x, a->_size.y);
	float thatR = fmaxf(b->_size.x, b->_size.y);

	if (myR * thatR == 0)
		return false;

	float my_angle = a->_angle;// +a->_my_obj->_angle;
	Vector2F my_pos = a->_pos;// +a->_my_obj->_pos;

	float that_angle = b->_angle;// +b->_my_obj->_angle;
	Vector2F that_pos = b->_pos;// +b->_my_obj->_pos;

	Vector2F dif = my_pos - that_pos;

	if (dif.SqrMagnitude() > sqr(myR + thatR))
		return false;

	Vector2F posA, posB, sizeA, sizeB; //A - наибольший, B - наименьший
	float angleA, angleB;

	if (a->_size.x * a->_size.y > b->_size.x * b->_size.y) {
		posA = my_pos;
		posB = that_pos;
		sizeA = a->_size;
		sizeB = b->_size;
		angleA = my_angle;
		angleB = that_angle;
	}
	else {
		posB = my_pos;
		posA = that_pos;
		sizeB = a->_size;
		sizeA = b->_size;
		angleB = my_angle;
		angleA = that_angle;
	}

	//перемещаем центр и угол к B
	angleA -= angleB;
	angleB = 0;
	posA -= posB;
	posB = { 0, 0 };

	//***************************

	Vector2F p1 = posA + Vector2F(cosf(angleA), sinf(angleA)) * sizeA.x;
	Vector2F p2 = posA + Vector2F(-sinf(angleA), cosf(angleA)) * sizeA.y;
	p1 = { p1.x / sizeB.x, p1.y / sizeB.y };
	p2 = { p2.x / sizeB.x, p2.y / sizeB.y };
	posA = { posA.x / sizeB.x, posA.y / sizeB.y };
	p1 -= posA;
	p2 -= posA;


	//Масштабируем так, чтобы B стал единичным кругом

	sizeA = { p1.SqrMagnitude(), p2.SqrMagnitude() }; //Кладём КВАДРАТЫ длин полуосей
	sizeB = { 1, 1 };
	//***********************************************

	//Переводим центр и угол к А
	angleB -= angleA;
	angleA = 0;

	posB -= posA;
	posA = { 0, 0 };

	posB = { posB.x * cosf(angleB) + posB.y * sinf(angleB),
			-posB.x * sinf(angleB) + posB.y * cosf(angleB) };
	//***************************
	float angle_part = 2 * PI / (float)EllipseCollider::COLLIDE_ITERATIONS;
	for (unsigned i = 0; i < EllipseCollider::COLLIDE_ITERATIONS; i++) {
		float x = posB.x + cosf(angle_part * i);
		float y = posB.y + sinf(angle_part * i);

		if (sqr(x) * sizeA.y + sqr(y) * sizeA.x <= sizeA.x * sizeA.y)
			return true;
	}
	if (sqr(posB.x) * sizeA.y + sqr(posB.y) * sizeA.x <= sizeA.x * sizeA.y)
		return true;

	return false;
}

//! Проверяет, пересекаются ли 2 данных коллайдера
bool Collider::IsCollide(SquareCollider* a, SquareCollider* b)
{
	for (unsigned i = 0; i < 2; i++) {
		Vector2F side = a->_points[i] - a->_points[i + 1];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		if (!AxisIntersect(a, b, norm))
			return false;
	}

	for (unsigned i = 0; i < 2; i++) {
		Vector2F side = b->_points[i] - b->_points[i + 1];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		if (!AxisIntersect(a, b, norm))
			return false;
	}
	
	return true;
}

bool Collider::AxisIntersect(SquareCollider* a, SquareCollider* b, const Vector2F& norm)
{
	float min_a, min_b, max_a, max_b;
	min_a = max_a = Vector2F::ScalarMult(a->_points[0], norm);
	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	for (unsigned j = 1; j < 4; j++)
	{
		float len_a = Vector2F::ScalarMult(a->_points[j], norm);
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_a < min_a)
			min_a = len_a;
		if (len_b < min_b)
			min_b = len_b;
		if (len_a > max_a)
			max_a = len_a;
		if (len_b > max_b)
			max_b = len_b;
	}

	return (min_b <= min_a && min_a <= max_b) || (min_b <= max_a && max_a <= max_b);
}

Vector2F Collider::AxisDistance(SquareCollider* a, SquareCollider* b, const Vector2F& norm, const Vector2F& dir)
{
	float k = Vector2F::ScalarMult(dir, norm);
	float min_a, min_b, max_a, max_b;
	min_a = max_a = Vector2F::ScalarMult(a->_points[0], norm);
	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	for (unsigned j = 1; j < 4; j++)
	{
		float len_a = Vector2F::ScalarMult(a->_points[j], norm);
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_a < min_a)
			min_a = len_a;
		if (len_b < min_b)
			min_b = len_b;
		if (len_a > max_a)
			max_a = len_a;
		if (len_b > max_b)
			max_b = len_b;
	}

	Vector2F out;

	if (k == 0 && (max_a < min_b || max_b < min_a))
		return { NAN, NAN };
	if (k == 0)
		return { -INFINITY, INFINITY };

	out.x = (min_b - max_a) / k;
	out.y = (max_b - min_a) / k;

	return { fminf(out.x, out.y) , fmaxf(out.x, out.y) };
}

float Collider::DistanceBetween(SquareCollider* a, SquareCollider* b, const Vector2F& direction)
{
	float out = 0;
	Vector2F possible_distance = { -INFINITY, INFINITY };
	for (unsigned i = 0; i < 2; i++) {
		Vector2F side = a->_points[i] - a->_points[i + 1];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		Vector2F axis_pos_dist = AxisDistance(a, b, norm, direction);

		if (isnan(axis_pos_dist.x) || isnan(axis_pos_dist.y)) {
			return NAN;
		}
		
		possible_distance.x = fmaxf(possible_distance.x, axis_pos_dist.x);
		possible_distance.y = fminf(possible_distance.y, axis_pos_dist.y);
	}

	for (unsigned i = 0; i < 2; i++) {
		Vector2F side = b->_points[i] - b->_points[i + 1];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		Vector2F axis_pos_dist = AxisDistance(a, b, norm, direction);
		if (isnan(axis_pos_dist.x) || isnan(axis_pos_dist.y)) {
			return NAN;
		}
		possible_distance.x = fmaxf(possible_distance.x, axis_pos_dist.x);
		possible_distance.y = fminf(possible_distance.y, axis_pos_dist.y);
	}

	if (possible_distance.x > possible_distance.y) {
		return NAN;
	}

	if (possible_distance.x <= 0 && possible_distance.y >= 0)
		return 0;

	if (possible_distance.x <= 0 && possible_distance.y <= 0)
		return possible_distance.y;

	if (possible_distance.x >= 0 && possible_distance.y >= 0)
		return possible_distance.x;
	return NAN;
}

bool Collider::IsCollide(CircleCollider* a, CircleCollider* b)
{
	return Vector2F(a->_pos - b->_pos).SqrMagnitude() <= sqr(a->_radius + b->_radius);
}

bool Collider::IsCollide(PolygonCollider* a, PolygonCollider* b)
{
	unsigned a_s = a->_points.size();
	unsigned b_s = b->_points.size();
	for (unsigned i = 0; i < a_s; i++) {
		Vector2F side = a->_points[i] - a->_points[(i + 1) % a_s];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		if (!AxisIntersect(a, b, norm))
			return false;
	}

	for (unsigned i = 0; i < b_s; i++) {
		Vector2F side = b->_points[i] - b->_points[(i + 1) % b_s];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		if (!AxisIntersect(a, b, norm))
			return false;
	}

	return true;
}

bool Collider::IsCollide(CircleCollider* a, SquareCollider* b)
{
	/*
	Vector2F norm = a->GetPos() - b->getPos();
	float pos, max_b;
	float magn = norm.Magnitude();
	Vector2F n = norm / magn;
	max_b = Vector2F::ScalarMult(b->_points[0] - b->_pos, norm);
	for (unsigned j = 1; j < 4; j++)
	{
		float len_b = Vector2F::ScalarMult(b->_points[j] - b->_pos, norm);
		if (len_b > max_b)
			max_b = len_b;
	}

	return magn < max_b / magn + a->_radius;
	*/

	float min_a, min_b, max_a, max_b;
	Vector2F norm = Vector2F(a->_pos - b->_pos).Normalized();
	Vector2F circle_point1, circle_point2;
	circle_point1 = a->_pos - norm * a->_radius;
	circle_point2 = a->_pos + norm * a->_radius;

	min_a = Vector2F::ScalarMult(circle_point1, norm);
	max_a = Vector2F::ScalarMult(circle_point2, norm);

	if (min_a > max_a) {
		float buf = min_a;
		min_a = max_a;
		max_a = buf;
	}

	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	for (unsigned j = 1; j < 4; j++)
	{
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_b < min_b)
			min_b = len_b;
		if (len_b > max_b)
			max_b = len_b;
	}

	return (min_b <= min_a && min_a <= max_b) || (min_b <= max_a && max_a <= max_b);
}

bool Collider::IsCollide(SquareCollider* a, CircleCollider* b)
{
	return IsCollide(b, a);
}

bool Collider::IsCollide(UniversalCollider* a, CircleCollider* b)
{
	SquareCollider* x;
	CircleCollider* y;
	switch (a->_type)
	{
	case tge::ColliderType::ERROR_COLLIDER:
		printf("[ERROR]: Error collider type\n");
		return false;
	case tge::ColliderType::SQUARE_COLLIDER:
		x = dynamic_cast<SquareCollider*>(a);
		if (!x) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(x, b);
	case tge::ColliderType::ELLIPSE_COLLIDER:
		printf("[ERROR]: Ellipse and circle can't collide\n");
		return false;
	case tge::ColliderType::POLYGON_COLLIDER:
		printf("[ERROR]: Polygon and circle can't collide\n");
		return false;
	case tge::ColliderType::CIRCLE_COLLIDER:
		y = dynamic_cast<CircleCollider*>(a);
		if (!y) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(y, b);
	default:
		printf("[ERROR]: Incorrect collider\n");
		return false;
		break;
	}
}

bool Collider::IsCollide(UniversalCollider* a, EllipseCollider* b)
{
	EllipseCollider* x;
	switch (a->_type)
	{
	case tge::ColliderType::ERROR_COLLIDER:
		printf("[ERROR]: Error collider type\n");
		return false;
	case tge::ColliderType::SQUARE_COLLIDER:
		printf("[ERROR]: Ellipse and square can't collide\n");
		return false;
	case tge::ColliderType::ELLIPSE_COLLIDER:
		x = dynamic_cast<EllipseCollider*>(a);
		if (!x) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(x, b);
	case tge::ColliderType::POLYGON_COLLIDER:
		printf("[ERROR]: Ellipse and polygon can't collide\n");
		return false;
	case tge::ColliderType::CIRCLE_COLLIDER:
		printf("[ERROR]: Ellipse and circle can't collide\n");
		return false;
	default:
		printf("[ERROR]: Incorrect collider\n");
		return false;
		break;
	}
}

bool Collider::IsCollide(UniversalCollider* a, PolygonCollider* b)
{
	SquareCollider* x;
	PolygonCollider* y;
	PolygonCollider temp;
	switch (a->_type)
	{
	case tge::ColliderType::ERROR_COLLIDER:
		printf("[ERROR]: Error collider type\n");
		return false;
	case tge::ColliderType::SQUARE_COLLIDER:
		x = dynamic_cast<SquareCollider*>(a);
		if (!x) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		temp.Init(*x);
		return IsCollide(&temp, b);
	case tge::ColliderType::ELLIPSE_COLLIDER:
		printf("[ERROR]: Ellipse and polygon can't collide\n");
		return false;
	case tge::ColliderType::POLYGON_COLLIDER:
		y = dynamic_cast<PolygonCollider*>(a);
		if (!y) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(y, b);
	case tge::ColliderType::CIRCLE_COLLIDER:
		printf("[ERROR]: Circle and polygon can't collide\n");
		return false;
	default:
		printf("[ERROR]: Incorrect collider\n");
		return false;
		break;
	}
}

bool Collider::IsCollide(UniversalCollider* a, SquareCollider* b)
{
	SquareCollider* x;
	PolygonCollider* y;
	CircleCollider* z;
	PolygonCollider temp;
	switch (a->_type)
	{
	case tge::ColliderType::ERROR_COLLIDER:
		printf("[ERROR]: Error collider type\n");
		return false;
	case tge::ColliderType::SQUARE_COLLIDER:
		x = dynamic_cast<SquareCollider*>(a);
		if (!x) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(x, b);
	case tge::ColliderType::ELLIPSE_COLLIDER:
		printf("[ERROR]: Ellipse and square can't collide\n");
		return false;
	case tge::ColliderType::POLYGON_COLLIDER:
		y = dynamic_cast<PolygonCollider*>(a);
		if (!y) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		temp.Init(*b);
		return IsCollide(y, &temp);
	case tge::ColliderType::CIRCLE_COLLIDER:
		z = dynamic_cast<CircleCollider*>(a);
		if (!z) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(z, b);
	default:
		printf("[ERROR]: Incorrect collider\n");
		return false;
		break;
	}
}

bool Collider::IsCollide(UniversalCollider* a, UniversalCollider* b)
{
	SquareCollider* x;
	EllipseCollider* y;
	PolygonCollider* z;
	CircleCollider* w;
	switch (b->_type)
	{
	case tge::ColliderType::ERROR_COLLIDER:
		printf("[ERROR]: Error collider type\n");
		return false;
	case tge::ColliderType::SQUARE_COLLIDER:
		x = dynamic_cast<SquareCollider*>(b);
		if (!x) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(a, x);
	case tge::ColliderType::ELLIPSE_COLLIDER:
		y = dynamic_cast<EllipseCollider*>(b);
		if (!y) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(a, y);
	case tge::ColliderType::POLYGON_COLLIDER:
		z = dynamic_cast<PolygonCollider*>(b);
		if (!z) {
			printf("[ERROR]: Incorrect collider");
			return false;
		}
		return IsCollide(a, z);
	case tge::ColliderType::CIRCLE_COLLIDER:
		w = dynamic_cast<CircleCollider*>(b);
		if (!w) {
			printf("[ERROR]: Incorrect collider\n");
			return false;
		}
		return IsCollide(a, w);
	default:
		printf("[ERROR]: Incorrect collider\n");
		return false;
		break;
	}
}

float Collider::DistanceBetween(CircleCollider* a, SquareCollider* b, const Vector2F& dir)
{
	float min_a, min_b, max_a, max_b;
	Vector2F norm = Vector2F(a->_pos - b->_pos).Normalized();
	Vector2F circle_point1, circle_point2;
	circle_point1 = a->_pos - norm * a->_radius;
	circle_point2 = a->_pos + norm * a->_radius;

	min_a = Vector2F::ScalarMult(circle_point1, norm);
	max_a = Vector2F::ScalarMult(circle_point2, norm);

	if (min_a > max_a) {
		float buf = min_a;
		min_a = max_a;
		max_a = buf;
	}

	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	for (unsigned j = 1; j < 4; j++)
	{
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_b < min_b)
			min_b = len_b;
		if (len_b > max_b)
			max_b = len_b;
	}

	return (min_b <= min_a && min_a <= max_b) || (min_b <= max_a && max_a <= max_b);
}

float Collider::DistanceBetween(CircleCollider* a, CircleCollider* b, const Vector2F& direction)
{
	if (IsCollide(a, b))
		return 0;
	Vector2F pos_a = {0, 0};
	Vector2F pos_b = b->_pos - a->_pos;

	float dir_angle = atan2f(direction.y, direction.x);
	float cos_dir = cosf(dir_angle);
	float sin_dir = sinf(dir_angle);
	pos_b = { pos_b.x * cos_dir + pos_b.y * sin_dir, -pos_b.x * sin_dir + pos_b.y * cos_dir };

	if (fabsf(pos_b.y) > a->_radius + b->_radius)
		return NAN;

	return sign(pos_b.x) * (fabsf(pos_b.x) - sqrtf(sqr(a->_radius + b->_radius) - sqr(pos_b.y)));
}

float Collider::DistanceBetween(PolygonCollider* a, PolygonCollider* b, const Vector2F& direction)
{
	unsigned a_s = a->_points.size();
	unsigned b_s = b->_points.size();
	float out = 0;
	Vector2F possible_distance = { -INFINITY, INFINITY };
	for (unsigned i = 0; i < a_s; i++) {
		Vector2F side = a->_points[i] - a->_points[(i + 1) % a_s];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		Vector2F axis_pos_dist = AxisDistance(a, b, norm, direction);

		if (isnan(axis_pos_dist.x) || isnan(axis_pos_dist.y))
			return NAN;

		possible_distance.x = fmaxf(possible_distance.x, axis_pos_dist.x);
		possible_distance.y = fminf(possible_distance.y, axis_pos_dist.y);
	}

	for (unsigned i = 0; i < b_s; i++) {
		Vector2F side = b->_points[i] - b->_points[(i + 1) % b_s];
		Vector2F norm;
		if (fabsf(side.y) < EPS)
			norm = { 0, 1 };
		else
			norm = { 1, -side.x / side.y };

		Vector2F axis_pos_dist = AxisDistance(a, b, norm, direction);
		if (isnan(axis_pos_dist.x) || isnan(axis_pos_dist.y))
			return NAN;
		possible_distance.x = fmaxf(possible_distance.x, axis_pos_dist.x);
		possible_distance.y = fminf(possible_distance.y, axis_pos_dist.y);
	}

	if (possible_distance.x > possible_distance.y)
		return NAN;

	if (possible_distance.x <= 0 && possible_distance.y >= 0)
		return 0;

	if (possible_distance.x <= 0 && possible_distance.y <= 0)
		return possible_distance.y;

	if (possible_distance.x >= 0 && possible_distance.y >= 0)
		return possible_distance.x;

	return NAN;
}

//! Проверяет пересечение двух коллайдеров по оси norm
bool Collider::AxisIntersect(PolygonCollider* a, PolygonCollider* b, const Vector2F& norm)
{
	float min_a, min_b, max_a, max_b;
	min_a = max_a = Vector2F::ScalarMult(a->_points[0], norm);
	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	unsigned a_s = a->_points.size();
	unsigned b_s = b->_points.size();
	for (unsigned j = 1; j < a_s; j++)
	{
		float len_a = Vector2F::ScalarMult(a->_points[j], norm);
		if (len_a < min_a)
			min_a = len_a;
		if (len_a > max_a)
			max_a = len_a;
	}

	for (unsigned j = 1; j < b_s; j++)
	{
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_b < min_b)
			min_b = len_b;
		if (len_b > max_b)
			max_b = len_b;
	}

	return (min_b <= min_a && min_a <= max_b) || (min_b <= max_a && max_a <= max_b);
}

//! Лучше не спрашивайте... И НИКОГДА НЕ ТРОГАЙТЕ
Vector2F Collider::AxisDistance(PolygonCollider* a, PolygonCollider* b, const Vector2F& norm, const Vector2F& dir)
{
	float k = Vector2F::ScalarMult(dir, norm);
	float min_a, min_b, max_a, max_b;
	min_a = max_a = Vector2F::ScalarMult(a->_points[0], norm);
	min_b = max_b = Vector2F::ScalarMult(b->_points[0], norm);
	unsigned a_s = a->_points.size();
	unsigned b_s = b->_points.size();
	for (unsigned j = 1; j < a_s; j++)
	{
		float len_a = Vector2F::ScalarMult(a->_points[j], norm);
		if (len_a < min_a)
			min_a = len_a;
		if (len_a > max_a)
			max_a = len_a;
	}

	for (unsigned j = 1; j < b_s; j++)
	{
		float len_b = Vector2F::ScalarMult(b->_points[j], norm);
		if (len_b < min_b)
			min_b = len_b;
		if (len_b > max_b)
			max_b = len_b;
	}

	Vector2F out;

	if (k == 0 && (max_a < min_b || max_b < min_a))
		return { NAN, NAN };
	if (k == 0)
		return { -INFINITY, INFINITY };

	out.x = (min_b - max_a) / k;
	out.y = (max_b - min_a) / k;

	return { fminf(out.x, out.y) , fmaxf(out.x, out.y) };
}