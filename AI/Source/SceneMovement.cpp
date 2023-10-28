#include "SceneMovement.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

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

	m_objectCount = 0;
	m_noGrid = 20;
	m_gridSize = m_worldHeight / m_noGrid;
	m_gridOffset = m_gridSize / 2;
	m_hourOfTheDay = 0;

	m_maxGridSize = m_gridSize * 20;

	GameObject *go = FetchGO();
	go->type = GameObject::GO_SHARK;
	go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
	go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
	go->target = go->pos;
	go->moveSpeed = 10.f;

}

// Makes a list of all the gameobjects and sets them active if they are not
GameObject* SceneMovement::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
		{
			go->active = true;
			++m_objectCount;
			return go;
		}
	}
	for (unsigned i = 0; i < 10; ++i)
	{
		GameObject *go = new GameObject(GameObject::GO_BALL);
		m_goList.push_back(go);
	}
	return FetchGO();
}

void SceneMovement::Update(double dt)
{
	SceneBase::Update(dt);

	static const float BALL_SPEED = 5.f;
	static const float HOUR_SPEED = 1.f;

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();
	
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
		GameObject *go = FetchGO();
		go->type = GameObject::GO_FISH;
		go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
		go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
		go->target = go->pos;
		go->steps = 0;
		go->currState = GameObject::STATE_FULL;
		go->energy = 8.f;
		go->nearest = NULL;
	}
	else if (bSpaceState && !Application::IsKeyPressed(VK_SPACE))
	{
		bSpaceState = false;
	}
	static bool bVState = false;
	if (!bVState && Application::IsKeyPressed('V'))
	{
		bVState = true;
		GameObject *go = FetchGO();
		go->type = GameObject::GO_FISHFOOD;
		go->scale.Set(m_gridSize, m_gridSize, m_gridSize);
		go->pos.Set(m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, m_gridOffset + Math::RandIntMinMax(0, m_noGrid - 1) * m_gridSize, 0);
		go->target = go->pos;
		go->moveSpeed = 1.f;
	}
	else if (bVState && !Application::IsKeyPressed('V'))
	{
		bVState = false;
	}

	//StateMachine
	static const float ENERGY_DROP_RATE = 0.2f;
	static const float FULL_SPEED = 8.f;
	static const float HUNGRY_SPEED = 4.f;
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->active)
			continue;
		if(go->type == GameObject::GO_FISH)
		{
			switch (go->currState)
			{
			case GameObject::STATE_TOOFULL:
				go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
				go->moveSpeed = 0;
				if (go->energy < 10.f)
					go->currState = GameObject::STATE_FULL;
				break;
			case GameObject::STATE_FULL:
				go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
				go->moveSpeed = FULL_SPEED;
				if (go->energy >= 10.f)
					go->currState = GameObject::STATE_TOOFULL;
				else if (go->energy < 5.f)
					go->currState = GameObject::STATE_HUNGRY;
				go->moveLeft = go->moveRight = go->moveUp = go->moveDown = true;
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
				}
				break;
			case GameObject::STATE_HUNGRY:
				go->energy -= ENERGY_DROP_RATE * static_cast<float>(dt) * m_speed;
				go->moveSpeed = HUNGRY_SPEED;
				if (go->energy >= 5.f)
					go->currState = GameObject::STATE_FULL;
				else if (go->energy < 0.f)
				{
					go->currState = GameObject::STATE_DEAD;
					go->countDown = 3.f;
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
				break;
			case GameObject::STATE_DEAD:
				go->moveSpeed = 0;
				go->countDown -= static_cast<float>(dt) * m_speed;
				if (go->countDown < 0)
				{
					go->active = false;
					--m_objectCount;
				}
				continue;
				break;
			}
		}
	}

	//Movement Section
	static const float SHARK_DIST = 10.f * m_gridSize;
	static const float FOOD_DIST = 20.f * m_gridSize;
	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			Vector3 dir = go->target - go->pos;
			if (dir.Length() < go->moveSpeed * dt * m_speed)
			{
				//GO->pos reach target
				go->pos = go->target;
				float random = Math::RandFloatMinMax(0.f, 1.f);

				if (random < 0.25f && go->moveRight)
					go->target = go->pos + Vector3(m_gridSize, 0, 0);
				else if (random < 0.5f && go->moveLeft)
					go->target = go->pos + Vector3(-m_gridSize, 0, 0);
				else if (random < 0.75f && go->moveUp)
					go->target = go->pos + Vector3(0, m_gridSize, 0);
				else if(go->moveDown)
					go->target = go->pos + Vector3(0, -m_gridSize, 0);
				if (go->target.x < 0 || go->target.x > m_noGrid * m_gridSize || go->target.y < 0 || go->target.y > m_noGrid * m_gridSize)
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
	}
} void SceneMovement::RenderGO(GameObject *go)
{
	std::ostringstream ss;
	switch(go->type)
	{
	// Ball
	case GameObject::GO_BALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);

		ss << go->id;
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;

	// Fish
	case GameObject::GO_FISH:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		if (go->currState == GameObject::STATE_TOOFULL)
			RenderMesh(meshList[GEO_TOOFULL], false);
		else if (go->currState == GameObject::STATE_FULL)
			RenderMesh(meshList[GEO_FULL], false);
		else if (go->currState == GameObject::STATE_HUNGRY)
			RenderMesh(meshList[GEO_HUNGRY], false);
		else if (go->currState == GameObject::STATE_DEAD)
			RenderMesh(meshList[GEO_DEAD], false);

		ss.precision(3);
		ss << go->energy;
		modelStack.Scale(0.5f, 0.5f, 0.5f);
		modelStack.Translate(0, -m_gridSize / 4, 0);
		RenderText(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0));
		modelStack.PopMatrix();
		break;

		// Shark
		case GameObject::GO_SHARK:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_SHARK], false);
		modelStack.PopMatrix();
		break;

		// Fish food
		case GameObject::GO_FISHFOOD:
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, zOffset);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_FISHFOOD], false);
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

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}

	//On screen text

	std::ostringstream ss;
	ss.precision(3);
	ss << "Speed:" << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 6);
	
	ss.str("");
	ss.precision(5);
	ss << "FPS:" << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 3);

	ss.str("");
	ss << "Count:" << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 9);

	ss.str("");
	ss.precision(4);
	ss << "Hour:" << m_hourOfTheDay;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 50, 12);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Movement", Color(0, 1, 0), 3, 50, 0);
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
}
