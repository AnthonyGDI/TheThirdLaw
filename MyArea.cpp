#include "StdAfx.h"
#include "MyArea.h"

#include "Game/GameFactory.h"

#include "FlowNodes/Helpers/FlowGameEntityNode.h"

#include <CryAnimation/ICryAnimation.h>
#include <CryEntitySystem/IEntityProxy.h>
#include <IActorSystem.h>

class CAreaRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		CAreaEntity::Register();
	}
};

CAreaRegistrator g_area2Registrator;

CAreaEntity::CAreaEntity()
{
}

void CAreaEntity::ProcessEvent(SEntityEvent& event)
{
	if (gEnv->IsDedicated())
		return;

	switch (event.event)
	{
		// Physicalize on level start for Launcher
	case ENTITY_EVENT_START_LEVEL:
		// Editor specific, physicalize on reset, property change or transform change
	case ENTITY_EVENT_RESET:
	case ENTITY_EVENT_EDITOR_PROPERTY_CHANGED:
	case ENTITY_EVENT_XFORM_FINISHED_EDITOR:
		Reset();
		break;
	case ENTITY_EVENT_ENTERAREA:
	{

		IEntity * pEntity = gEnv->pEntitySystem->GetEntity((EntityId)event.nParam[0]);
		IActor* pActor = gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor((EntityId)event.nParam[0]);
		EntityId PlayerId = gEnv->pGame->GetIGameFramework()->GetClientActorId();
		IActor* pPlayerAc = gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor(PlayerId);
		if (pActor == pPlayerAc)
		{
			gEnv->pLog->Log("%s: Player Entered the MyArea", GetEntity()->GetName());

			TFlowInputData data;
			data.SetUserFlag(true);
			ActivateFlowNodeOutput(eOutputPort_AreaEnter, data);

			TFlowInputData dataNum;
			dataNum.SetUserFlag(true);
			dataNum.Set<EntityId>(static_cast<EntityId>(event.nParam[0]));
			ActivateFlowNodeOutput(eOutputPort_AreaEnterEntityId, dataNum);

		}

		if (pActor != pPlayerAc)
		{
			gEnv->pLog->Log("%s: Something entered the MyArea", GetEntity()->GetName());
		}

		break;
	}
	case ENTITY_EVENT_LEAVEAREA:
	{
		IEntity * pEntity = gEnv->pEntitySystem->GetEntity((EntityId)event.nParam[0]);
		IActor* pActor = gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor((EntityId)event.nParam[0]);
		EntityId PlayerId = gEnv->pGame->GetIGameFramework()->GetClientActorId();
		IActor* pPlayerAc = gEnv->pGame->GetIGameFramework()->GetIActorSystem()->GetActor(PlayerId);
		if (pActor == pPlayerAc)
		{
			gEnv->pLog->Log("%s: Player Left the MyArea", GetEntity()->GetName());

			TFlowInputData data;
			data.SetUserFlag(true);
			ActivateFlowNodeOutput(eOutputPort_AreaLeave, data);

			TFlowInputData dataNum;
			dataNum.SetUserFlag(true);
			dataNum.Set<EntityId>(static_cast<EntityId>(event.nParam[0]));
			ActivateFlowNodeOutput(eOutputPort_AreaLeaveEntityId, dataNum);

		}

		if (pActor != pPlayerAc)
		{
			gEnv->pLog->Log("%s: Something left the MyArea", GetEntity()->GetName());
		}
		break;
	}
	}
}

void CAreaEntity::FindAllRocksInLevel()
{
	m_pLog.clear();
	// We only handle default spawning below for the Launcher
	// Editor has special logic in CEditorGame
	if (gEnv->IsEditor())
		return;

	// Spawn at first default spawner
	auto *pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	auto *pRockClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass("Log");
	auto extensionId = gEnv->pGame->GetIGameFramework()->GetIGameObjectSystem()->GetID("Log");

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pEntity = pEntityIterator->Next();

		if (pEntity->GetClass() != pRockClass)
			continue;

		auto *pGameObject = gEnv->pGame->GetIGameFramework()->GetGameObject(pEntity->GetId());
		if (pGameObject == nullptr)
			continue;

		auto *pRock = static_cast<Log *>(pGameObject->QueryExtension(extensionId));
		if (pRock == nullptr)
			continue;

		// Adding finded rock into array for future use in game 
		m_pLog.push_back(pRock);

		break;
	}
}

void CAreaEntity::Reset()
{
	CAreaEntity::FindAllRocksInLevel();
	IEntity &entity = *GetEntity();

	// Check if the light is active
	if (!GetPropertyBool(eProperty_Active))
		return;

	IEntityAreaProxy *pTriggerProxy = (IEntityAreaProxy*)(GetEntity()->GetProxy(ENTITY_PROXY_AREA));

	if (!pTriggerProxy)
	{
		GetEntity()->CreateProxy(ENTITY_PROXY_AREA);
		pTriggerProxy = (IEntityAreaProxy*)GetEntity()->GetProxy(ENTITY_PROXY_AREA);
	}

	if (pTriggerProxy)
	{
		float radius = GetPropertyFloat(eProperty_Radius) * 0.5f;
		AABB boundingBox = AABB(Vec3(-radius, -radius, -radius), Vec3(radius, radius, radius));
		pTriggerProxy->SetBox(Vec3(-radius, -radius, -radius), Vec3(radius, radius, radius), nullptr, 0);
		pTriggerProxy->AddEntity(GetEntity()->GetId());
		for (int i = 0; i < m_pLog.size(); i++)
		{
			pTriggerProxy->AddEntity(m_pLog[i]->GetEntityId());
			CryLog("MyArea.adding entity id: %i", (int)m_pLog[i]->GetEntityId());
		}
	}
	else
	{
		gEnv->pLog->Log("%s: Warning: Area Has Bad Params", GetEntity()->GetName());
	}
}

// Static function
void CAreaEntity::Register()
{
	auto properties = new SNativeEntityPropertyInfo[eNumProperties];
	memset(properties, 0, sizeof(SNativeEntityPropertyInfo) * eNumProperties);

	RegisterEntityProperty<bool>(properties, eProperty_Active, "Active", "1", "", 0, 1);

	RegisterEntityProperty<float>(properties, eProperty_Radius, "Radius", "4", "", 0.1f, 100.f);

	// Finally, register the entity class so that instances can be created later on either via Launcher or Editor
	CGameFactory::RegisterNativeEntity<CAreaEntity>("MyArea", "ThirdLawEntities", "Trigger.bmp", 0u, properties, eNumProperties);

	// Create flownode
	CGameEntityNodeFactory &nodeFactory = CGameFactory::RegisterEntityFlowNode("MyArea");

	// Define input ports, and the callback function for when they are triggered
	std::vector<SInputPortConfig> inputs;
	inputs.push_back(InputPortConfig_Void("TurnOn", "Turns the Area on"));
	inputs.push_back(InputPortConfig_Void("TurnOff", "Turns the Area off"));
	nodeFactory.AddInputs(inputs, OnFlowgraphActivation);

	std::vector<SOutputPortConfig> outputs;
	outputs.push_back(OutputPortConfig_Void("AreaEnter", "If player enter to area"));
	outputs.push_back(OutputPortConfig_Void("AreaLeave", "If player leave area"));
	outputs.push_back(OutputPortConfig<int>("AreaEnterEntityId", "Entity id enter area"));
	outputs.push_back(OutputPortConfig<int>("AreaLeaveEntityId", "Entity id leave area"));
	//outputs.push_back(OutputPortConfig_AnyType("IsActive", "If player leave area"));
	nodeFactory.AddOutputs(outputs);


	// Mark the factory as complete, indicating that there will be no additional ports
	nodeFactory.Close();
}

void CAreaEntity::OnFlowgraphActivation(EntityId entityId, IFlowNode::SActivationInfo *pActInfo, const class CFlowGameEntityNode *pNode)
{
	if (auto *pLightSource = static_cast<CAreaEntity *>(QueryExtension(entityId)))
	{
		if (IsPortActive(pActInfo, eInputPort_TurnOn))
		{
			//pLightSource->SetPropertyBool(eProperty_Active, true);
		}
		else if (IsPortActive(pActInfo, eInputPort_TurnOff))
		{
			//pLightSource->SetPropertyBool(eProperty_Active, false);
		}
	}
}