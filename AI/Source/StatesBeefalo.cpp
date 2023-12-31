#include "StatesBeefalo.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 4.f;

static const float WANDER_SPEED = 3.f;

StateBeefaloWander::StateBeefaloWander(const std::string & stateID)
	: State(stateID)
{
}

StateBeefaloWander::~StateBeefaloWander()
{
}

void StateBeefaloWander::Enter(GameObject* go)
{
	go->moveSpeed = WANDER_SPEED;
	go->breedingCooldownTimer = 0;
}

void StateBeefaloWander::Update(double dt, GameObject* go)
{
	go->breedingCooldownTimer += static_cast<float>(dt);

	if (go->breedingCooldownTimer >= go->breedingCooldown)
	{
		go->sm->SetNextState("StateBeefaloBreeding", go);
	}
}

void StateBeefaloWander::Exit(GameObject* go)
{
	go->breedingCooldownTimer = 0;
}

StateBeefaloBreeding::StateBeefaloBreeding(const std::string & stateID)
	: State(stateID)
{
}

StateBeefaloBreeding::~StateBeefaloBreeding()
{
}

void StateBeefaloBreeding::Enter(GameObject* go)
{
	int distance[] = { 1, 1 };
	
	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_BEEFALO, 1, distance));

	go->sm->SetNextState("StateBeefaloWander", go);
}

void StateBeefaloBreeding::Update(double dt, GameObject* go)
{
}

void StateBeefaloBreeding::Exit(GameObject* go)
{
}


StateBeefaloAngry::StateBeefaloAngry(const std::string & stateID)
	: State(stateID)
{
}

StateBeefaloAngry::~StateBeefaloAngry()
{
}

void StateBeefaloAngry::Enter(GameObject* go)
{
	go->moveSpeed = FULL_SPEED;
	go->countDown = 0;
}

void StateBeefaloAngry::Update(double dt, GameObject* go)
{
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
	//once nearest is set, beefalo will chase it
	if (go->nearest)
	{
		if (go->nearest->pos.x > go->pos.x)
			go->moveLeft = false;
		else
			go->moveRight = false;
		if (go->nearest->pos.y > go->pos.y)
			go->moveDown = false;
		else
			go->moveUp = false;
	}
	else //go->nearest is nullptr
	{
		go->sm->SetNextState("StateBeefaloWander", go);
	}
}

void StateBeefaloAngry::Exit(GameObject* go)
{
}

StateBeefaloDead::StateBeefaloDead(const std::string & stateID)
	: State(stateID)
{
}

StateBeefaloDead::~StateBeefaloDead()
{
}

void StateBeefaloDead::Enter(GameObject* go)
{
	go->countDown = 1.f;
	go->moveSpeed = 0;
}

void StateBeefaloDead::Update(double dt, GameObject* go)
{
	go->countDown -= static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	if (go->countDown <= 0)
	{
		go->active = false;
	}
}

void StateBeefaloDead::Exit(GameObject* go)
{
}
