#include "Line.h"
#include <cmath>
Line::Line()
{
	a = b = c = 0;
}

Line::Line(const Vector2F& p, const Vector2F& q)
{
	a = p.y - q.y;
	b = q.x - p.x;
	c = -a * p.x - b * p.y;
	Normalize();
}

void Line::Normalize()
{
	float z = sqrtf(a * a + b * b);
	if (abs(z) > EPS)
		a /= z, b /= z, c /= z;
}

float Line::Distance(const Vector2F& p)
{
	return a * p.x + b * p.y + c;
}

float Determinant(float a, float b, float c, float d)
{
	return a * d - b * c;
}

inline bool Between(float l, float r, float x)
{
	return fmin(l, r) <= x + EPS && x <= fmax(l, r) + EPS;
}

inline void swap(float& a, float& b)
{
	float c = b;
	b = a;
	a = c;
}

inline void swap(Vector2F& a, Vector2F& b)
{
	Vector2F c = b;
	b = a;
	a = c;
}

inline bool Intersect1D(float a, float b, float c, float d) {
	if (a > b)  swap(a, b);
	if (c > d)  swap(c, d);
	return fmax(a, c) <= fmin(b, d) + EPS;
}

bool Intersect(Vector2F a, Vector2F b, Vector2F c, Vector2F d) {
	if (!Intersect1D(a.x, b.x, c.x, d.x) || !Intersect1D(a.y, b.y, c.y, d.y))
		return false;
	Line m(a, b);
	Line n(c, d);
	float zn = Determinant(m.a, m.b, n.a, n.b);
	Vector2F left, right;
	if (abs(zn) < EPS) {
		if (abs(m.Distance(c)) > EPS || abs(n.Distance(a)) > EPS)
			return false;
		if (b < a)  swap(a, b);
		if (d < c)  swap(c, d);
		left = a < c ? c : a;
		right = b < d ? b : d;
		return true;
	}
	else {
		left.x = right.x = -Determinant(m.c, m.b, n.c, n.b) / zn;
		left.y = right.y = -Determinant(m.a, m.c, n.a, n.c) / zn;
		return Between(a.x, b.x, left.x)
			&& Between(a.y, b.y, left.y)
			&& Between(c.x, d.x, left.x)
			&& Between(c.y, d.y, left.y);
	}
}