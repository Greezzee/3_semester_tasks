#include "GameObject.h"
void GameObject::SetID(unsigned new_id)
{
	_ID = new_id;
}

unsigned GameObject::GetID() const
{
	return _ID;
}

void GameObject::SetGamefield(ObjectContainer* field)
{
	gamefield = field;
}

ObjectContainer* GameObject::GetGamefield()
{
	return gamefield;
}

Vector2F GameObject::GetPos()
{
	return _pos;
}

float GameObject::GetRotation()
{
	return _angle;
}

void GameObject::SetTag(const std::string& new_tag)
{
	_tag = new_tag;
}

const std::string& GameObject::GetTag()
{
	return _tag;
}

Vector2F GameObject::GetDirection()
{
	return _basic_direction;
}

void GameObject::SetPos(Vector2F pos) {
	this->_pos = pos;
}

void GameObject::SetDirection(Vector2F dir) {
	this->_basic_direction = dir;
}

void GameObject::SetRotation(float angle) {
	this->_angle = angle;
}