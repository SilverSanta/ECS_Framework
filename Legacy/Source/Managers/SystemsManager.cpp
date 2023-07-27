// INCLUDES
#include "SystemsManager.h"
#include "EntitiesManager.h"
#include "UserExtension.h"



// CONSTRUCTOR
LegacySystemsManager::~LegacySystemsManager() {}

// USER-DEFINED
LegacySystemsManager::LegacySystemsManager(EntitiesManager* entitiesmanager, ComponentsManager* componentsmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;

	m_System_Action = new ActionSystem(m_Manager_Components, m_Manager_Entities, m_System_Movement, m_System_State);
	m_System_Animation = new AnimationSystem(m_Manager_Components, m_Manager_Entities);
	m_System_Collision = new CollisionSystem(m_Manager_Components, m_Manager_Entities);
	m_System_Movement = new MovementSystem(m_Manager_Components, m_Manager_Entities, m_System_State);
	m_System_State = new StateSystem(m_Manager_Components, m_Manager_Entities);
	m_System_Render = new RenderSystem(m_Manager_Components, m_Manager_Entities);
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
}

