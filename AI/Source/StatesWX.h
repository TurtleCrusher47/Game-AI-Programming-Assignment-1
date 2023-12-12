#ifndef STATES_NIGHTMARE_H
#define STATES_NIGHTMARE_H

#include "State.h"
#include "GameObject.h"

class StateNightmareHungry : public State
{
public:
	StateNightmareHungry(const std::string &stateID);
	virtual ~StateNightmareHungry();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateNightmareChase : public State
{
public:
	StateNightmareChase(const std::string &stateID);
	virtual ~StateNightmareChase();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateNightmareSatiated : public State
{
public:
	StateNightmareSatiated(const std::string &stateID);
	virtual ~StateNightmareSatiated();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};
#endif