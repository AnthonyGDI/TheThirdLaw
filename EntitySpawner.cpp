#include "EntitySpawner.h"
EntitySpawner::EntitySpawner()
{
	//SpawnEntities();
}

std::vector<IEntity*> EntitySpawner::EntitiesInRange(IEntity *entity, float range)
{
	std::vector<IEntity*> entities;
	for (int i = 0; i < spawnedEntities.size(); i++)
	{
		if (spawnedEntities[i] != entity)
		{
			if ((spawnedEntities[i]->GetWorldPos() - entity->GetWorldPos()).GetLength() <= range)
			{
				entities.push_back(spawnedEntities[i]);
			}
		}
	}
	return entities;
}

void EntitySpawner::SpawnEntities(SSystemGlobalEnvironment* gEnv)
{
	
}

void EntitySpawner::AddEntity(IEntity *entity)
{
	std::string str = entity->GetClass()->GetName();
	CryLog("Adding Entity of Type: *%s*", str);
	if (str == "Rock")
	{
		CryLog("Yup, worked");
		auto *pGameObject = gEnv->pGame->GetIGameFramework()->GetGameObject(entity->GetId());
		auto extensionId = gEnv->pGame->GetIGameFramework()->GetIGameObjectSystem()->GetID("Rock");
		auto *pRock = static_cast<CRock *>(pGameObject->QueryExtension(extensionId));
		pRock->propertyWeights = rockWeights;
		//(CRock)entity->GetClass()->GetName();
	}
	else if (str == std::string("Rock"))
	{
		CryLog("Second worked");
	}
	else
	{
		CryLog("Neither first nor second worked");
	}
	spawnedEntities.push_back(entity);

}

void EntitySpawner::PrintEntityPositions()
{
	for (int i = 0; i < spawnedEntities.size(); i++)
	{
		Vec3 temp = spawnedEntities[i]->GetWorldPos();
		CryLog("Entity %d Pos: %f, %f, %f", i+1, temp.x, temp.y, temp.z);
	}
}

void EntitySpawner::UpdateEntities()
{
	for (int i = 0; i < spawnedEntities.size(); i++)
	{
		//EntitySpawner::spawnedEntities[i]->UpdateEntity();
		auto *pGameObject = gEnv->pGame->GetIGameFramework()->GetGameObject(spawnedEntities[i]->GetId());
		auto extensionId = gEnv->pGame->GetIGameFramework()->GetIGameObjectSystem()->GetID("Rock");
		auto *pRock = static_cast<CRock *>(pGameObject->QueryExtension(extensionId));

		pRock->UpdateEntity(EntitySpawner::EntitiesInRange(spawnedEntities[i], range));
	}
}