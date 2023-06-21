#pragma once

class MyFramework;


// LEGACY INCLUDES
#include "ComponentsManager.h"
#include "SystemsManager.h"
#include "UserNamespace.h"

// USER COMPONENTS
#include "TestComponent.h"
#include "CameraComponent.h"

// USER SYSTEMS
#include "TestSystem.h"
#include "CameraSystem.h"


// USER COMPONENTS MANAGER
class ComponentsManager :public LegacyComponentsManager
{
public:
	ComponentsManager(uint16_t maxentities) : LegacyComponentsManager(maxentities)
	{
		m_MaxEntities = maxentities;

		Components_Test.reserve(m_MaxEntities);
		Components_Camera.reserve(m_MaxEntities);
		// USER-DEFINED		
		// USER-DEFINED		
		// USER-DEFINED		
		// USER-DEFINED		
		// USER-DEFINED		
		// USER-DEFINED		
		// USER-DEFINED		
	}

	template <class T>
	T* _CreateComponent(Entity* owner, std::vector<T>& TComponents, user::ComponentType comptype)
	{
		// (1) Check if this is available in entity;		
		uint64_t ComponentID = owner->GenerateComponentId(comptype);
		bool bIsAvailable = (owner->Get_ComponentIdFromEntityByType(comptype) == NULL) ? (true) : (false);

		// (2) if available - emplace in repository, then create an interfaced pointer to that location in memory 
		if (bIsAvailable)
		{
			T* NewComponent = new T;
			IComponent* NewComponentInterfaced = static_cast<IComponent*>(NewComponent);

			NewComponentInterfaced->m_OwnerId = owner->Get_EntityId();
			NewComponentInterfaced->m_Id = ComponentID;
			NewComponentInterfaced->m_ComponentsManager = this;
			NewComponentInterfaced->m_Type = comptype;

			// (3) Place in the repository
			TComponents.emplace_back(*NewComponent);
			owner->AddComponentToEntity(NewComponentInterfaced);

			T* ValidPtr = this->Get_ComponentPtrFromId(ComponentID, TComponents);
			return ValidPtr;
		}
		else
		{
			return nullptr;
		}
	}	

	void _DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted) override;
	void UpdateRepositories(uint64_t currenttime) override;

public:
	std::vector<TestComponent> Components_Test;
	std::vector<CameraComponent> Components_Camera;
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
};

// USER SYSTEMS MANAGER
class SystemsManager : public LegacySystemsManager
{
public:
	SystemsManager(EntitiesManager* entitiesmanager, ComponentsManager* componentsmanager);
	~SystemsManager(){}

public:
	TestSystem* System_Test;
	CameraSystem* System_Camera;
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
	// USED-DEFINED
};

// USER BLUEPRINTS
class UserBlueprints
{
public:
	UserBlueprints(MyFramework* game, ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~UserBlueprints();

	// GLOBAL
	uint64_t Create_MouseCursor();
	uint64_t Create_Camera(float cameraspeed, float limitsleft, float limitsright, float limitsup, float limitsdown);

	// IN-GAME
	uint64_t Create_Goblin(float coordinates_x, float coordinates_y, std::string filepath, std::string filepathhealthbar);
	uint64_t Create_Grid(float coordinates_x, float coordinates_y, int cellson_x, int cellson_y, int cellwidth, int cellheight);
	uint64_t Create_Tile(float coordinates_x, float coordinates_y, std::string filepath);
	
	// MENU
	uint64_t Create_Button(float coordinates_x, float coordinates_y, int buttonwwidth, int buttonheight, int spritewidth, int spriteheight, std::string filepath);
	uint64_t Create_Background(float coordinates_x, float coordinates_y, user::Resolution resolution, std::string incompletefilepath, std::unordered_map < user::Resolution, std::pair<uint16_t, uint16_t>> mapper_resolution);
	

private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
	MyFramework* Game;
};









