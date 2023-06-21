#pragma once

#include "ActionSystem.h"

#include "EntitiesManager.h"
#include "UserExtension.h"
#include "TransformComponent.h"
#include "StateComponent.h"
#include "MovementComponent.h"
#include "MovementSystem.h"
#include "StateSystem.h"


// CONSTRUCTORS
ActionSystem::ActionSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, MovementSystem* systemmovement, StateSystem* statesystem)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
	System_Movement = systemmovement;
	System_State = statesystem;
}
ActionSystem::~ActionSystem(){}


// DEFINITIONS
void ActionSystem::Action_StopMoving(MovementComponent* MovementPtr, StateComponent* StatePtr)
{
	if (MovementPtr != nullptr && StatePtr != nullptr)
	{
		System_State->ChangeCurrentState(StatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, StatePtr->m_CurrentDirection);
	}
}
void ActionSystem::Action_WalkTo(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, float targetx, float targety)
{
	if (TransformPtr != nullptr && MovementPtr != nullptr && StatePtr != nullptr)
	{
		// (1) HEADING
		MovementPtr->m_NormalVector.NormalBetweenTwoPoints(TransformPtr->X, TransformPtr->Y, targetx, targety);

		// (2) Setting the target
		MovementPtr->Target_X = targetx;
		MovementPtr->Target_Y = targety;
		MovementPtr->bHasTarget = true;

		// (3) Walk
		Action_Walk(MovementPtr, StatePtr);
	}
}
void ActionSystem::Action_Walk(MovementComponent* MovementPtr, StateComponent* StatePtr)
{
	if (MovementPtr != nullptr && StatePtr != nullptr)
	{
		System_State->ChangeCurrentState(StatePtr, user::State::MOVING, user::SubState::MOVING_WALKING, StatePtr->m_CurrentDirection);
	}
}
void ActionSystem::Action_Attack(uint64_t AttackerID, uint64_t DefenderID)
{
	Entity* Attacker = Manager_Entities->Get_EntityById(AttackerID);

	if (Attacker->Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the attacked to ATTACKING
		StateComponent* Attacker_State = Manager_Components->Get_ComponentPtrFromId(Attacker->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);
		System_State->ChangeCurrentState(Attacker_State, user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, Attacker_State->m_CurrentDirection);




		// WIP
		// WIP
		// WIP
		// WIP
		// WIP
	}
}
void ActionSystem::Action_DropDead(uint64_t DyingAgentID)
{
	Entity* DyingAgent = Manager_Entities->Get_EntityById(DyingAgentID);

	if (DyingAgent->Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the dying agend to DROPPINGDEAD		
		StateComponent* DyingAgent_State = Manager_Components->Get_ComponentPtrFromId(DyingAgent->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);
		System_State->ChangeCurrentState(DyingAgent_State, user::State::DYING, user::SubState::DYING_NORMAL, DyingAgent_State->m_CurrentDirection);
	}
}
void ActionSystem::Action_UsePotion(uint64_t UnitID)
{
	Entity* Unit = Manager_Entities->Get_EntityById(UnitID);

	if (Unit->Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the attacked to ATTACKING
		StateComponent* Unit_State = Manager_Components->Get_ComponentPtrFromId(Unit->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);
		System_State->ChangeCurrentState(Unit_State, user::State::TAKING_ACTION, user::SubState::ACTION_USING_POTION, Unit_State->m_CurrentDirection);




		// WIP
		// WIP
		// WIP
		// WIP
		// WIP
	}
}