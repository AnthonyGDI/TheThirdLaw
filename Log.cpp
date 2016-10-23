#include "StdAfx.h"
#include "Log.h"

#include "Game/GameFactory.h"

#include "FlowNodes/Helpers/FlowGameEntityNode.h"

class LogRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		Log::Register();
		RegisterCVars();
	}

	void RegisterCVars()
	{
		m_pGeometry = gEnv->pConsole->RegisterString("w_bulletGeometry", "Objects/Default/primitive_sphere.cgf", VF_CHEAT, "Sets the path to the geometry assigned to every weapon bullet");
	}

public:
	ICVar *m_pGeometry;

	float m_mass;
	float m_initialVelocity;
};

LogRegistrator g_bulletRegistrator;


Log::Log()
// Start by setting the light slot to an invalid value by default
	: m_logSlot(-1)
{
}

void Log::ProcessEvent(SEntityEvent& event)
{
	if (gEnv->IsDedicated())
		return;
	//Debug switch
	switch (event.event)
	{
	//! Sent when triggering entity enters to the area proximity, this event sent to all target entities of the area.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area
	case ENTITY_EVENT_ENTERAREA:
		CryLog("Entity_event_enterArea");
		break;

	//! Sent when triggering entity leaves the area proximity, this event sent to all target entities of the area.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area
	case ENTITY_EVENT_LEAVEAREA:
		CryLog("Entity_event_leaveArea");
		break;
	//! Sent when triggering entity is near to the area proximity, this event sent to all target entities of the area.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area
	case ENTITY_EVENT_ENTERNEARAREA:
		CryLog("Entity_event_enterNearArea");
		break;
	//! Sent when triggering entity leaves the near area within proximity region of the outside area border.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area
	case ENTITY_EVENT_LEAVENEARAREA:
		CryLog("Entity_event_leaveNearArea");
		break;
	//! Sent when triggering entity moves inside the area within proximity region of the outside area border.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area
	case ENTITY_EVENT_MOVEINSIDEAREA:
		CryLog("Entity_event_moveInsideArea");
		break;
	//! Sent when triggering entity moves inside the area within the near region of the outside area border.
	//! nParam[0] = TriggerEntityId, nParam[1] = AreaId, nParam[2] = EntityId of Area, fParam[0] = FadeRatio (0-1)
	case ENTITY_EVENT_MOVENEARAREA:
		CryLog("Entity_event_MoveNearArea");
		break;
	//! Sent when triggering entity enters or leaves an area so all active areas of same group get notified. This event is sent to all target entities of the area.
	case ENTITY_EVENT_CROSS_AREA:
		CryLog("Entity_event_Cross_Area");
		break;
	}
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
	}
}

void Log::Reset()
{
	IEntity &entity = *GetEntity();

	// Check if we have to unload first
	if (m_logSlot != -1)
	{
		entity.FreeSlot(m_logSlot);
		m_logSlot = -1;
	}
	
	// Check if the light is active
	if (!GetPropertyBool(eProperty_Active))
		return;
}

// Static function
void Log::Register()
{
	auto properties = new SNativeEntityPropertyInfo[eNumProperties];
	memset(properties, 0, sizeof(SNativeEntityPropertyInfo) * eNumProperties);

	RegisterEntityProperty<bool>(properties, eProperty_Active, "Active", "1", "", 0, 1);

	RegisterEntityProperty<float>(properties, eProperty_Radius, "Radius", "10", "", 0, 100000.f);

	{
		ENTITY_PROPERTY_GROUP("PhysicalProperties", ePropertyGroup_PhysicalPropertiesBegin, ePropertyGroup_PhysicalPropertiesEnd, properties);

		RegisterEntityProperty<float>(properties, eProperty_Light, "Light", "1", "Description of light", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Mass, "Mass", "1", "Description of mass", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Heat, "Heat", "1", "Description of heat", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Color, "Color", "1", "Description of color", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Size, "Size", "1", "Description of size", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Hardness, "Hardness", "1", "Description of hardness", 0, 100.f);
		RegisterEntityProperty<float>(properties, eProperty_Force, "Force", "1", "Description of force", 0, 100.f);
	}

	// Finally, register the entity class so that instances can be created later on either via Launcher or Editor
	CGameFactory::RegisterNativeEntity<Log>("Log", "ThirdLawEntities", "Light.bmp", 0u, properties, eNumProperties);

	// Create flownode
	CGameEntityNodeFactory &nodeFactory = CGameFactory::RegisterEntityFlowNode("Log");

	// Define input ports, and the callback function for when they are triggered
	std::vector<SInputPortConfig> inputs;

	inputs.push_back(InputPortConfig<int>("PropertySelector", "Selects which property to output"));
	nodeFactory.AddInputs(inputs, OnFlowgraphActivation);

	// Define output ports, and the callback function for when they are triggered
	std::vector<SOutputPortConfig> outputs;

	outputs.push_back(OutputPortConfig<float>("Value", "Selected Physical Property's value"));
	nodeFactory.AddOutputs(outputs);

	// Mark the factory as complete, indicating that there will be no additional ports
	nodeFactory.Close();
}

void Log::OutputFG_value(float value)
{
	TFlowInputData data;
	data.SetUserFlag(true);
	data.Set<float>(value);
	ActivateFlowNodeOutput(eOutputPort_value, data);

	//Just to change values...
	this->ChangeProperty(eProperty_Mass, 1);
	this->ChangeProperty(eProperty_Color, 1.1);
	this->ChangeProperty(eProperty_Force, 1.2);
	this->ChangeProperty(eProperty_Hardness, -.1);
	this->ChangeProperty(eProperty_Heat, -.001);
	this->ChangeProperty(eProperty_Light, 1.3);
	this->ChangeProperty(eProperty_Size, 1.5);
}

void Log::OnFlowgraphActivation(EntityId entityId, IFlowNode::SActivationInfo *pActInfo, const class CFlowGameEntityNode *pNode)
{
	if (auto *pLightSource = static_cast<Log *>(QueryExtension(entityId)))
	{
		if (IsPortActive(pActInfo, eInputPort_PropertySelector))
		{
			int value;
			TFlowInputData* data1 = pActInfo->GetInputPort(eInputPort_PropertySelector);
			int* i = data1->GetPtr<int>();
			value = *i; 
			float outputV;
			switch (value)
			{
			case 0:
				CryLog("input int was 0");
				outputV = pLightSource->GetPropertyFloat(eProperty_Light);
				break;
			case 1:
				CryLog("input int was 1");
				outputV = pLightSource->GetPropertyFloat(eProperty_Mass);
				break;
			case 2:
				CryLog("input int was 2");
				outputV = pLightSource->GetPropertyFloat(eProperty_Heat);
				break;
			case 3:
				CryLog("input int was 3");
				outputV = pLightSource->GetPropertyFloat(eProperty_Color);
				break;
			case 4:
				CryLog("input int was 4");
				outputV = pLightSource->GetPropertyFloat(eProperty_Size);
				break;
			case 5:
				CryLog("input int was 5");
				outputV = pLightSource->GetPropertyFloat(eProperty_Hardness);
				break;
			case 6:
				CryLog("input int was 6");
				outputV = pLightSource->GetPropertyFloat(eProperty_Force);
				break;
			}
			pLightSource->OutputFG_value(outputV); //I chose 0, but I want to use the input value
		}
	}
}

void Log::ChangeProperty(EProperties prop, float amount)
{
	this->SetPropertyFloat(prop, (this->GetPropertyFloat(prop) + amount));
}

void Log::PostInit(IGameObject *pGameObject)
{
	// Make sure we get logged collision events
	// Note the difference between immediate (directly on the physics thread) and logged (delayed to run on main thread) physics events.
	pGameObject->EnablePhysicsEvent(true, eEPE_OnCollisionLogged);

	const int requiredEvents[] = { eGFE_OnCollision };
	pGameObject->UnRegisterExtForEvents(this, NULL, 0);
	pGameObject->RegisterExtForEvents(this, requiredEvents, sizeof(requiredEvents) / sizeof(int));

	// Set the model
	const int geometrySlot = 0;
	
	LoadMesh(geometrySlot, g_bulletRegistrator.m_pGeometry->GetString());

	// Load the custom bullet material.
	// This material has the 'mat_bullet' surface type applied, which is set up to play sounds on collision with 'mat_default' objects in Libs/MaterialEffects
	auto *pBulletMaterial = gEnv->p3DEngine->GetMaterialManager()->LoadMaterial("Materials/bullet");
	GetEntity()->SetMaterial(pBulletMaterial);

	// Now create the physical representation of the entity
	Physicalize();

	// Make sure that bullets are always rendered regardless of distance
	if (auto *pRenderProxy = static_cast<IEntityRenderProxy *>(GetEntity()->GetProxy(ENTITY_PROXY_RENDER)))
	{
		// Ratio is 0 - 255, 255 being 100% visibility
		pRenderProxy->SetViewDistRatio(255);
	}

	// Apply an impulse so that the bullet flies forward
	if (auto *pPhysics = GetEntity()->GetPhysics())
	{
		pe_action_impulse impulseAction;

		// Set the actual impulse, in this cause the value of the initial velocity CVar in bullet's forward direction
		impulseAction.impulse = GetEntity()->GetWorldRotation().GetColumn1() * g_bulletRegistrator.m_initialVelocity;

		// Send to the physical entity
		pPhysics->Action(&impulseAction);
	}
}

void Log::HandleEvent(const SGameObjectEvent &event)
{
	// Handle the OnCollision event, in order to have the entity removed on collision
	if (event.event == eGFE_OnCollision)
	{
		EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.ptr);
		//CryLog("My ID: %f", GetEntityId());
		//CryLog("idCollider: %f", physCollision->idCollider);
		CryLog("You shot me!");
		// Collision info can be retrieved using the event pointer
		//EventPhysCollision *physCollision = reinterpret_cast<EventPhysCollision *>(event.ptr);

		// Queue removal of this entity, unless it has already been done
		//gEnv->pEntitySystem->RemoveEntity(GetEntityId());
	}
}
void Log::Physicalize()
{
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_RIGID;

	//physParams.mass = g_bulletRegistrator.m_mass;

	GetEntity()->Physicalize(physParams);
}