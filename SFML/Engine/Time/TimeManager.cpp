#include "TimeManager.h"
unsigned long long TimeManager::TimeFromLastRestart;
unsigned long long TimeManager::DeltaTime;
sf::Clock TimeManager::clock;

void TimeManager::Init() {
	clock.restart();
	TimeFromLastRestart = 0;
	DeltaTime = 0;
}

void TimeManager::Restart() {
	Init();
}

void TimeManager::Update() {
	unsigned long long t = clock.getElapsedTime().asMicroseconds();
	DeltaTime = t - TimeFromLastRestart;
	TimeFromLastRestart = t;
}

unsigned long long TimeManager::GetTimeFromLastRestart() {
	return TimeFromLastRestart;
}

unsigned long long TimeManager::GetDeltaTime() {
	return DeltaTime;
}

float TimeManager::GetDeltaTimeF() {
	return (float) (DeltaTime) / 1000000.0f;
}
