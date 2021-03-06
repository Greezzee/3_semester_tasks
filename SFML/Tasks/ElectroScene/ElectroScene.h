#pragma once
#include "../../Engine/AllEngine.h"

struct Charge {
	float q;
	Vector2F pos;
};

class ElectroScene : public Scene
{
public:
	void Init() {
		q = 100;
		type = true;
		potential_field = std::vector<std::vector<float>>(800, std::vector<float>(450, 0));
		//charges_.push_back({ -1000, {400, 200} });
		//charges_.push_back({ 1000, {500, 200} });
	}
	void Update() {
		if (InputManager::IsPressed(MouseKey::Mouse_Right))
			q *= -1;
		if (InputManager::IsPressed(MouseKey::Mouse_Left)) {
			charges_.push_back({ q, InputManager::GetMousePos() });
			for (int i = 0; i < 800; i += 1)
				for (int j = 0; j < 450; j += 1) {
					Vector2F pos = { (float)i * 2, (float)j * 2 };
					potential_field[i][j] += charges_.back().q / (charges_.back().pos - pos).Magnitude();
				}
		}
		if (InputManager::IsPressed(KeyboardKey::SPACE))
			type = !type;
		if (InputManager::IsPressed(KeyboardKey::R)) {
			charges_.resize(0);
			potential_field = std::vector<std::vector<float>>(800, std::vector<float>(450, 0));
		}
		if (InputManager::IsPressed(UP)) {
			q = q / std::abs(q) * (std::abs(q) + 20);
		}

		if (InputManager::IsPressed(DOWN)) {
			if (std::abs(q) > 20)
				q = q / std::abs(q) * (std::abs(q) - 20);
		}


		charges_.push_back({ q, InputManager::GetMousePos() });
		if (type) {
			for (int i = 0; i < 1600; i += 40)
				for (int j = 0; j < 900; j += 40) {
					Vector2F E{ 0, 0 };
					Vector2F pos = { (float)i, (float)j };
					for (int k = 0; k < charges_.size(); k++) {
						E += charges_[k].q / std::pow((charges_[k].pos - pos).Magnitude(), 3) * (charges_[k].pos - pos);
					}
					Debugger::DrawLine(pos - E.Normalized() * 20, pos + E.Normalized() * 20, 1);
					Debugger::DrawLine(pos - E.Normalized() * 20, pos - E.Normalized() * 10, 2, 0, Color::Red());
				}
		}
		else {
			auto present_field = potential_field;
			for (int i = 0; i < 800; i += 1)
				for (int j = 0; j < 450; j += 1) {
					Vector2F pos = { (float)i * 2, (float)j * 2 };
					present_field[i][j] += charges_.back().q / (charges_.back().pos - pos).Magnitude();
				}
			for (int i = 0; i < 800 - 1; i += 1)
				for (int j = 0; j < 450 - 1; j += 1) {
					Vector2F pos = { (float)i * 2, (float)j * 2 };
					if (present_field[i][j] >= 0 && present_field[i + 1][j] >= 0 && present_field[i][j + 1] >= 0 && present_field[i + 1][j + 1] >= 0 ||
						present_field[i][j] <= 0 && present_field[i + 1][j] <= 0 && present_field[i][j + 1] <= 0 && present_field[i + 1][j + 1] <= 0) {
					}
					else
						Debugger::DrawPoint(pos, 2);
				}
		}
		for (int k = 0; k < charges_.size(); k++)
			Debugger::DrawPoint(charges_[k].pos, charges_[k].q / 10, 0, charges_[k].q >= 0 ? Color::Red() : Color::Blue());
		charges_.pop_back();
	}
	void Destroy() {

	}
private:
	std::vector<Charge> charges_;
	std::vector<std::vector<float>> potential_field;
	float q;
	bool type;
};