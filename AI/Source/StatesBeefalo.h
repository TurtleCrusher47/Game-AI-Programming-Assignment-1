#ifndef STATES_BEEFALO_H
#define STATES_BEEFALO_H

#include "State.h"
#include "GameObject.h"

class StateBeefaloWander : public State
{
public:
	StateBeefaloWander(const std::string &stateID);
	virtual ~StateBeefaloWander();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateBeefaloAngry : public State
{
public:
	StateBeefaloAngry(const std::string &stateID);
	virtual ~StateBeefaloAngry();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateBeefaloDead : public State
{
public:
	StateBeefaloDead(const std::string &stateID);
	virtual ~StateBeefaloDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};
#endif