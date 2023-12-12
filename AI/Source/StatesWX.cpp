#include "StatesWX.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float HUNGER_DROP_RATE = 0.3f;
static const float CHASE_SPEED = 10.f;

static const float HUNGRY_SPEED = 7.f;

StateWXNeutral::StateWXNeutral(const std::string & stateID)
	: State(stateID)
{
}

StateWXNeutral::~StateWXNeutral()
{
}

void StateWXNeutral::Enter(GameObject* go)
{
	go->moveSpeed = HUNGRY_SPEED;
	go->nearest = NULL;
	go->countDown = 0;
}

// Wander around until an enemy comes within range, then change to chase state and chase that enemy
void StateWXNeutral::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

	if (go->nearest)
	{
		go->sm->SetNextState("StateWXLowPower", go);
		std::cout << "chase" << std::endl;
		std::cout << go->nearest << std::endl;
	}
	else
	{
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;


			std::cout << "Check" << std::endl;

			const float ENEMY_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
				PostOffice::GetInstance()->Send("Scene", 
					new MessageWRU(go, MessageWRU::NEAREST_DAMAGEABLE, ENEMY_DIST));
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
	go->moveSpeed = CHASE_SPEED;
}

void StateWXLowPower::Update(double dt, GameObject* go)
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
	//go->moveSpeed = HUNGRY_SPEED;
	go->countDown = 3.f;
	go->moveSpeed = 0;
}

void StateWXEnergised::Update(double dt, GameObject* go)
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

void StateWXEnergised::Exit(GameObject* go)
{
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

void StateWXDead::Exit(GameObject* go)
{
}

