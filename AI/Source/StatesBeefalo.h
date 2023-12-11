#ifndef STATES_FISH_H
#define STATES_FISH_H

#include "State.h"
#include "GameObject.h"

class StateTooFull : public State
{
public:
	StateTooFull(const std::string &stateID);
	virtual ~StateTooFull();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateFull : public State
{
public:
	StateFull(const std::string &stateID);
	virtual ~StateFull();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateHungry : public State
{
public:
	StateHungry(const std::string &stateID);
	virtual ~StateHungry();

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