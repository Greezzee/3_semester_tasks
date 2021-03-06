#pragma once
#include "../../Engine/AllEngine.h"

void Drawtriag(int derph, Vector2F A, Vector2F B, Vector2F C)
{
	if (derph < 0)
		return;

	Debugger::DrawLine(A, B, 1);
	Debugger::DrawLine(B, C, 1);
	Debugger::DrawLine(C, A, 1);

	Drawtriag(derph - 1, A, (A + B) / 2.f, (A + C) / 2.f);
	Drawtriag(derph - 1, B, (C + B) / 2.f, (A + B) / 2.f);
	Drawtriag(derph - 1, C, (C + B) / 2.f, (A + C) / 2.f);
}

class Serpinsky : public Scene
{
public:
	void Init() {

	}
	void Update() {
		Vector2F A, B, C;
		A = { (float)500, (float)100 };
		B = { (float)100, (float)500 };
		C = { (float)800, (float)500 };
		Drawtriag(6, A, B, C);
	}
	void Destroy() {

	}
};