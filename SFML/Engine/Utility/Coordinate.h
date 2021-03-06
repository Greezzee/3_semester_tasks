#pragma once
const double EPS = 1E-9;

/*!
* Полезные структуры: двумерные векторы различных типов.
*
* Конструкторы и операции определены только для float, но в скором времени добавлю и остальные.
*
* Введён вместо встроенные в SFML векторов, чтобы избежать появление зависимого от sfml кода там, где он не необходим
*/
struct Vector2F
{
	//! Конструктор по умолчанию, вектор (0, 0)
	Vector2F();

	//! Конструктор вектора (a, b)
	Vector2F(float a, float b);

	//! Конструктор копирования
	Vector2F(const Vector2F& that);
	float x, y;

	//! Вернёт длину вектора
	float Magnitude() const;

	//! Вернёт квадрат длины вектора. БЫСТРЕЕ, ЧЕМ ОБЫЧНАЯ ДЛИНА
	float SqrMagnitude() const;

	//! Вернёт единичный вектор, сонаправленный с данным
	Vector2F Normalized() const;

	//! Операции сложения векторов и умножения на число совпадают с классическими векторными операциями
	/*!
	* Введены операции умножения и деления вектора на вектор
	* (a, b) * (c, d) = (a * c, b * d)
	* (a, b) / (c, d) = (a / c, b / d)
	*/
	const Vector2F& operator=(const Vector2F& that);
	const Vector2F& operator+=(const Vector2F& that);
	const Vector2F& operator-=(const Vector2F& that);

	const bool operator< (const Vector2F& that);

	//! Вернёт скалярное произведение векторов
	static float ScalarMult(const Vector2F& a, const Vector2F& b);
};

struct Vector2U
{
	//! Конструктор по умолчанию, вектор (0, 0)
	Vector2U();

	//! Конструктор вектора (a, b)
	Vector2U(unsigned a, unsigned b);

	//! Конструктор копирования
	Vector2U(const Vector2U& that);
	unsigned x, y;

	//! Операции сложения векторов и умножения на число совпадают с классическими векторными операциями
	/*!
	* Введены операции умножения и деления вектора на вектор
	* (a, b) * (c, d) = (a * c, b * d)
	* (a, b) / (c, d) = (a / c, b / d)
	*/
	const Vector2U& operator=(const Vector2U& that);
	const Vector2U& operator+=(const Vector2U& that);
	const Vector2U& operator-=(const Vector2U& that);
};

struct Vector2D
{
	//! Конструктор по умолчанию, вектор (0, 0)
	Vector2D();

	//! Конструктор вектора (a, b)
	Vector2D(double a, double b);

	//! Конструктор копирования
	Vector2D(const Vector2D& that);
	double x, y;

	//! Вернёт длину вектора
	double Magnitude() const;

	//! Вернёт квадрат длины вектора. БЫСТРЕЕ, ЧЕМ ОБЫЧНАЯ ДЛИНА
	double SqrMagnitude() const;

	//! Вернёт единичный вектор, сонаправленный с данным
	Vector2D Normalized();

	//! Операции сложения векторов и умножения на число совпадают с классическими векторными операциями
	/*!
	* Введены операции умножения и деления вектора на вектор
	* (a, b) * (c, d) = (a * c, b * d)
	* (a, b) / (c, d) = (a / c, b / d)
	*/
	const Vector2D& operator=(const Vector2D& that);
	const Vector2D& operator+=(const Vector2D& that);
	const Vector2D& operator-=(const Vector2D& that);

	const bool operator< (const Vector2D& that);

};


struct Vector2I
{
	//! Конструктор по умолчанию, вектор (0, 0)
	Vector2I();

	//! Конструктор вектора (a, b)
	Vector2I(int a, int b);

	//! Конструктор копирования
	Vector2I(const Vector2I& that);
	int x, y;

	//! Операции сложения векторов и умножения на число совпадают с классическими векторными операциями
	/*!
	* Введены операции умножения и деления вектора на вектор
	* (a, b) * (c, d) = (a * c, b * d)
	* (a, b) / (c, d) = (a / c, b / d)
	*/
	const Vector2I& operator=(const Vector2I& that);
	const Vector2I& operator+=(const Vector2I& that);
	const Vector2I& operator-=(const Vector2I& that);

	Vector2F to2F() const;

};

const Vector2F& operator+(const Vector2F& that);
const Vector2F operator-(const Vector2F& that);
const Vector2F operator+(const Vector2F& a, const Vector2F& b);
const Vector2F operator*(const Vector2F& a, const Vector2F& b);
const Vector2F operator/(const Vector2F& a, const Vector2F& b);
const Vector2F operator-(const Vector2F& a, const Vector2F& b);
const Vector2F operator*(float a, const Vector2F& b);
const Vector2F operator*(const Vector2F& a, float b);
const Vector2F operator/(const Vector2F& a, float b);
const bool operator==(const Vector2F& a, const Vector2F& b);
const bool operator!=(const Vector2F& a, const Vector2F& b);
const Vector2U& operator+(const Vector2U& that);
const Vector2U operator-(const Vector2U& that);
const Vector2U operator+(const Vector2U& a, const Vector2U& b);
const Vector2U operator*(const Vector2U& a, const Vector2U& b);
const Vector2U operator/(const Vector2U& a, const Vector2U& b);
const Vector2U operator-(const Vector2U& a, const Vector2U& b);
const Vector2U operator*(unsigned a, const Vector2U& b);
const Vector2U operator*(const Vector2U& a, unsigned b);
const Vector2U operator/(const Vector2U& a, unsigned b);
const bool operator==(const Vector2U& a, const Vector2U& b);
const bool operator!=(const Vector2U& a, const Vector2U& b);
const Vector2D& operator+(const Vector2D& that);
const Vector2D operator-(const Vector2D& that);
const Vector2D operator+(const Vector2D& a, const Vector2D& b);
const Vector2D operator*(const Vector2D& a, const Vector2D& b);
const Vector2D operator/(const Vector2D& a, const Vector2D& b);
const Vector2D operator-(const Vector2D& a, const Vector2D& b);
const Vector2D operator*(double a, const Vector2D& b);
const Vector2D operator*(const Vector2D& a, double b);
const Vector2D operator/(const Vector2D& a, double b);
const bool operator==(const Vector2D& a, const Vector2D& b);
const bool operator!=(const Vector2D& a, const Vector2D& b);
const Vector2I& operator+(const Vector2I& that);
const Vector2I operator-(const Vector2I& that);
const Vector2I operator+(const Vector2I& a, const Vector2I& b);
const Vector2I operator*(const Vector2I& a, const Vector2I& b);
const Vector2I operator/(const Vector2I& a, const Vector2I& b);
const Vector2I operator-(const Vector2I& a, const Vector2I& b);
const Vector2I operator*(int a, const Vector2I& b);
const Vector2I operator*(const Vector2I& a, int b);
const Vector2I operator/(const Vector2I& a, int b);
const bool operator==(const Vector2I& a, const Vector2I& b);
const bool operator!=(const Vector2I& a, const Vector2I& b);