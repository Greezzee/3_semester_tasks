#include "ObjectContainer.h"

#include <stdlib.h>
#include <stdio.h>
using namespace tge;
void ObjectContainer::Init()
{
	_objects = new ObjectsArray;

	_objects->SetLayersCount(0);
	_objects->SetMaxObjects(0);
}

void ObjectContainer::Init(unsigned max_obj, unsigned layers_count)
{
	_objects = new ObjectsArray;

	_objects->SetMaxObjects(max_obj);
	_objects->SetLayersCount(layers_count);
}

void ObjectContainer::Destroy()
{
	for (unsigned i = 0; i < _objects->MAX_OBJECTS; i++)
		if (_objects->obj[i].obj != nullptr) {
			KillObject(i);
		}
	ClearKilledObjects();
	delete _objects;
}

void ObjectContainer::SpawnObject(const GameobjectSpawnData& new_object)
{
	if (_objects->top < _objects->MAX_OBJECTS) {
		_objects->obj[_objects->top].obj = new_object.obj;
		new_object.obj->SetID(_objects->top);
		new_object.obj->SetGamefield(this);
		for (int i = 0; i < _objects->LAYERS_COUNT; i++) {
			if (new_object.is_on_layer[i]) {
				_objects->layers[i].obj_id[_objects->layers[i].top] = _objects->top;
				_objects->obj[_objects->top].layer_pos[i] = _objects->layers[i].top;
				_objects->layers[i].top++;
			}
		}
		_objects->top++;
	}
	else
		throw;
}

void ObjectContainer::KillObject(const unsigned ID)
{
	if (_objects->to_kill.top < _objects->MAX_OBJECTS) {
		_objects->to_kill.obj_id[_objects->to_kill.top] = ID;
		_objects->to_kill.top++;
	}
}

void ObjectContainer::KillObject(const GameObject* object)
{
	KillObject(object->GetID());
}

void ObjectContainer::ClearKilledObjects()
{
	for (unsigned to_delete = 0; to_delete < _objects->to_kill.top; to_delete++) {
		int delete_id = _objects->to_kill.obj_id[to_delete];
		if (delete_id == -1)
			continue;
		for (int i = 0; i < _objects->LAYERS_COUNT; i++) //стираем объект со всех слоёв
			if (_objects->obj[delete_id].layer_pos[i] != -1) {
				if (_objects->obj[delete_id].layer_pos[i] == _objects->layers[i].top - 1) {
					_objects->layers[i].top--;
					_objects->layers[i].obj_id[_objects->layers[i].top] = -1;
				}
				else {
					_objects->layers[i].top--;
					unsigned moved_id = _objects->layers[i].obj_id[_objects->layers[i].top];
					_objects->layers[i].obj_id[_objects->obj[delete_id].layer_pos[i]] = moved_id;
					_objects->layers[i].obj_id[_objects->obj[moved_id].layer_pos[i]] = -1;
					_objects->obj[moved_id].layer_pos[i] = _objects->obj[delete_id].layer_pos[i];
				}
			}
		//стираем сам объект
		_objects->obj[delete_id].obj->Destroy();
		delete _objects->obj[delete_id].obj;
		_objects->top--;
		if (delete_id == _objects->top) {
			_objects->obj[delete_id].obj = nullptr;
			for (int i = 0; i < _objects->LAYERS_COUNT; i++)
				_objects->obj[delete_id].layer_pos[i] = -1;
		}
		else {
			unsigned moved_id = _objects->top;
			_objects->obj[delete_id] = _objects->obj[moved_id];
			_objects->obj[delete_id].obj->SetID(delete_id);
			for (int i = 0; i < _objects->LAYERS_COUNT; i++)
				if (_objects->obj[moved_id].layer_pos[i] != -1)
					_objects->layers[i].obj_id[_objects->obj[moved_id].layer_pos[i]] = delete_id;
			_objects->obj[moved_id].obj = nullptr;
			for (int i = 0; i < _objects->LAYERS_COUNT; i++)
				_objects->obj[moved_id].layer_pos[i] = -1;
			for (unsigned i = to_delete + 1; i < _objects->to_kill.top; i++) {
				if (_objects->to_kill.obj_id[i] == moved_id)
					_objects->to_kill.obj_id[i] = delete_id;
				else if (_objects->to_kill.obj_id[i] == delete_id)
					_objects->to_kill.obj_id[i] = -1;
			}
				
		}
	}
	for (unsigned i = 0; i < _objects->to_kill.top; i++)
		_objects->to_kill.obj_id[i] = -1;
	_objects->to_kill.top = 0;

}

void ObjectContainer::ApplyToAllObjects(void(*func)(GameObject*, void*), void* data)
{
	for (unsigned i = 0; i < _objects->top; i++)
		func(_objects->obj[i].obj, data);
}
void ObjectContainer::ApplyToLayerObjects(unsigned layer_id, void(*func)(GameObject*, void*), void* data)
{
	if (layer_id >= _objects->LAYERS_COUNT)
		return;

	for (unsigned i = 0; i < _objects->layers[layer_id].top; i++)
		func(_objects->obj[_objects->layers[layer_id].obj_id[i]].obj, data);
}