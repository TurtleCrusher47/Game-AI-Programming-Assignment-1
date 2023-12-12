#include "StatesWX.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float POWER_DROP_RATE = 5.f;
static const float COUNTDOWN_DROP_RATE = 5.f;
static const float NEUTRAL_SPEED = 8.f;
static const float LOW_POWER_SPEED = 3.f;
static const float ENERGISED_SPEED = 15.f;

StateWXNeutral::StateWXNeutral(const std::string & stateID)
	: State(stateID)
{
}

StateWXNeutral::~StateWXNeutral()
{
}

void StateWXNeutral::Enter(GameObject* go)
{
	go->moveSpeed = NEUTRAL_SPEED;
	go->nearest = NULL;
	go->countDown = 0;
}

// Wander around until an enemy comes within range, then change to chase state and chase that enemy
void StateWXNeutral::Update(double dt, GameObject* go)
{
	go->power -= POWER_DROP_RATE * static_cast<float>(dt);
	if (go->power < 20.f)
		go->sm->SetNextState("StateWXLowPower", go);
	else if (go->power > 80)
		go->sm->SetNextState("StateWXEnergised", go);

	go->deathCountdown -= COUNTDOWN_DROP_RATE * static_cast<float>(dt);
	if (go->deathCountdown <= 0)
		go->sm->SetNextState("StateWXDead", go);

	go->countDown += static_cast<float>(dt);
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
	//once nearest is set, wx will chase it
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
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;

			const float ENEMY_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", 
				new MessageWRU(go, MessageWRU::NEAREST_BEEFALO, ENEMY_DIST));
		}
	}
}

void StateWXNeutral::Exit(GameObject* go)
{
}

StateWXLowPower::StateWXLowPower(const std::string & stateID)
	: State(stateID)
{
}

StateWXLowPower::~StateWXLowPower()
{
}

void StateWXLowPower::Enter(GameObject* go)
{
	go->moveSpeed = LOW_POWER_SPEED;
}

void StateWXLowPower::Update(double dt, GameObject* go)
{
	go->power -= POWER_DROP_RATE * static_cast<float>(dt);
	if (go->power > 21.f)
		go->sm->SetNextState("StateWXNeutral", go);
	else if (go->power > 80)
		go->sm->SetNextState("StateWXEnergised", go);

	go->deathCountdown -= COUNTDOWN_DROP_RATE * static_cast<float>(dt);
	if (go->deathCountdown <= 0)
		go->sm->SetNextState("StateWXDead", go);

	if (go->power <= 0)
		go->sm->SetNextState("StateWXDead", go);


	go->countDown += static_cast<float>(dt);
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;

	//once nearest is set, wolfgang will chase it
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
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;

			const float ENEMY_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", 
				new MessageWRU(go, MessageWRU::NEAREST_BEEFALO, ENEMY_DIST));
		}
	}
}

void StateWXLowPower::Exit(GameObject* go)
{
}

StateWXEnergised::StateWXEnergised(const std::string & stateID)
	: State(stateID)
{
}

StateWXEnergised::~StateWXEnergised()
{
}

void StateWXEnergised::Enter(GameObject* go)
{
	go->moveSpeed = ENERGISED_SPEED;

	if (go->canSpawn)
	{
		int distance[] = { 1, 1 };
		PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_WX, 1, distance));
	}
}

void StateWXEnergised::Update(double dt, GameObject* go)
{
	go->power -= POWER_DROP_RATE * static_cast<float>(dt);
	if (go->power <= 75.f)
		go->sm->SetNextState("StateWXNeutral", go);

	go->deathCountdown -= COUNTDOWN_DROP_RATE * static_cast<float>(dt);
	if (go->deathCountdown <= 0)
		go->sm->SetNextState("StateWXDead", go);
}

void StateWXEnergised::Exit(GameObject* go)
{
	go->canSpawn = true;
}

StateWXDead::StateWXDead(const std::string & stateID)
	: State(stateID)
{
}

StateWXDead::~StateWXDead()
{
}

void StateWXDead::Enter(GameObject* go)
{
	//go->moveSpeed = HUNGRY_SPEED;
	go->countDown = 3.f;
	go->moveSpeed = 0;
}

void StateWXDead::Update(double dt, GameObject* go)
{
	go->countDown -= static_cast<float>(dt);
	if (go->countDown <= 0)
	{
		go->active = false;
	}
}

void StateWXDead::Exit(GameObject* go)
{
}

