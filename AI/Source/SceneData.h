#ifndef SCENE_DATA
#define SCENE_DATA

#include "SingletonTemplate.h"

class SceneData : public Singleton<SceneData>
{
	friend Singleton<SceneData>;
public:
	int GetObjectCount() const;
	void SetObjectCount(int objectCount);
	int GetFishCount() const;
	void SetFishCount(int fishCount);

private:
	SceneData();
	~SceneData();
	int m_objectCount;
	int m_fishCount;
};

#endif
