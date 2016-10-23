#pragma once
#include "Entities/Helpers/NativeEntityBase.h"
#include "Property.h"

////////////////////////////////////////////////////////
// Physicalized Rock, Quinn and Kevin Rock, UROC
////////////////////////////////////////////////////////
class Log
	: public CGameObjectExtensionHelper<Log, CNativeEntityBase>
{
	enum EInputPorts
	{
		eInputPort_PropertySelector = 0
	};

	enum EOutputPors
	{
		eOutputPort_value = 0
	};

	// Indices of the properties, registered in the Register function
	enum EProperties
	{
		eProperty_Active = 0,
		eProperty_Radius,

		ePropertyGroup_PhysicalPropertiesBegin,
		eProperty_Light,
		eProperty_Mass,
		eProperty_Heat,
		eProperty_Color,
		eProperty_Size,
		eProperty_Hardness,
		eProperty_Force,
		ePropertyGroup_PhysicalPropertiesEnd,

		eNumProperties
	};

public:
	Log();
	virtual ~Log() {}
	// CNativeEntityBase
	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~CNativeEntityBase

	// Called to register the entity class and its properties
	static void Register();

	// Flowgraph helper
	void OutputFG_value(float value);

	// Called when one of the input Flowgraph ports are activated in one of the entity instances tied to this class
	static void OnFlowgraphActivation(EntityId entityId, IFlowNode::SActivationInfo* pActInfo, const class CFlowGameEntityNode *pNode);

	// ISimpleExtension	
	virtual void PostInit(IGameObject *pGameObject) override;
	virtual void HandleEvent(const SGameObjectEvent &event) override;
	// ~ISimpleExtension

	void ChangeProperty(EProperties prop, float amount);

protected:
	// Called on entity spawn, or when the state of the entity changes in Editor
	void Reset();

	// Specifies the entity geometry slot in which the light is loaded, -1 if not currently loaded
	// We default to using slot 1 for this light sample, in case the user would want to put geometry into slot 0.
	int m_logSlot;

	void Physicalize();
};