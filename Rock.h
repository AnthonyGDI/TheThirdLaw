#pragma once
#include "Entities/Helpers/NativeEntityBase.h"
#include "Property.h"

////////////////////////////////////////////////////////
// Physicalized Rock, Quinn and Kevin Rock, UROC
////////////////////////////////////////////////////////
class CRock
	: public CGameObjectExtensionHelper<CRock, CNativeEntityBase>
{
public:
	std::vector<float> propertyValues = { 1, 1, 1, 1, 1, 1 };
	std::vector<float> propertyWeights;
	void UpdateEntity(std::vector<IEntity*> entitiesInRange);
	void AffectEntity();
	virtual ~CRock() {}
	//std::vector<std::vector<Property>> propertyWeights;
	// ISimpleExtension	
	virtual void PostInit(IGameObject *pGameObject) override;
	virtual void HandleEvent(const SGameObjectEvent &event) override;
	// ~ISimpleExtension

protected:
	void Physicalize();
};