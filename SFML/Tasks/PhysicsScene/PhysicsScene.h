#include "../../Engine/AllEngine.h"
#include "Circle.h"
#include "Connector.h"
#include <iostream>
#include <vector>

class PhysicsScene : public Scene
{
public:
	void Init() {
		for (int i = 0; i < 1000; i++) {
			Circle a;
			a.Init({ (float)(rand() % 1280), (float)(rand() % 720) }, { 0, 0 }, 2, 2);
			//objs_.push_back(a);
		}
		active_mass = 4;
		active_bf = 1;
		frame_time = 0;
		GraphicManager::ShowFPS(true);

		//Circle* A = new Circle, * B = new Circle;
		//A->Init({ 400, 400 }, { 0, 0 }, 4, 1);
		//B->Init({ 500, 400 }, { 0, 0 }, 4, 1);
		//objs_.push_back(A);
		//objs_.push_back(B);

		//cons_.push_back(new ElasticConnector(objs_[0], objs_[1], 100, 5000));
	}
	void Update() {

		frame_time += TimeManager::GetDeltaTimeF();

		if (InputManager::IsPressed(KeyboardKey::R)) {
			Destroy();
			cons_.resize(0);
			objs_.resize(0);
		}

		if (InputManager::IsPressed(KeyboardKey::UP)) {
			active_mass += 1;
		}
		if (InputManager::IsPressed(KeyboardKey::DOWN) && active_mass > 1) {
			active_mass -= 1;
		}

		if (InputManager::IsPressed(MouseKey::Mouse_Left)) {
			pos = InputManager::GetMousePos();
		}

		if (InputManager::IsDown(MouseKey::Mouse_Left)) {
			Debugger::DrawLine(pos, InputManager::GetMousePos(), 2);
		}

		if (InputManager::IsPressed(KeyboardKey::N1))
			active_bf = 1.f;
		if (InputManager::IsPressed(KeyboardKey::N2))
			active_bf = 0.75f;
		if (InputManager::IsPressed(KeyboardKey::N3))
			active_bf = 0.5f;
		if (InputManager::IsPressed(KeyboardKey::N4))
			active_bf = 0.f;

		if (InputManager::IsRealesed(MouseKey::Mouse_Left)) {
			Circle* a = new Circle;
			a->Init(pos, (InputManager::GetMousePos() - pos) * 3, active_mass, active_bf);
			objs_.push_back(a);
		}

		if (frame_time >= dt) {
			frame_time -= dt;

			time += dt;

			if (time >= 0.01 && objs_.size() < 2) {
				Circle* a = new Circle;
				Circle* b = new Circle;
				int x = rand() % 1000 + 100;
				a->Init({ (float)(x), 0.f }, { 0, 0 }, 4, 1);
				b->Init({ (float)(x + 100), 20.f }, { 0, 0 }, 4, 1);
				objs_.push_back(a);
				objs_.push_back(b);
				cons_.push_back(new ElasticConnector(a, b, 120, 50));
				time = 0;
			}

			for (size_t i = 0; i < objs_.size(); i++) {
				objs_[i]->Update();
			}
			for (int k = 0; k < 1; k++) {
				for (size_t i = 0; i < objs_.size(); i++) {
					for (size_t j = i + 1; j < objs_.size(); j++) {
						objs_[i]->TestCollision(*objs_[j]);
					}
				}
			}
		}

		for (int i = 0; i < cons_.size(); i++) {
			cons_[i]->ApplyConnection();
			cons_[i]->Draw();
		}

		for (size_t i = 0; i < objs_.size(); i++) {
			objs_[i]->Draw();
		}

	}
	void Destroy() {
		for (int i = 0; i < cons_.size(); i++)
			delete cons_[i];
		for (int i = 0; i < objs_.size(); i++)
			delete objs_[i];
	}

private:
	std::vector<Circle*> objs_;
	std::vector<Connector*> cons_;
	Vector2F pos;

	float active_mass;
	float active_bf;
	float time = 0;

	float frame_time;
};