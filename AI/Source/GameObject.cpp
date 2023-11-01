
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	// sm(NULL),
	nearest(NULL)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update(double dt)
{

}