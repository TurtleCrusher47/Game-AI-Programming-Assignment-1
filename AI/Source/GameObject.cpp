
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	sm(NULL),
	nearest(NULL)
{
	static int count = 0;
	id = ++count;
	moveLeft = moveRight = moveUp = moveDown = true;

}

GameObject::~GameObject()
{
	if (sm != NULL)
	{
		delete sm;
		sm = NULL;
	}
}