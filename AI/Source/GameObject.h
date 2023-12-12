#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"
#include "StateMachine.h"
#include "ObjectBase.h"

struct GameObject : public ObjectBase
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CROSS,
		GO_CIRCLE,
		GO_FISH,
		GO_SHARK,
		GO_FISHFOOD,
		GO_BEE,
		GO_BEEHIVE,
		GO_NIGHTMARE,
		GO_CLOCKWORK,
		GO_BEEFALO,
		GO_BLACK,
		GO_WHITE,
		GO_TOTAL, //must be last
	};

	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	bool active;
	float mass;
	Vector3 target;
	int id;
	int steps;
	float energy;
	int health;
	int damage;
	float attackCooldownTimer;
	int attackCooldown;
	float moveSpeed;
	float breedingCooldownTimer;
	float breedingCooldown;
	float countDown;
	GameObject *nearest;
	bool moveLeft;
	bool moveRight;
	bool moveUp;
	bool moveDown;

	bool isAngry;
	StateMachine *sm;
	//each instance has to have its own currState and nextState pointer(can't be shared)
	State* currState; //week 5: should probably be private. put that under TODO
	State* nextState; //week 5: should probably be private. put that under TODO

	GameObject(GAMEOBJECT_TYPE typeValue = GO_NONE);
	~GameObject();

	bool Handle(Message* message);
};

#endif