#pragma once
#include <iostream>

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class TransformComponent;
class StateComponent;
class MovementComponent;
class MovementSystem;
class StateSystem;


class ActionSystem
{
public:
	ActionSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, MovementSystem* systemmovement, StateSystem* statesystem);
	~ActionSystem();


	void Action_StopMoving(MovementComponent* MovementPtr, StateComponent* StatePtr);
	void Action_WalkTo(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, float targetx, float targety);
	void Action_Walk(MovementComponent* MovementPtr, StateComponent* StatePtr);
	void Action_Attack(uint64_t AttackerID, uint64_t DefenderID);
	void Action_DropDead(uint64_t DyingAgentID);
	void Action_UsePotion(uint64_t UnitID);
	

private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
	MovementSystem* System_Movement;
	StateSystem* System_State;
};
