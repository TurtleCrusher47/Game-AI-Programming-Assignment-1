#ifndef STATES_BEEFALO_H
#define STATES_BEEFALO_H

#include "State.h"
#include "GameObject.h"

class StateWander : public State
{
public:
	StateWander(const std::string &stateID);
	virtual ~StateWander();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateAngry : public State
{
public:
	StateAngry(const std::string &stateID);
	virtual ~StateAngry();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateDead : public State
{
public:
	StateDead(const std::string &stateID);
	virtual ~StateDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateDead : public State
{
public:
	StateDead(const std::string &stateID);
	virtual ~StateDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

#endif