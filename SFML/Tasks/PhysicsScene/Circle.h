#pragma once
#include "../../Engine/AllEngine.h"

const float dt = 0.01;

class Circle : public GameObject
{
public:
	Vector2F GetSpeed() {
		return speed_;
	}
	void SetSpeed(Vector2F a) {
		speed_ = a;
	}
	void Init() {
		mass_ = 4;
		collider_.SetRadius(mass_ * 5.f);
		_pos = { 300, 300 };
		prev_pos_ = _pos;
		accel_ = { 0, 0 };
		//std::cout << "Obj created\n";
	}

	void Init(Vector2F pos, Vector2F speed, float mass = 4, float bf = 1) {
		mass_ = mass;
		collider_.SetRadius(mass_ * 5.f);
		_pos = pos;
		prev_pos_ = _pos;
		speed_ = speed;
		//std::cout << "Obj created\n";
		bounce_factor = bf;
		switch ((int)(bf * 4))
		{
		case 4:
			color = Color::White();
			break;
		case 3:
			color = Color::Blue();
			break;
		case 2:
			color = Color::Red();
			break;
		case 0:
			color = Color::Green();
			break;
		}
		//AddForce(Vector2F(0, 200) / dt * mass_);
	}

	//! Обновление объекта. Вызывается каждый кадр (на данный момент. В будущем появится пауза и данная функция вызываться в ней не будет)
	void Update() {

		AddForce(Vector2F(0, 200.f * mass_));

		speed_ += accel_ * dt;
		_pos += speed_ * dt + 0.5 * accel_ * dt * dt;
		collider_.SetPos(_pos);

		if (_pos.y < 0 || _pos.y > 720 - mass_ * 5.0) {
			if (_pos.y < 0)
				_pos.y = 0;
			if (_pos.y > 720 - mass_ * 5.0)
				_pos.y = 720 - mass_ * 5.0;
			speed_.y *= -bounce_factor;
		}
		if (_pos.x < 0 || _pos.x > 1280) {
			if (_pos.x < 0)
				_pos.x = 0;
			if (_pos.x > 1280)
				_pos.x = 1280;
			speed_.x *= -bounce_factor;
		}

		accel_ = { 0, 0 };
	}

	//! Рисование объекта. Переопределяйте вручную! Вызывается вообще каждый кадр (даже на паузе)
	void Draw() {
		Debugger::DrawCollider(collider_, 0, color);
	}

	//! Уничтожение объекта. Вызывается 1 раз, когда игровое поле уничтожает объект
	void Destroy() {

	}

	void TestCollision(Circle& other) {
		if (Collider::IsCollide(&collider_, &other.collider_)) {
			//std::cout << "Collided!\n";
			Vector2F line = (_pos - other._pos).Normalized();
			float a = Vector2F::ScalarMult(line, speed_ - other.speed_);
			float distance = (_pos - other._pos).Magnitude() - mass_ * 5.0 - other.mass_ * 5.0;
			/*
			if (mass_ >= other.mass_)
				other.pos_ += line * distance;
			else
				pos_ += line * distance;
			*/
			float p1 = mass_ * speed_.Magnitude();
			float p2 = other.mass_ * other.speed_.Magnitude();
			_pos -= line * distance / (p1 + p2) * p1;
			other._pos += line * distance / (p1 + p2) * p2;

			if (a < EPS) {
				//std::cout << "applied!\n";
				Vector2F dp = Vector2F::ScalarMult((1 + bounce_factor * other.bounce_factor) * (speed_ - other.speed_) / (1.f / mass_ + 1.f / other.mass_), line) * line;
				speed_ -= dp / mass_;
				other.speed_ += dp / other.mass_;
			}
		}
	}

	//! Создание абсолютно нового объекта этого типа
	GameObject* Clone() const {
		return new Circle;
	}

	void AddForce(Vector2F force) {
		accel_ += force / mass_;
	}
private:
	CircleCollider collider_;
	float mass_;
	float bounce_factor = 1.f;
	Color color = Color::White();

	Vector2F prev_pos_;

	Vector2F accel_, speed_;
};