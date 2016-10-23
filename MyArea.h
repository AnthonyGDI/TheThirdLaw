#pragma once

//#include "StdAfx.h"
#include "Entities/Helpers/NativeEntityBase.h"
#include "Log.h"

class CGameEntityNodeFactory;

////////////////////////////////////////////////////////
// Sample entity for creating a light source
////////////////////////////////////////////////////////
class CAreaEntity
	: public CGameObjectExtensionHelper<CAreaEntity, CNativeEntityBase>
{
	enum EInputPorts
	{
		eInputPort_TurnOn = 0,
		eInputPort_TurnOff
	};

	enum EOutputPorts
	{
		eOutputPort_AreaEnter = 0,
		eOutputPort_AreaLeave,
		eOutputPort_AreaEnterEntityId,
		eOutputPort_AreaLeaveEntityId
	};


	// Indices of the properties, registered in the Register function
	enum EProperties
	{
		eProperty_Active = 0,
		eProperty_Radius,
		eNumProperties
	};

public:
	std::vector<Log*> m_pLog;

	CAreaEntity();
	virtual ~CAreaEntity() {}

	// CNativeEntityBase
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~CNativeEntityBase

public:
	// Called to register the entity class and its properties
	static void Register();

	// Called when one of the input Flowgraph ports are activated in one of the entity instances tied to this class
	static void OnFlowgraphActivation(EntityId entityId, IFlowNode::SActivationInfo* pActInfo, const class CFlowGameEntityNode *pNode);

	void FindAllRocksInLevel();

protected:
	// Called on entity spawn, or when the state of the entity changes in Editor
	void Reset();
};