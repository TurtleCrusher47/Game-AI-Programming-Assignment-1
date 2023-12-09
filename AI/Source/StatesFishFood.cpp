#include "StatesFishFood.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

StateEvolve::StateEvolve(const std::string& stateID)
	: State(stateID)
{
}

void StateEvolve::Enter(GameObject* go)
{
	PostOffice::GetInstance()->Send("Scene", new MessageEvolve(go));
}

void StateEvolve::Update(double dt, GameObject* go)
{
}

void StateEvolve::Exit(GameObject* go)
{
}

StateGrow::StateGrow(const std::string& stateID)
	: State(stateID)
{
}

void StateGrow::Enter(GameObject* go)
{
	go->countDown = 0;
}

void StateGrow::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);
	if (go->countDown >= 15.f) //after 15 seconds, make fishfood evolve
	{
		go->sm->SetNextState("Evolve", go);
	}
}

void StateGrow::Exit(GameObject* go)
{
}
