#pragma once
#include "AnimationSystem.h"
#include "ActionSystem.h"
#include "CollisionSystem.h"
#include "MovementSystem.h"
#include "StateSystem.h"
#include "RenderSystem.h"

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;

class LegacySystemsManager {

	//CONSTRUCTOR
public:
	LegacySystemsManager(EntitiesManager* entitiesmanager, ComponentsManager* componentsmanager);
	~LegacySystemsManager();


protected:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;

public:
	ActionSystem* m_System_Action;
	AnimationSystem* m_System_Animation;
	CollisionSystem* m_System_Collision;
	MovementSystem* m_System_Movement;
	StateSystem* m_System_State;
	RenderSystem* m_System_Render;
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
};
