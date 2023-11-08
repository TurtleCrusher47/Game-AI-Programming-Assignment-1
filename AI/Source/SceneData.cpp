#include "SceneData.h"

int SceneData::GetObjectCount() const
{
	return m_objectCount;
}

void SceneData::SetObjectCount(int objectCount)
{
	m_objectCount = objectCount;
}

int SceneData::GetFishCount() const
{
	return m_fishCount;
}

void SceneData::SetFishCount(int fishCount)
{
	m_fishCount = fishCount;
}

SceneData::SceneData()
{
}

SceneData::~SceneData()
{
}
