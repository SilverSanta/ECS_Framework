// INCLUDES
#include "SystemsManager.h"
#include "EntitiesManager.h"
#include "UserExtension.h"



// CONSTRUCTOR
LegacySystemsManager::~LegacySystemsManager() {}

// USER-DEFINED
LegacySystemsManager::LegacySystemsManager(EntitiesManager* entitiesmanager, ComponentsManager* componentsmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;

	System_Action = new ActionSystem(Manager_Components, Manager_Entities, System_Movement, System_State);
	System_Animation = new AnimationSystem(Manager_Components, Manager_Entities);
	System_Collision = new CollisionSystem(Manager_Components, Manager_Entities);
	System_Movement = new MovementSystem(Manager_Components, Manager_Entities, System_State);
	System_State = new StateSystem(Manager_Components, Manager_Entities);
	System_Render = new RenderSystem(Manager_Components, Manager_Entities);
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
}

