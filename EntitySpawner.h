#pragma once
#include "StdAfx.h"
#include "Game\Game.h"
#include "Rock.h"
#include <vector>

class EntitySpawner
{
	std::vector<IEntity*> spawnedEntities;
	float range = 1.5;
	//Setup Weights For Entities
	std::vector<float> rockWeights = { (float)(rand() % 4 + 1), (float)(rand() % 4 + 1), (float)(rand() % 4 + 1), (float)(rand() % 4 + 1), (float)(rand() % 4 + 1), (float)(rand() % 4 + 1) };

public:
	EntitySpawner();
	void AddEntity(IEntity *entity);
	void SpawnEntities(SSystemGlobalEnvironment* gEnv);
	void UpdateEntities();//Called once per frame
	std::vector<IEntity*> EntitiesInRange(IEntity *entity, float range);
	void PrintEntityPositions();
};