#include "ObjectsArray.h"

tge::ObjectsArray::ObjectsArray()
{
	MAX_OBJECTS = 0;
	LAYERS_COUNT = 0;
	SetMaxObjects(0);
	SetLayersCount(0);
}

void tge::ObjectsArray::SetMaxObjects(unsigned new_max)
{
	top = 0;
	to_kill.top = 0;
	to_kill.obj_id.resize(0);
	to_kill.obj_id.resize(new_max, -1);
	for (unsigned i = 0; i < LAYERS_COUNT; i++) {
		layers[i].top = 0;
		layers[i].obj_id.resize(0);
		layers[i].obj_id.resize(new_max, -1);
	}
	for (unsigned i = 0; i < MAX_OBJECTS; i++) {
		if (obj[i].obj != nullptr) {
			delete obj[i].obj;
			obj[i].obj = nullptr;
		}
	}
	obj.resize(new_max);
	MAX_OBJECTS = new_max;
}

void tge::ObjectsArray::SetLayersCount(unsigned count)
{
	for (unsigned i = 0; i < MAX_OBJECTS; i++) {
		obj[i].layer_pos.resize(count, -1);
	}
	layers.resize(count);
	for (unsigned i = 0; i < count; i++)
		layers[i].obj_id.resize(MAX_OBJECTS);
	LAYERS_COUNT = count;
}