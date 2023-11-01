
#include "GameObject.h"

GameObject::GameObject(GAMEOBJECT_TYPE typeValue) 
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	moveSpeed(1.f),
	energy(10.f),
	sm(NULL),
	nearest(NULL)
{
}

GameObject::~GameObject()
{
	if (sm != NULL)
	delete sm;
}

void GameObject::Update(double dt)
{
	// //StateMachine
	// static const float ENERGY_DROP_RATE = 0.2f;
	// static const float FULL_SPEED = 8.f;
	// static const float HUNGRY_SPEED = 4.f;
	// for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	// {
	// 	GameObject *go = (GameObject *)*it;
	// 	if (!go->active)
	// 		continue;
	// 	if(go->type == GameObject::GO_FISH)
	// 	{
	// 		switch (go->currState)
	// 		{
	// 		case GameObject::STATE_TOOFULL:
	// 			go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
	// 			go->moveSpeed = 0;
	// 			if (go->energy < 10.f)
	// 				go->currState = GameObject::STATE_FULL;
	// 			break;
	// 		case GameObject::STATE_FULL:
	// 			go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
	// 			go->moveSpeed = FULL_SPEED;
	// 			if (go->energy >= 10.f)
	// 				go->currState = GameObject::STATE_TOOFULL;
	// 			else if (go->energy < 5.f)
	// 				go->currState = GameObject::STATE_HUNGRY;
	// 			go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
	// 			if (go->nearest)
	// 			{
	// 				if (go->nearest->pos.x > go->pos.x)
	// 					go->moveRight = false;
	// 				else
	// 					go->moveLeft = false;
	// 				if (go->nearest->pos.y > go->pos.y)
	// 					go->moveUp = false;
	// 				else
	// 					go->moveDown = false;
	// 			}
	// 			break;
	// 		case GameObject::STATE_HUNGRY:
	// 			go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
	// 			go->moveSpeed = HUNGRY_SPEED;
	// 			if (go->energy >= 5.f)
	// 				go->currState = GameObject::STATE_FULL;
	// 			else if (go->energy < 0.f)
	// 			{
	// 				go->currState = GameObject::STATE_DEAD;
	// 				go->countDown = 3.f;
	// 			}
	// 			go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
	// 			if (go->nearest)
	// 			{
	// 				if (go->nearest->pos.x > go->pos.x)
	// 					go->moveLeft = false;
	// 				else
	// 					go->moveRight = false;
	// 				if (go->nearest->pos.y > go->pos.y)
	// 					go->moveDown = false;
	// 				else
	// 					go->moveUp = false;
	// 			}
	// 			break;
	// 		case GameObject::STATE_DEAD:
	// 			go->moveSpeed = 0;
	// 			go->countDown -= static_cast<float>(dt) * m_speed;
	// 			if (go->countDown < 0)
	// 			{
	// 				go->active = false;
	// 				--m_objectCount;
	// 			}
	// 			continue;
	// 			break;
	// 		}
	// 	}
	// }
}