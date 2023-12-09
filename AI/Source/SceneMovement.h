#ifndef SCENE_MOVEMENT_H
#define SCENE_MOVEMENT_H

#include <map>
#include <vector>
#include <queue>
#include "GameObject.h"
#include "SceneBase.h"
#include "ObjectBase.h"
#include "StateMachine.h"

class SceneMovement : public SceneBase, public ObjectBase
{
public:
	SceneMovement();
	~SceneMovement();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);
	bool Handle(Message* message);

	GameObject* FetchGO(GameObject::GAMEOBJECT_TYPE type);
protected:
	void InitStateMachines();
	void ProcessMessages();
	void RenderLine(const Vector3& start, const Vector3& end);

	//week 5
	//make a map of statemachines that will be shared by every object in golist. reduces memory footprint (tho admittedly not very useful in a scene this small?).
	std::map<GameObject::GAMEOBJECT_TYPE, StateMachine*> m_stateMachines;
	std::queue<Message *> m_messageQueue;

	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;

	float m_hourOfTheDay;
	int m_numGO[GameObject::GO_TOTAL];
	float zOffset;
};

#endif