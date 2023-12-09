#include "StatesShark.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

static const float MESSAGE_INTERVAL = 1.f;
static const float CRAZY_SPEED = 16.f;
static const float NAUGHTY_SPEED = 12.f;
static const float HAPPY_SPEED = 8.f;

StateCrazy::StateCrazy(const std::string & stateID)
	: State(stateID)
{
}

StateCrazy::~StateCrazy()
{
}

void StateCrazy::Enter(GameObject* go)
{
	go->moveSpeed = CRAZY_SPEED;
	go->nearest = NULL;
	go->countDown = MESSAGE_INTERVAL;
}

void StateCrazy::Update(double dt, GameObject* go)
{
	if (SceneData::GetInstance()->GetFishCount() < 12)
		go->sm->SetNextState("Naughty", go);
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
	else //nearest is nullptr
	{
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(go, MessageWRU::HIGHEST_ENERGYFISH, 0)); //no need for threshold here
		}
	}
}

void StateCrazy::Exit(GameObject* go)
{
}

StateNaughty::StateNaughty(const std::string & stateID)
	: State(stateID)
{
}

StateNaughty::~StateNaughty()
{
}

void StateNaughty::Enter(GameObject* go)
{
	go->moveSpeed = NAUGHTY_SPEED;
	go->nearest = NULL;
	go->countDown = MESSAGE_INTERVAL;
}

void StateNaughty::Update(double dt, GameObject* go)
{
	go->countDown += static_cast<float>(dt);

	if (SceneData::GetInstance()->GetFishCount() > 10)
		go->sm->SetNextState("Crazy", go);
	else if(SceneData::GetInstance()->GetFishCount() < 6)
		go->sm->SetNextState("Happy", go);
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
	else //nearest is nullptr
	{
		if (go->countDown >= MESSAGE_INTERVAL) //ensure at least 1 second interval between messages
		{
			go->countDown -= MESSAGE_INTERVAL;

			//week 4
			//send message to Scene requesting for nearest to be updated
			//message is allocated on the heap (WARNING: expensive. 
			//either refactor PostOffice to not assume heap-allocated messages,
			//or pool messages to avoid real-time heap allocation)
			PostOffice::GetInstance()->Send("Scene", new MessageWRU(go, MessageWRU::NEAREST_FULLFISH, 0)); //no need for threshold here
		}
	}
}

void StateNaughty::Exit(GameObject* go)
{
}

StateHappy::StateHappy(const std::string & stateID)
	: State(stateID)
{
}

StateHappy::~StateHappy()
{
}

void StateHappy::Enter(GameObject* go)
{
	go->moveSpeed = HAPPY_SPEED;
	go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;

	int range[] = { 10, 15 };
	PostOffice::GetInstance()->Send("Scene", new MessageSpawn(go, GameObject::GO_FISH, 1, range));
}

void StateHappy::Update(double dt, GameObject* go)
{
	if (SceneData::GetInstance()->GetFishCount() > 4)
		go->sm->SetNextState("Naughty", go);
}

void StateHappy::Exit(GameObject* go)
{
}
