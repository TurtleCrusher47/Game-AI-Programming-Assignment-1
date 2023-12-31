#include "StatesClockwork.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.2f;
static const float CHASE_SPEED = 10.f;

static const float WANDER_SPEED = 5.f;

StateClockworkWander::StateClockworkWander(const std::string & stateID)
	: State(stateID)
{
}

StateClockworkWander::~StateClockworkWander()
{
}

void StateClockworkWander::Enter(GameObject* go)
{
	go->moveSpeed = WANDER_SPEED;
	go->nearest = NULL;
	go->countDown = 0;
}

// Wander around until an enemy comes within range, then change to chase state and chase that enemy
void StateClockworkWander::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

	if (go->nearest)
	{
		go->sm->SetNextState("StateClockworkChase", go);
	}
	else
	{
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;


			std::cout << "Check" << std::endl;

			const float ENEMY_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
				PostOffice::GetInstance()->Send("Scene", 
					new MessageWRU(go, MessageWRU::NEAREST_PLAYER, ENEMY_DIST));
		}
	}
}

void StateClockworkWander::Exit(GameObject* go)
{
}

StateClockworkChase::StateClockworkChase(const std::string & stateID)
	: State(stateID)
{
}

StateClockworkChase::~StateClockworkChase()
{
}

void StateClockworkChase::Enter(GameObject* go)
{
	go->moveSpeed = CHASE_SPEED;
}

void StateClockworkChase::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
	//once nearest is set, clockwork will chase it
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

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			const float ENEMY_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", 
				new MessageWRU(go, MessageWRU::NEAREST_DAMAGEABLE, ENEMY_DIST));
		}
	}
}

void StateClockworkChase::Exit(GameObject* go)
{
}

StateClockworkDead::StateClockworkDead(const std::string & stateID)
	: State(stateID)
{
}

StateClockworkDead::~StateClockworkDead()
{
}

void StateClockworkDead::Enter(GameObject* go)
{
	go->countDown = 3.f;
	go->moveSpeed = 0;
	go->damage = 0;
}

void StateClockworkDead::Update(double dt, GameObject* go)
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
				if (go->nearest->type == GameObject::GO_WOLFGANG)
				go->nearest->sm->SetNextState("StateWolfgangDead", go->nearest);
			}
			else if (go->nearest->health <= 0)
			{
				if (go->nearest->type == GameObject::GO_WX)
				go->nearest->sm->SetNextState("StateWXDead", go->nearest);
			}
		}
	}
}

void StateClockworkDead::Exit(GameObject* go)
{
}
