#pragma once
#include "Circle.h"
class Connector
{
public:
	virtual void ApplyConnection() = 0;
	void Draw() {
		Debugger::DrawLine(A->GetPos(), B->GetPos(), thick, 0, color);
	}
protected:
	float len = 0;
	Circle* A = nullptr, * B = nullptr;
	Color color = Color::White();
	float thick = 0;
};

class SolidConnector : public Connector
{
public:
	SolidConnector(Circle* a, Circle* b, float l) {
		len = l;
		A = a;
		B = b;
		color = Color::White();
		thick = 2;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (std::abs(d - len) > 1) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - len;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
	}
};

class ElasticConnector : public Connector
{
public:
	ElasticConnector(Circle* a, Circle* b, float l, float k) {
		len = l;
		A = a;
		B = b;
		color = Color::Green();
		thick = 2;
		k_ = k;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (std::abs(d - len) > 1) {
			float dist = d - len;
			A->AddForce(-k_ * dist * n);
			B->AddForce(+k_ * dist * n);
		}
	}
private:
	float k_;
};

class MinMaxConnector : public Connector
{
public:
	MinMaxConnector(Circle* a, Circle* b, float lmin, float lmax) {
		l_min = lmin;
		l_max = lmax;
		A = a;
		B = b;
		color = Color::Blue();
		thick = 2;
	}
	void ApplyConnection() override {
		Vector2F con = A->GetPos() - B->GetPos();
		float d = con.Magnitude();
		Vector2F n = con / d;
		if (d - l_min < 0) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - l_min;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
		else if (l_max - d < 0) {
			Vector2F cent_speed = (A->GetSpeed() + B->GetSpeed()) / 2;
			float dist = d - l_max;
			A->SetPos(A->GetPos() - n * dist / 2);
			B->SetPos(B->GetPos() + n * dist / 2);
			A->SetSpeed(A->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - A->GetSpeed(), n));
			B->SetSpeed(B->GetSpeed() + n * Vector2F::ScalarMult(cent_speed - B->GetSpeed(), n));
		}
	}
private:
	float l_min, l_max;
};