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

		m_Components_Test.reserve(m_MaxEntities);
		m_Components_Camera.reserve(m_MaxEntities);
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
		uint64_t ComponentID = owner->_GenerateComponentId(comptype);
		bool bIsAvailable = (owner->_Get_ComponentIdFromEntityByType(comptype) == NULL) ? (true) : (false);

		// (2) if available - emplace in repository, then create an interfaced pointer to that location in memory 
		if (bIsAvailable)
		{
			T* NewComponent = new T;
			IComponent* NewComponentInterfaced = static_cast<IComponent*>(NewComponent);

			NewComponentInterfaced->m_OwnerId = owner->_Get_EntityId();
			NewComponentInterfaced->m_Id = ComponentID;
			NewComponentInterfaced->m_ComponentsManager = this;
			NewComponentInterfaced->m_Type = comptype;

			// (3) Place in the repository
			TComponents.emplace_back(*NewComponent);
			owner->_AddComponentToEntity(NewComponentInterfaced);

			delete NewComponent;

			T* ValidPtr = this->_Get_ComponentPtrFromId(ComponentID, TComponents);
			return ValidPtr;
		}
		else
		{
			return nullptr;
		}
	}	

	void _DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted) override;
	void _UpdateRepositories(uint64_t currenttime) override;

public:
	std::vector<TestComponent> m_Components_Test;
	std::vector<CameraComponent> m_Components_Camera;
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
	TestSystem* m_System_Test;
	CameraSystem* m_System_Camera;
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
	uint64_t _Create_MouseCursor();
	uint64_t _Create_Camera(float cameraspeed, float limitsleft, float limitsright, float limitsup, float limitsdown);

	// IN-GAME
	uint64_t _Create_Goblin(float coordinates_x, float coordinates_y, std::string filepath, std::string filepathhealthbar);
	uint64_t _Create_Grid(float coordinates_x, float coordinates_y, int cellson_x, int cellson_y, int cellwidth, int cellheight);
	uint64_t _Create_Tile(float coordinates_x, float coordinates_y, std::string filepath);
	
	// MENU
	uint64_t _Create_Button(float coordinates_x, float coordinates_y, int buttonwwidth, int buttonheight, int spritewidth, int spriteheight, std::string filepath);
	uint64_t _Create_Background(float coordinates_x, float coordinates_y, user::Resolution resolution, std::string incompletefilepath, std::unordered_map < user::Resolution, std::pair<uint16_t, uint16_t>> mapper_resolution);
	uint64_t _Create_Text(float coordinates_x, float coordinates_y, bool cameratransformable, int textheight, const char* text, const char* fontpath, user::Alignment position, mfk::ColourList colour, user::Layer layer);
	


private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;
	MyFramework* Game;
};









