#include "SceneMovement.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>
#include "StatesFish.h"
#include "StatesShark.h"
#include "StatesFishFood.h"
#include "SceneData.h"
#include "PostOffice.h"
#include "ConcreteMessages.h"

static const float FOOD_SPEED = 1.f;

SceneMovement::SceneMovement()
{
}

SceneMovement::~SceneMovement()
{
}

void SceneMovement::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//week 5
	//make statemachines for each object type
	InitStateMachines();

	SceneData::GetInstance()->SetObjectCount(0);
	SceneData::GetInstance()->SetFishCount(0);
	SceneData::GetInstance()->SetNumGrid(20);
	SceneData::GetInstance()->SetGridSize(m_worldHeight / SceneData::GetInstance()->GetNumGrid());
	SceneData::GetInstance()->SetGridOffset(SceneData::GetInstance()->GetGridSize() * 0.5f);
	m_hourOfTheDay = 0;

	float gridSize = SceneData::GetInstance()->GetGridSize();
	float gridOffset = SceneData::GetInstance()->GetGridOffset();
	int numGrid = SceneData::GetInstance()->GetNumGrid();
	GameObject *go = FetchGO(GameObject::GO_SHARK);
	go->scale.Set(gridSize, gridSize, gridSize);
	go->pos.Set(gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, numGrid - 1) * gridSize, 0);
	go->target = go->pos;
	go->sm->SetNextState("Happy", go);

	//week 4
	//register this scene with the "post office"
	//post office will now be capable of addressing this scene with messages
	PostOffice::GetInstance()->Register("Scene", this);
}

//week 5
//make statemachines for each object type
void SceneMovement::InitStateMachines()
{
	m_stateMachines.insert(std::make_pair(GameObject::GO_FISH, new StateMachine()));
	m_stateMachines.insert(std::make_pair(GameObject::GO_FISHFOOD, new StateMachine()));
	m_stateMachines.insert(std::make_pair(GameObject::GO_SHARK, new StateMachine()));
	
	//all fish will share this statemachine
	StateMachine* sm = m_stateMachines[GameObject::GO_FISH];
	sm->AddState(new StateTooFull("TooFull"));
	sm->AddState(new StateFull("Full"));
	sm->AddState(new StateHungry("Hungry"));
	sm->AddState(new StateDead("Dead"));

	//all food will share this statemachine
	sm = m_stateMachines[GameObject::GO_FISHFOOD];
	sm->AddState(new StateEvolve("Evolve"));
	sm->AddState(new StateGrow("Grow"));

	//all shark will share this statemachine
	sm = m_stateMachines[GameObject::GO_SHARK];
	sm->AddState(new StateCrazy("Crazy"));
	sm->AddState(new StateNaughty("Naughty"));
	sm->AddState(new StateHappy("Happy"));
}

GameObject* SceneMovement::FetchGO(GameObject::GAMEOBJECT_TYPE type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active && go->type == type)
		{
			go->active = true;
			return go;
		}
	}
	for (unsigned i = 0; i < 5; ++i)
	{
		GameObject *go = new GameObject(type);
		m_goList.push_back(go);
		//week 5
		//assign the appropriate statemachine to the object
		go->sm = m_stateMachines[type];
	}
	return FetchGO(type);
}

void SceneMovement::Update(double dt)
{
	SceneBase::Update(dt);

	static const float HOUR_SPEED = 1.f;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	float gridSize = SceneData::GetInstance()->GetGridSize();
	float gridOffset = SceneData::GetInstance()->GetGridOffset();
	int noGrid = SceneData::GetInstance()->GetNumGrid();
	
	if(Application::IsKeyPressed(VK_OEM_MINUS))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed(VK_OEM_PLUS))
	{
		m_speed += 0.1f;
	}

	m_hourOfTheDay += HOUR_SPEED * static_cast<float>(dt) * m_speed;
	if (m_hourOfTheDay >= 24.f)
		m_hourOfTheDay = 0;

	//Input Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}
	static bool bSpaceState = false;
	if (!bSpaceState && Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = true;
		GameObject *go = FetchGO(GameObject::GO_FISH);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->energy = 8.f;
		go->nearest = NULL;
		go->sm->SetNextState("Full", go);
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
	static bool bVState = false;
	if (!bVState && Application::IsKeyPressed('V'))
	{
		bVState = true;
		GameObject *go = FetchGO(GameObject::GO_FISHFOOD);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->moveSpeed = FOOD_SPEED;
		go->sm->SetNextState("Grow", go);
	}
	else if (bVState && !Application::IsKeyPressed('V'))
	{
		bVState = false;
	}
	static bool bBState = false;
	if (!bBState && Application::IsKeyPressed('B'))
	{
		bBState = true;
		GameObject *go = FetchGO(GameObject::GO_SHARK);
		go->scale.Set(gridSize, gridSize, gridSize);
		go->pos.Set(gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, gridOffset + Math::RandIntMinMax(0, noGrid - 1) * gridSize, 0);
		go->target = go->pos;
		go->sm->SetNextState("Happy", go);
	}
	else if (bBState && !Application::IsKeyPressed('B'))
	{
		bBState = false;
	}

	//StateMachine
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;
		if (go->sm)
			go->sm->Update(dt, go);
	}

	//do collision detection and response
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;

		//week 5 (bug fix)
		//set nearest to null if nearest is already inactive - so that fish will not hang at the same spot after consuming food
		if (go->nearest!=nullptr && !go->nearest->active)
			go->nearest = nullptr;

		if (go->type == GameObject::GO_FISH)
		{
			for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;
				if (!go2->active)
					continue;
				if (go2->type == GameObject::GO_SHARK)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < gridSize)
					{
						go->energy = -1;
					}
				}
				else if (go2->type == GameObject::GO_FISHFOOD)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < gridSize)
					{
						go->energy += 2.5f;
						go2->active = false;
					}
				}
			}
		}
	}

	//Movement Section
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;
		Vector3 dir = go->target - go->pos;
		if (dir.Length() < go->moveSpeed * dt * m_speed)
		{
			//GO->pos reach target
			go->pos = go->target;
			float random = Math::RandFloatMinMax(0.f, 1.f);
			if (random < 0.25f && go->moveRight)
				go->target = go->pos + Vector3(gridSize, 0, 0);
			else if (random < 0.5f && go->moveLeft)
				go->target = go->pos + Vector3(-gridSize, 0, 0);
			else if (random < 0.75f && go->moveUp)
				go->target = go->pos + Vector3(0, gridSize, 0);
			else if(go->moveDown)
				go->target = go->pos + Vector3(0, -gridSize, 0);
			if (go->target.x < 0 || go->target.x > noGrid * gridSize || go->target.y < 0 || go->target.y > noGrid * gridSize)
				go->target = go->pos;
		}
		else
		{
			try
			{
				dir.Normalize();
				go->pos += dir * go->moveSpeed * static_cast<float>(dt) * m_speed;
			}
			catch (DivideByZero)
			{
			}
		}
	}

	//week 4
	//Counting objects
	int objectCount = 0;
	m_numGO[GameObject::GO_FISH] = m_numGO[GameObject::GO_SHARK] = m_numGO[GameObject::GO_FISHFOOD] = 0;

	//create message on the stack, then pass the address of message to each gameobject
	//i.e. everyone shares the same message. fewer allocation of memory.
	MessageCheckActive msgCheckActive = MessageCheckActive();
	MessageCheckFish msgCheckFish = MessageCheckFish();
	MessageCheckFood msgCheckFood = MessageCheckFood();
	MessageCheckShark msgCheckShark = MessageCheckShark();
	for (GameObject* go : m_goList)
	{
		//since PostOffice does not support sending to multiple observers under
		//a single key, we opt for this approach
		//consider changing PostOffice to support this functionality if you want! :)
		objectCount += static_cast<int>(go->Handle(&msgCheckActive));
		m_numGO[GameObject::GO_FISH] += static_cast<int>(go->Handle(&msgCheckFish));
		m_numGO[GameObject::GO_FISHFOOD] += static_cast<int>(go->Handle(&msgCheckFood));
		m_numGO[GameObject::GO_SHARK] += static_cast<int>(go->Handle(&msgCheckShark));
	}

	SceneData::GetInstance()->SetObjectCount(objectCount);
	SceneData::GetInstance()->SetFishCount(m_numGO[GameObject::GO_FISH]);

	//week 5
	ProcessMessages();
}

void SceneMovement::RenderGO(GameObject *go)
{
	std::ostringstream ss;
	switch(go->type)
	{
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);

		ss.str("");
		ss.precision(3);
		ss << go->id;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISH:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);

		if (go->sm)
		{
			if (go->sm->GetCurrentState(go) == "TooFull")
				RenderMesh(meshList[GEO_TOOFULL], false);
			else if (go->sm->GetCurrentState(go) == "Full")
				RenderMesh(meshList[GEO_FULL], false);
			else if (go->sm->GetCurrentState(go) == "Hungry")
				RenderMesh(meshList[GEO_HUNGRY], false);
			else
				RenderMesh(meshList[GEO_DEAD], false);
		}

		modelStack.PushMatrix();
			ss.precision(3);
			ss << "[" << go->pos.x << ", " << go->pos.y << "]";
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, SceneData::GetInstance()->GetGridSize() / 4, 0);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();

		modelStack.PushMatrix();
			ss.str("");
			ss.precision(3);
			ss << go->energy;
			modelStack.Scale(0.5f, 0.5f, 0.5f);
			modelStack.Translate(0, -SceneData::GetInstance()->GetGridSize() / 4, 0);
			RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_SHARK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);

		if (go->sm)
		{
			if (go->sm->GetCurrentState(go) == "Crazy")
				RenderMesh(meshList[GEO_CRAZY], false);
			else if (go->sm->GetCurrentState(go) == "Happy")
				RenderMesh(meshList[GEO_HAPPY], false);
			else
				RenderMesh(meshList[GEO_SHARK], false);
		}
			modelStack.PushMatrix();
				ss.str("");
				ss.precision(3);
				ss << "[" << go->pos.x << ", " << go->pos.y << "]";
				modelStack.Scale(0.5f, 0.5f, 0.5f);
				modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, -SceneData::GetInstance()->GetGridSize() / 4, 0);
				RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FISHFOOD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FISHFOOD], false);
			modelStack.PushMatrix();
				ss.str("");
				ss.precision(3);
				//ss << "[" << go->pos.x << ", " << go->pos.y << "]";
				ss << "[" << go->countDown << "]";
				modelStack.Scale(0.5f, 0.5f, 0.5f);
				modelStack.Translate(-SceneData::GetInstance()->GetGridSize() / 4, -SceneData::GetInstance()->GetGridSize() / 4, 0);
				RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		break;
	}
}

void SceneMovement::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
	
	RenderMesh(meshList[GEO_AXES], false);

	modelStack.PushMatrix();
	modelStack.Translate(m_worldHeight * 0.5f, m_worldHeight * 0.5f, -1.f);
	modelStack.Scale(m_worldHeight, m_worldHeight, m_worldHeight);
	RenderMesh(meshList[GEO_BG], false);
	modelStack.PopMatrix();

	zOffset = 0;
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			zOffset += 0.001f;
			RenderGO(go);
		}
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(3);

	float fCounter = 0;
	for (std::vector<GameObject*>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject* go = (GameObject*)*it;
		if ((go->active) && (go->type == GameObject::GO_FISH) && (go->nearest))
		{
			ss.str("");
			ss << "Fish:" << go->id << "[" << go->pos.x << "," << go->pos.y << 
				"]>[" << go->nearest->pos.x << "," << go->nearest->pos.y << "]";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 56 - fCounter);
			fCounter += 2.0f;

			//week 5
			//draw line from fish to target
			RenderLine(go->pos, go->nearest->pos);
		}
	}

	ss.str("");
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);
	
	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Count:" << SceneData::GetInstance()->GetObjectCount();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss << "Fishes:" << m_numGO[GameObject::GO_FISH];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 18);

	ss.str("");
	ss << "Shark:" << m_numGO[GameObject::GO_SHARK];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 15);

	ss.str("");
	ss << "Food:" << m_numGO[GameObject::GO_FISHFOOD];
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Aquarium", Color(0, 1, 0), 3, 50, 0);
}

void SceneMovement::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
	//week 5
	//clean up message queue if need be
	while (!m_messageQueue.empty())
	{
		delete m_messageQueue.front();
		m_messageQueue.pop();
	}
	//clean up all statemachines
	//loop through entire map and delete
	//im using for_each to iterate through the map, but a standard for loop works too
	std::for_each(m_stateMachines.begin(), m_stateMachines.end(), [](auto& sm) {
		delete sm.second;
	});
}

bool SceneMovement::Handle(Message* message)
{
	//week 5 - instead of handling the message immediately upon firing the message,
	//		   we instead push the message into a message queue. The queue will
	//		   get handled at the end of the frame
	m_messageQueue.push(message);

	return true;
}

//week 5
//call this at the end of update, where we go through each message one by one
void SceneMovement::ProcessMessages()
{
	while(!m_messageQueue.empty())
	{
		//take message at front of queue and remove it from queue
		Message* message = m_messageQueue.front();
		m_messageQueue.pop();

		//handle each message as was done in week 4
		MessageWRU* messageWRU = dynamic_cast<MessageWRU*>(message);
		if (messageWRU)
		{
			//get pointer to the entity who fired the event
			//a FISH looking for fish food OR SHARK looking for fish etc.
			GameObject* go = messageWRU->go;
			go->nearest = nullptr;

			float nearestDistance = FLT_MAX; //FLT_MAX is max possible float value
			float highestEnergy = FLT_MIN; //FLT_MIN is min possible positive float value

			//scan through entire list looking for target object
			for (GameObject* go2 : m_goList)
			{
				if (!go2->active)
					continue;

				//message indicates go is hunting for nearest fish food
				if (messageWRU->type == MessageWRU::NEAREST_FISHFOOD &&
					go2->type == GameObject::GO_FISHFOOD)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < messageWRU->threshold && distance < nearestDistance)
					{
						nearestDistance = distance;
						go->nearest = go2;
					}
				}
				//message indicates go is seeking nearest shark
				else if (messageWRU->type == MessageWRU::NEAREST_SHARK &&
						 go2->type == GameObject::GO_SHARK)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < messageWRU->threshold && distance < nearestDistance)
					{
						nearestDistance = distance;
						go->nearest = go2;
					}
				}
				//message indicates go is hunting for full fish
				else if (messageWRU->type == MessageWRU::NEAREST_FULLFISH &&
						 go2->type == GameObject::GO_FISH)
				{
					float distance = (go->pos - go2->pos).Length();
					if (distance < nearestDistance &&
						(go2->sm->GetCurrentState(go2) == "TooFull" || go2->sm->GetCurrentState(go2) == "Full"))
					{
						nearestDistance = distance;
						go->nearest = go2;
					}
				}
				//message indicated go is hunting for highest energy fish
				else if (messageWRU->type == MessageWRU::HIGHEST_ENERGYFISH &&
						 go2->type == GameObject::GO_FISH)
				{
					if (go2->energy > highestEnergy)
					{
						highestEnergy = go2->energy;
						go->nearest = go2;
					}
				}
			}

			//week 5: make food stop
			if (go->nearest && go->nearest->type == GameObject::GO_FISHFOOD)
			{
				//this message is created on the stack because we're trying to avoid heap allocation where we can
				MessageStop msg; //msg will remain in memory until after it's been handled
				go->nearest->Handle(&msg);
			}
		}

		SceneData& sceneData = *SceneData::GetInstance(); //just defining a short hand for the singleton
		//week 5
		//check if this message is MessageSpawn
		MessageSpawn* messageSpawn = dynamic_cast<MessageSpawn*>(message);
		if (messageSpawn != nullptr)
		{
			for (int i = 0; i < messageSpawn->count; ++i)
			{
				//figure out the tile position to spawn the object(food)
				int sign = Math::RandFloatMinMax(0.f, 1.f) < 0.5f ? -1 : 1;
				GameObject* go = FetchGO((GameObject::GAMEOBJECT_TYPE)messageSpawn->type);
				//position is calculated as a random offset from the tile positions of the message sender(fish)
				int tileX = static_cast<int>(messageSpawn->go->pos.x / sceneData.GetGridSize()) + sign * Math::RandIntMinMax(messageSpawn->distRange[0], messageSpawn->distRange[1]);
				int tileY = static_cast<int>(messageSpawn->go->pos.y / sceneData.GetGridSize()) + sign * Math::RandIntMinMax(messageSpawn->distRange[0], messageSpawn->distRange[1]);
				tileX = Math::Clamp(tileX, 0, sceneData.GetNumGrid() - 1);
				tileY = Math::Clamp(tileY, 0, sceneData.GetNumGrid() - 1);

				go->scale.Set(sceneData.GetGridSize(), sceneData.GetGridSize(), sceneData.GetGridSize());
				go->pos.Set(sceneData.GetGridOffset() + tileX * sceneData.GetGridSize(),
							sceneData.GetGridOffset() + tileY * sceneData.GetGridSize(), 0);
				go->target = go->pos;
				go->moveSpeed = FOOD_SPEED;

				if (go->type == GameObject::GO_FISH) //case: shark spawns fish
				{
					go->energy = 8.0f;
					go->sm->SetNextState("Full", go);
				}
				else if (go->type == GameObject::GO_FISHFOOD) //case: fish spawns food
				{
					go->sm->SetNextState("Grow", go);
				}
			}
		}

		MessageEvolve* messageEvolve = dynamic_cast<MessageEvolve*>(message);
		if (messageEvolve != nullptr)
		{
			messageEvolve->go->active = false;
			GameObject* go = FetchGO(GameObject::GO_FISH);
			go->scale.Set(sceneData.GetGridSize(), sceneData.GetGridSize(), sceneData.GetGridSize());
			go->pos = messageEvolve->go->pos;
			go->target = messageEvolve->go->target;
			go->energy = 8.0f;
			go->sm->SetNextState("Full", go);
		}

		delete message; //remember, the message is allocated on the heap!
	}
}

//week 5
//helper function to draw line from fish to target
//make sure line mesh is supported
void SceneMovement::RenderLine(const Vector3& start, const Vector3& end)
{
	modelStack.Clear();
	modelStack.PushMatrix();
	const Vector3 displacement = end - start;
	modelStack.Translate(start.x, start.y, start.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(displacement.y, displacement.x)), 0, 0, 1);
	modelStack.Scale(displacement.Length(), 1.f, 1.f);
	RenderMesh(meshList[GEO_LINE], false);
	modelStack.PopMatrix();
}
