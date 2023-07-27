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


	void _Action_StopMoving(MovementComponent* MovementPtr, StateComponent* StatePtr);
	void _Action_WalkTo(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, float targetx, float targety);
	void _Action_Walk(MovementComponent* MovementPtr, StateComponent* StatePtr);
	void _Action_Attack(uint64_t AttackerID, uint64_t DefenderID);
	void _Action_DropDead(uint64_t DyingAgentID);
	void _Action_UsePotion(uint64_t UnitID);
	

private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;
	MovementSystem* m_System_Movement;
	StateSystem* m_System_State;
};
