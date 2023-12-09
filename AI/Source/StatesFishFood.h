#pragma once

#include "State.h"
#include "GameObject.h"

//week 5
//these 2 states are meant for fishfood

class StateEvolve : public State
{
public:
	StateEvolve(const std::string& stateID);
	~StateEvolve() {};

	void Enter(GameObject* go);
	void Update(double dt, GameObject* go);
	void Exit(GameObject* go);
};

class StateGrow : public State
{
public:
	StateGrow(const std::string& stateID);
	~StateGrow() {};

	void Enter(GameObject* go);
	void Update(double dt, GameObject* go);
	void Exit(GameObject* go);
};
