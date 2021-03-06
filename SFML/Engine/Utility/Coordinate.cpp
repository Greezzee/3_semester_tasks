#include "Coordinate.h"
#include <cmath>
float Vector2F::ScalarMult(const Vector2F& a, const Vector2F& b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2F::Vector2F()
{
	x = 0;
	y = 0;
}

Vector2F::Vector2F(float a, float b)
{
	x = a;
	y = b;
}

Vector2F::Vector2F(const Vector2F& that)
{
	x = that.x;
	y = that.y;
}

float Vector2F::SqrMagnitude() const
{
	return x * x + y * y;
}

float Vector2F::Magnitude() const
{
	return sqrtf(SqrMagnitude());
}

const Vector2F operator/(const Vector2F& a, float b)
{
	return Vector2F(a.x / b, a.y / b);
}

Vector2F Vector2F::Normalized() const
{
	float len = Magnitude();
	if (len == 0)
		return *this;
	return *this / len;
}

const bool Vector2F::operator<(const Vector2F& that)
{
	return x < that.x - EPS || abs(x - that.x) < EPS && y < that.y - EPS;
}

const Vector2F& Vector2F::operator=(const Vector2F& that)
{
	x = that.x;
	y = that.y;
	return *this;
}

const Vector2F& operator+(const Vector2F& that)
{
	return that;
}
const Vector2F operator-(const Vector2F& that)
{
	return Vector2F(-that.x, -that.y);
}

const Vector2F& Vector2F::operator+=(const Vector2F& that)
{
	x += that.x;
	y += that.y;
	return *this;
}

const Vector2F& Vector2F::operator-=(const Vector2F& that)
{
	x -= that.x;
	y -= that.y;
	return *this;
}

const Vector2F operator+(const Vector2F& a, const Vector2F& b)
{
	return Vector2F(a.x + b.x, a.y + b.y);
}

const Vector2F operator*(const Vector2F& a, const Vector2F& b)
{
	return Vector2F(a.x * b.x, a.y * b.y);
}

const Vector2F operator/(const Vector2F& a, const Vector2F& b)
{
	return Vector2F(a.x / b.x, a.y / b.y);
}

const Vector2F operator-(const Vector2F& a, const Vector2F& b)
{
	return Vector2F(a.x - b.x, a.y - b.y);
}

const Vector2F operator*(float a, const Vector2F& b)
{
	return Vector2F(a * b.x, a * b.y);
}

const Vector2F operator*(const Vector2F& a, float b)
{
	return Vector2F(a.x * b, a.y * b);
}

const bool operator==(const Vector2F& a, const Vector2F& b)
{
	return a.x == b.x && a.y == b.y;
}

const bool operator!=(const Vector2F& a, const Vector2F& b)
{
	return !(a == b);
}

Vector2D::Vector2D()
{
	x = 0;
	y = 0;
}

Vector2D::Vector2D(double a, double b)
{
	x = a;
	y = b;
}

Vector2D::Vector2D(const Vector2D& that)
{
	x = that.x;
	y = that.y;
}

double Vector2D::SqrMagnitude() const
{
	return x * x + y * y;
}

double Vector2D::Magnitude() const
{
	return sqrt(SqrMagnitude());
}

const Vector2D operator/(Vector2D& a, double b)
{
	return Vector2D(a.x / b, a.y / b);
}


Vector2D Vector2D::Normalized()
{
	double len = Magnitude();
	if (len == 0)
		return *this;
	return *this / len;
}
const bool Vector2D::operator<(const Vector2D& that)
{
	return x < that.x - EPS || abs(x - that.x) < EPS && y < that.y - EPS;
}

const Vector2D& Vector2D::operator=(const Vector2D& that)
{
	x = that.x;
	y = that.y;
	return *this;
}

const Vector2D& operator+(const Vector2D& that)
{
	return that;
}
const Vector2D operator-(const Vector2D& that)
{
	return Vector2D(-that.x, -that.y);
}

const Vector2D& Vector2D::operator+=(const Vector2D& that)
{
	x += that.x;
	y += that.y;
	return *this;
}

const Vector2D& Vector2D::operator-=(const Vector2D& that)
{
	x -= that.x;
	y -= that.y;
	return *this;
}

const Vector2D operator+(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.x + b.x, a.y + b.y);
}

const Vector2D operator*(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.x * b.x, a.y * b.y);
}

const Vector2D operator/(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.x / b.x, a.y / b.y);
}

const Vector2D operator-(const Vector2D& a, const Vector2D& b)
{
	return Vector2D(a.x - b.x, a.y - b.y);
}

const Vector2D operator*(double a, const Vector2D& b)
{
	return Vector2D(a * b.x, a * b.y);
}

const Vector2D operator*(const Vector2D& a, double b)
{
	return Vector2D(a.x * b, a.y * b);
}

const bool operator==(const Vector2D& a, const Vector2D& b)
{
	return a.x == b.x && a.y == b.y;
}

const bool operator!=(const Vector2D& a, const Vector2D& b)
{
	return !(a == b);
}

Vector2I::Vector2I()
{
	x = 0;
	y = 0;
}

Vector2I::Vector2I(int a, int b)
{
	x = a;
	y = b;
}

Vector2I::Vector2I(const Vector2I& that)
{
	x = that.x;
	y = that.y;
}

const Vector2I operator/(Vector2I& a, int b)
{
	return Vector2I(a.x / b, a.y / b);
}

const Vector2I& Vector2I::operator=(const Vector2I& that)
{
	x = that.x;
	y = that.y;
	return *this;
}

const Vector2I& operator+(const Vector2I& that)
{
	return that;
}
const Vector2I operator-(const Vector2I& that)
{
	return Vector2I(-that.x, -that.y);
}

const Vector2I& Vector2I::operator+=(const Vector2I& that)
{
	x += that.x;
	y += that.y;
	return *this;
}

const Vector2I& Vector2I::operator-=(const Vector2I& that)
{
	x -= that.x;
	y -= that.y;
	return *this;
}

const Vector2I operator+(const Vector2I& a, const Vector2I& b)
{
	return Vector2I(a.x + b.x, a.y + b.y);
}

const Vector2I operator*(const Vector2I& a, const Vector2I& b)
{
	return Vector2I(a.x * b.x, a.y * b.y);
}

const Vector2I operator/(const Vector2I& a, const Vector2I& b)
{
	return Vector2I(a.x / b.x, a.y / b.y);
}

const Vector2I operator-(const Vector2I& a, const Vector2I& b)
{
	return Vector2I(a.x - b.x, a.y - b.y);
}

const Vector2I operator*(int a, const Vector2I& b)
{
	return Vector2I(a * b.x, a * b.y);
}

const Vector2I operator*(const Vector2I& a, int b)
{
	return Vector2I(a.x * b, a.y * b);
}

const bool operator==(const Vector2I& a, const Vector2I& b)
{
	return a.x == b.x && a.y == b.y;
}

const bool operator!=(const Vector2I& a, const Vector2I& b)
{
	return !(a == b);
}

Vector2U::Vector2U()
{
	x = 0;
	y = 0;
}

Vector2U::Vector2U(unsigned a, unsigned b)
{
	x = a;
	y = b;
}

Vector2U::Vector2U(const Vector2U& that)
{
	x = that.x;
	y = that.y;
}

const Vector2U operator/(Vector2U& a, unsigned b)
{
	return Vector2U(a.x / b, a.y / b);
}

const Vector2U& Vector2U::operator=(const Vector2U& that)
{
	x = that.x;
	y = that.y;
	return *this;
}

const Vector2U& operator+(const Vector2U& that)
{
	return that;
}
const Vector2U operator-(const Vector2U& that)
{
	return Vector2U(that.x, that.y);
}

const Vector2U& Vector2U::operator+=(const Vector2U& that)
{
	x += that.x;
	y += that.y;
	return *this;
}

const Vector2U& Vector2U::operator-=(const Vector2U& that)
{
	x -= that.x;
	y -= that.y;
	return *this;
}

const Vector2U operator+(const Vector2U& a, const Vector2U& b)
{
	return Vector2U(a.x + b.x, a.y + b.y);
}

const Vector2U operator*(const Vector2U& a, const Vector2U& b)
{
	return Vector2U(a.x * b.x, a.y * b.y);
}

const Vector2U operator/(const Vector2U& a, const Vector2U& b)
{
	return Vector2U(a.x / b.x, a.y / b.y);
}

const Vector2U operator-(const Vector2U& a, const Vector2U& b)
{
	return Vector2U(a.x - b.x, a.y - b.y);
}

const Vector2U operator*(unsigned a, const Vector2U& b)
{
	return Vector2U(a * b.x, a * b.y);
}

const Vector2U operator*(const Vector2U& a, unsigned b)
{
	return Vector2U(a.x * b, a.y * b);
}

const bool operator==(const Vector2U& a, const Vector2U& b)
{
	return a.x == b.x && a.y == b.y;
}

const bool operator!=(const Vector2U& a, const Vector2U& b)
{
	return !(a == b);
}

Vector2F Vector2I::to2F() const {
	return Vector2F((float)this->x, (float)this->y);
}
