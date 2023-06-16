#pragma once

#include "EntitiesManager.h"
#include "UserExtension.h"
#include "StateSystem.h"


StateSystem::StateSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
}
StateSystem::~StateSystem() {}

void StateSystem::_UpdateState_AllPreviousToCurrent()
{
	for (auto& State : Manager_Components->Components_State)
	{
		UpdateState_PreviousToCurrent(&State);
	}
}
void StateSystem::_CheckForDead_All()
{
	std::vector<uint64_t> ToBeDeleted;

	for (auto& State : Manager_Components->Components_State)
	{
		if (State.m_CurrentState == user::State::DEAD)
		{
			if (State.m_Time_Current - State.m_Time_StartOfCurrentState >= State.m_Time_Dead * 1000)
			{
				ToBeDeleted.push_back(State.Get_OwnerId());
			}
		}
	}
	for (auto& id : ToBeDeleted)
	{
		Manager_Components->_DeleteComponents(Manager_Entities->_DeleteEntity(Manager_Entities->Get_EntityById(id)));
	}
}
void StateSystem::ChangeCurrentState(StateComponent* stateptr, user::State newstate, user::SubState newsubstate, user::Direction newdirection)
{
	if (stateptr->m_CurrentState != newstate || stateptr->m_CurrentSubState != newsubstate || stateptr->m_CurrentDirection != newdirection)
	{
		if (CheckForInterruptability(newstate, stateptr->m_CurrentState) == true)
		{
			Set_State(stateptr, newstate);
			Set_SubState(stateptr, newsubstate);
			Set_Direction(stateptr, newdirection);

			stateptr->m_Time_StartOfCurrentState = stateptr->m_Time_Current;
		}
	}
}
void StateSystem::ReturnToIdle(StateComponent* stateptr)
{
	// (1) Forcing the state change (bypassing CheckForInterruptibility)
	stateptr->m_CurrentState = user::State::IDLE;

	Set_State(stateptr, user::State::IDLE);
	Set_SubState(stateptr, user::SubState::IDLE_STANDING);
	Set_Direction(stateptr, stateptr->m_CurrentDirection);

	stateptr->m_Time_StartOfCurrentState = stateptr->m_Time_Current;
}
void StateSystem::Set_State(StateComponent* StatePtr, user::State state)
{
	if (CheckForInterruptability(state, StatePtr->m_CurrentState) == true)
	{
		StatePtr->m_StateFromPreviousFrame = StatePtr->m_CurrentState;
		StatePtr->m_CurrentState = state;
	}
}
void StateSystem::Set_SubState(StateComponent* StatePtr, user::SubState substate)
{
	if ((int)substate * 1000 >= (int)StatePtr->m_CurrentState * 10000 && (int)substate * 1000 < ((int)StatePtr->m_CurrentState + 1) * 10000)
	{
		StatePtr->m_CurrentSubState = substate;
	}
}
void StateSystem::Set_Direction(StateComponent* StatePtr, user::Direction direction)
{
	if (StatePtr->m_CurrentState == user::State::IDLE || StatePtr->m_CurrentState == user::State::MOVING)
	{
		switch (direction)
		{
		case user::Direction::Facing_RIGHT:
			StatePtr->m_CurrentDirection = user::Direction::Facing_RIGHT;
			break;
		case user::Direction::Facing_LEFT:
			StatePtr->m_CurrentDirection = user::Direction::Facing_LEFT;
			break;
		case user::Direction::Facing_DOWN:
			StatePtr->m_CurrentDirection = user::Direction::Facing_DOWN;
			break;
		case user::Direction::Facing_UP:
			StatePtr->m_CurrentDirection = user::Direction::Facing_UP;
			break;
		}
	}
}
bool StateSystem::CheckForInterruptability(user::State newstate, user::State currentstate)
{// We are using the order of enum class State		
	if ((int)currentstate <= 2)
	{
		return true;
	}
	else if ((int)newstate > (int)currentstate)
	{
		return true;
	}
	else
	{
		return false;
	}
}
