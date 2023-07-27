#pragma once

#include "EntitiesManager.h"
#include "UserExtension.h"
#include "StateSystem.h"


StateSystem::StateSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
}
StateSystem::~StateSystem() {}

int StateSystem::_GenerateTotalState(user::State state, user::SubState substate, user::Direction direction)
{
	int total = 100000 * (int)state + 100 * (int)substate + 1 * (int)direction;
	return total;
}
void StateSystem::_UpdateState_AllPreviousToCurrent()
{
	for (auto& State : m_Manager_Components->m_Components_State)
	{
		_UpdateState_PreviousToCurrent(&State);
	}
}
void StateSystem::_CheckForDead_All()
{
	std::vector<uint64_t> EntitiesToBeDeleted;

	for (auto& State : m_Manager_Components->m_Components_State)
	{
		if (State.m_CurrentState == user::State::DEAD)
		{
			if (State.m_Time_Current - State.m_Time_StartOfCurrentState >= State.m_Time_Dead * 1000)
			{				
				EntitiesToBeDeleted.push_back(State._Get_OwnerId());
			}
		}
	}
	for (auto& id : EntitiesToBeDeleted)
	{		
		m_Manager_Components->_DeleteComponents(m_Manager_Entities->_DeleteEntity(id));
	}
}
void StateSystem::_ChangeCurrentState(StateComponent* stateptr, user::State newstate, user::SubState newsubstate, user::Direction newdirection)
{
	if (stateptr->m_CurrentState != newstate || stateptr->m_CurrentSubState != newsubstate || stateptr->m_CurrentDirection != newdirection)
	{
		if (_CheckForInterruptability(newstate, stateptr->m_CurrentState) == true)
		{
			_Set_State(stateptr, newstate);
			_Set_SubState(stateptr, newsubstate);
			_Set_Direction(stateptr, newdirection);

			stateptr->m_Time_StartOfCurrentState = stateptr->m_Time_Current;
		}
	}
}
void StateSystem::_ReturnToIdle(StateComponent* stateptr)
{
	// (1) Forcing the state change (bypassing CheckForInterruptibility)
	stateptr->m_CurrentState = user::State::IDLE;

	_Set_State(stateptr, user::State::IDLE);
	_Set_SubState(stateptr, user::SubState::IDLE_DEFAULT);
	_Set_Direction(stateptr, stateptr->m_CurrentDirection);

	stateptr->m_Time_StartOfCurrentState = stateptr->m_Time_Current;
}
void StateSystem::_Set_State(StateComponent* StatePtr, user::State state)
{
	if (_CheckForInterruptability(state, StatePtr->m_CurrentState) == true)
	{
		StatePtr->m_StateFromPreviousFrame = StatePtr->m_CurrentState;
		StatePtr->m_CurrentState = state;
	}
}
void StateSystem::_Set_SubState(StateComponent* StatePtr, user::SubState substate)
{
	if ((int)substate * 1000 >= (int)StatePtr->m_CurrentState * 100000 && (int)substate * 1000 < ((int)StatePtr->m_CurrentState + 1) * 100000)
	{
		StatePtr->m_CurrentSubState = substate;
	}
}
void StateSystem::_Set_Direction(StateComponent* StatePtr, user::Direction direction)
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
bool StateSystem::_CheckForInterruptability(user::State newstate, user::State currentstate)
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
