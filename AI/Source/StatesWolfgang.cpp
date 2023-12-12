#include "StatesWolfgang.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float HUNGER_DROP_RATE = 6.f;
static const float CHASE_SPEED = 10.f;
static const float NEUTRAL_SPEED = 5.f;
static const float HUNGRY_SPEED = 7.f;
static const float SATIATED_SPEED = 0.f;

StateWolfgangNeutral::StateWolfgangNeutral(const std::string & stateID)
	: State(stateID)
{
}

StateWolfgangNeutral::~StateWolfgangNeutral()
{
}

void StateWolfgangNeutral::Enter(GameObject* go)
{
	go->moveSpeed = HUNGRY_SPEED;
	go->nearest = NULL;
	go->countDown = 0;
}

// Wander around until an enemy comes within range, then change to chase state and chase that enemy
void StateWolfgangNeutral::Update(double dt, GameObject* go)
{
	go->hunger -= HUNGER_DROP_RATE * static_cast<float>(dt);
	if (go->hunger < 60.f)
		go->sm->SetNextState("StateWolfgangHungry", go);
	else if (go->hunger > 80)
		go->sm->SetNextState("StateWolfgangSatiated", go);
}

void StateWolfgangNeutral::Exit(GameObject* go)
{
}

StateWolfgangHungry::StateWolfgangHungry(const std::string & stateID)
	: State(stateID)
{
}

StateWolfgangHungry::~StateWolfgangHungry()
{
}

void StateWolfgangHungry::Enter(GameObject* go)
{
	go->moveSpeed = CHASE_SPEED;
	go->hungry = true;
}

void StateWolfgangHungry::Update(double dt, GameObject* go)
{
	if (go->hunger > 60)
		go->sm->SetNextState("StateWolfgangNeutral", go);

	go->hunger -= HUNGER_DROP_RATE * static_cast<float>(dt);
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

void StateWolfgangHungry::Exit(GameObject* go)
{
	go->hungry = false;
}

StateWolfgangSatiated::StateWolfgangSatiated(const std::string & stateID)
	: State(stateID)
{
}

StateWolfgangSatiated::~StateWolfgangSatiated()
{
}

void StateWolfgangSatiated::Enter(GameObject* go)
{
	go->moveSpeed = SATIATED_SPEED;

	int distance[] = { 1, 1 };
	
	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_WOLFGANG, 1, distance));
}

void StateWolfgangSatiated::Update(double dt, GameObject* go)
{
	go->hunger -= HUNGER_DROP_RATE * static_cast<float>(dt);
	if (go->hunger < 80.f)
		go->sm->SetNextState("StateWolfgangNeutral", go);
}

void StateWolfgangSatiated::Exit(GameObject* go)
{
}

StateWolfgangDead::StateWolfgangDead(const std::string & stateID)
	: State(stateID)
{
}

StateWolfgangDead::~StateWolfgangDead()
{
}

void StateWolfgangDead::Enter(GameObject* go)
{
	//go->moveSpeed = HUNGRY_SPEED;
	go->countDown = 3.f;
	go->moveSpeed = 0;
}

void StateWolfgangDead::Update(double dt, GameObject* go)
{
	go->countDown -= static_cast<float>(dt);
	if (go->countDown <= 0)
	{
		go->active = false;

		if (go->nearest == NULL)
			return;

		if ((go->pos - go->nearest->pos).Length() <= 3 * SceneData::GetInstance()->GetGridSize())
		{
			go->nearest->health -= 30;

			if (go->nearest->health <= 0)
			{
				if (go->nearest->type == GameObject::GO_BEEFALO)
				go->nearest->sm->SetNextState("StateBeefaloDead", go->nearest);
			}
		}
	}
}

void StateWolfgangDead::Exit(GameObject* go)
{
}

