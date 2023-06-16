#pragma once

#include "mfk.h"
#include "UserNamespace.h"

// FORWARD DECLARATIONS
class Entity;
class ComponentsManager;
struct SDL_Renderer;

class IComponent {

public:
	IComponent(){}
	~IComponent(){}

	// VIRTUALS
public:
	virtual void _Update(uint32_t currentTime) = 0;
	virtual void _Render(SDL_Renderer* renderer) = 0;
	virtual void _DeleteFromRepository() = 0;
	
	
	// GETTERS
	uint64_t Get_OwnerId()
	{
		return m_OwnerId;
	}

	// COMPONENT DATA
public:
	uint64_t m_Id;
protected:	
	user::ComponentType m_Type;
	uint64_t m_OwnerId;
	ComponentsManager* m_ComponentsManager;


	// FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
};

