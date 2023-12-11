#ifndef STATES_CLOCKWORK_H
#define STATES_CLOCKWORK_H

#include "State.h"
#include "GameObject.h"

class StateClockworkWander : public State
{
public:
	StateClockworkWander(const std::string &stateID);
	virtual ~StateClockworkWander();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateClockworkChase : public State
{
public:
	StateClockworkChase(const std::string &stateID);
	virtual ~StateClockworkChase();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateClockworkDead : public State
{
public:
	StateClockworkDead(const std::string &stateID);
	virtual ~StateClockworkDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};
#endif