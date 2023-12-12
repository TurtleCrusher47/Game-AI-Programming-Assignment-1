#ifndef STATES_WX_H
#define STATES_WX_H

#include "State.h"
#include "GameObject.h"

class StateWXNeutral : public State
{
public:
	StateWXNeutral(const std::string &stateID);
	virtual ~StateWXNeutral();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWXLowPower : public State
{
public:
	StateWXLowPower(const std::string &stateID);
	virtual ~StateWXLowPower();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWXEnergised : public State
{
public:
	StateWXEnergised(const std::string &stateID);
	virtual ~StateWXEnergised();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWXDead : public State
{
public:
	StateWXDead(const std::string &stateID);
	virtual ~StateWXDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

#endif