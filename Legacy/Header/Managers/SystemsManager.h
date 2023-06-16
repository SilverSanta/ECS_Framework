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
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;

public:
	ActionSystem* System_Action;
	AnimationSystem* System_Animation;
	CollisionSystem* System_Collision;
	MovementSystem* System_Movement;
	StateSystem* System_State;
	RenderSystem* System_Render;
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
	// USER DEFINED
};
