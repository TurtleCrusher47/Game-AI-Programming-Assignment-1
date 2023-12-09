#include "StatesFish.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"
#include "SceneData.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float ENERGY_DROP_RATE = 0.2f;
static const float FULL_SPEED = 8.f;
static const float HUNGRY_SPEED = 4.f;

StateTooFull::StateTooFull(const std::string & stateID)
	: State(stateID)
{
}

StateTooFull::~StateTooFull()
{
}

void StateTooFull::Enter(GameObject* go)
{
	go->moveSpeed = 0;
}

void StateTooFull::Update(double dt, GameObject* go)
{
	go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (go->energy < 10.f)
		go->sm->SetNextState("Full", go);
}

void StateTooFull::Exit(GameObject* go)
{
}

StateFull::StateFull(const std::string & stateID)
	: State(stateID)
{
}

StateFull::~StateFull()
{
}

void StateFull::Enter(GameObject* go)
{
	go->moveSpeed = FULL_SPEED;
	go->nearest = NULL;
	go->countDown = 0;
}

void StateFull::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

	go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt);
	if (go->energy >= 10.f)
		go->sm->SetNextState("TooFull", go);
	else if (go->energy < 5.f)
		go->sm->SetNextState("Hungry", go);
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;

	//once nearest is set, fish will continue to move away from shark even
	//when they have move significantly far away (until it changes state).
	if (go->nearest)
	{
		if (go->nearest->pos.x > go->pos.x)
			go->moveRight = false;
		else
			go->moveLeft = false;
		if (go->nearest->pos.y > go->pos.y)
			go->moveUp = false;
		else
			go->moveDown = false;

		//if shark is far enough, reset nearest to null
		const float SHARK_DIST = 10.f * SceneData::GetInstance()->GetGridSize();
		if ((go->pos - go->nearest->pos).Length() > SHARK_DIST)
			go->nearest = nullptr;
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
			const float SHARK_DIST = 6.f * SceneData::GetInstance()->GetGridSize();
			PostOffice::GetInstance()->Send("Scene", 
				new MessageWRU(go, MessageWRU::NEAREST_SHARK, SHARK_DIST));
		}
	}
}

void StateFull::Exit(GameObject* go)
{
}

StateHungry::StateHungry(const std::string & stateID)
	: State(stateID)
{
}

StateHungry::~StateHungry()
{
}

void StateHungry::Enter(GameObject* go)
{
	go->moveSpeed = HUNGRY_SPEED;
	go->nearest = NULL;
	go->countDown = MESSAGE_INTERVAL;

	//week 5
	int range[] = { 3, 6 };
	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_FISHFOOD, 2, range));
}

void StateHungry::Update(double dt, GameObject* go)
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

void StateHungry::Exit(GameObject* go)
{
}

StateDead::StateDead(const std::string & stateID)
	: State(stateID)
{
}

StateDead::~StateDead()
{
}

void StateDead::Enter(GameObject* go)
{
	go->countDown = 3.f;
	go->moveSpeed = 0;
}

void StateDead::Update(double dt, GameObject* go)
{
	go->countDown -= static_cast<float>(dt);
	if (go->countDown < 0)
	{
		go->active = false;
	}
}

void StateDead::Exit(GameObject* go)
{
}
