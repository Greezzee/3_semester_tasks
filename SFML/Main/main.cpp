//#include <vld.h>
#include <iostream>
#include "../Tasks/PhysicsScene/PhysicsScene.h"
#include "../Tasks/ElectroScene/ElectroScene.h"
#include "../Tasks/Serpinsky/Serpinsky.h"

int main()
{
	GameManager::Init();
	Scene* start = new PhysicsScene;
	start->Init();
	GameManager::Launch(start);
	return 0;
}