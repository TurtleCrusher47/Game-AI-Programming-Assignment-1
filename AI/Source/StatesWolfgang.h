#ifndef STATES_WOLFGANG_H
#define STATES_WOLFGANG_H

#include "State.h"
#include "GameObject.h"

class StateWolfgangNeutral : public State
{
public:
	StateWolfgangNeutral(const std::string &stateID);
	virtual ~StateWolfgangNeutral();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWolfgangHungry : public State
{
public:
	StateWolfgangHungry(const std::string &stateID);
	virtual ~StateWolfgangHungry();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWolfgangSatiated : public State
{
public:
	StateWolfgangSatiated(const std::string &stateID);
	virtual ~StateWolfgangSatiated();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

class StateWolfgangDead : public State
{
public:
	StateWolfgangDead(const std::string &stateID);
	virtual ~StateWolfgangDead();

	virtual void Enter(GameObject* go);
	virtual void Update(double dt, GameObject* go);
	virtual void Exit(GameObject* go);
};

#endif