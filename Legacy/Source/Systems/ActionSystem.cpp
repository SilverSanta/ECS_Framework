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
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
	m_System_Movement = systemmovement;
	m_System_State = statesystem;
}
ActionSystem::~ActionSystem(){}


// DEFINITIONS
void ActionSystem::_Action_StopMoving(MovementComponent* MovementPtr, StateComponent* StatePtr)
{
	if (MovementPtr != nullptr && StatePtr != nullptr)
	{
		m_System_State->_ChangeCurrentState(StatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, StatePtr->m_CurrentDirection);
	}
}
void ActionSystem::_Action_WalkTo(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, float targetx, float targety)
{
	if (TransformPtr != nullptr && MovementPtr != nullptr && StatePtr != nullptr)
	{
		// (1) HEADING
		MovementPtr->m_NormalVector.NormalBetweenTwoPoints(TransformPtr->m_X, TransformPtr->m_Y, targetx, targety);

		// (2) Setting the target
		MovementPtr->m_Target_X = targetx;
		MovementPtr->m_Target_Y = targety;
		MovementPtr->m_bHasTarget = true;

		// (3) Walk
		_Action_Walk(MovementPtr, StatePtr);
	}
}
void ActionSystem::_Action_Walk(MovementComponent* MovementPtr, StateComponent* StatePtr)
{
	if (MovementPtr != nullptr && StatePtr != nullptr)
	{
		m_System_State->_ChangeCurrentState(StatePtr, user::State::MOVING, user::SubState::MOVING_WALKING, StatePtr->m_CurrentDirection);
	}
}
void ActionSystem::_Action_Attack(uint64_t AttackerID, uint64_t DefenderID)
{
	Entity* Attacker = m_Manager_Entities->_Get_EntityById(AttackerID);

	if (Attacker->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the attacked to ATTACKING
		StateComponent* Attacker_State = m_Manager_Components->_Get_ComponentPtrFromId(Attacker->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);
		m_System_State->_ChangeCurrentState(Attacker_State, user::State::TAKING_ACTION, user::SubState::ACTION_ATTACKING, Attacker_State->m_CurrentDirection);




		// WIP
		// WIP
		// WIP
		// WIP
		// WIP
	}
}
void ActionSystem::_Action_DropDead(uint64_t DyingAgentID)
{
	Entity* DyingAgent = m_Manager_Entities->_Get_EntityById(DyingAgentID);

	if (DyingAgent->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the dying agend to DROPPINGDEAD		
		StateComponent* DyingAgent_State = m_Manager_Components->_Get_ComponentPtrFromId(DyingAgent->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);
		m_System_State->_ChangeCurrentState(DyingAgent_State, user::State::DYING, user::SubState::DYING_NORMAL, DyingAgent_State->m_CurrentDirection);
	}
}
void ActionSystem::_Action_UsePotion(uint64_t UnitID)
{
	Entity* Unit = m_Manager_Entities->_Get_EntityById(UnitID);

	if (Unit->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
	{
		// Change state of the attacked to ATTACKING
		StateComponent* Unit_State = m_Manager_Components->_Get_ComponentPtrFromId(Unit->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);
		m_System_State->_ChangeCurrentState(Unit_State, user::State::TAKING_ACTION, user::SubState::ACTION_USING_POTION, Unit_State->m_CurrentDirection);




		// WIP
		// WIP
		// WIP
		// WIP
		// WIP
	}
}