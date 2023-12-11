#include "StatesBeefalo.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.2f;
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
}

void StateBeefaloWander::Update(double dt, GameObject* go)
{
}

void StateBeefaloWander::Exit(GameObject* go)
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
	go->nearest = NULL;
	go->countDown = 0;
}

void StateBeefaloAngry::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

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
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			const float ENEMY_DIST = 2.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", 
				new MessageWRU(go, MessageWRU::NEAREST_DAMAGEABLE, ENEMY_DIST));
		}
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
	go->moveSpeed = HUNGRY_SPEED;
	go->nearest = NULL;
	go->countDown = MESSAGE_INTERVAL;

	//week 5
	int range[] = { 3, 6 };
	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_FISHFOOD, 2, range));
}

void StateBeefaloDead::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt); //check against this value before sending message(so we don't send the message every frame)

	go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (go->energy >= 5.f)
		go->sm->SetNextState("Full", go);
	else if (go->energy < 0.f)
	{
		go->sm->SetNextState("Dead", go);
	}
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
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
			const float FOOD_DIST = 20.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(go, MessageWRU::NEAREST_FISHFOOD, FOOD_DIST));
		}
	}
}

void StateBeefaloDead::Exit(GameObject* go)
{
}
