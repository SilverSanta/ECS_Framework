#pragma once
#include "StateComponent.h"

// FORWARD DECLARATIONS

class EntitiesManager;
class ComponentsManager;
class ActionSystem;
class MovementSystem;


class StateSystem
{
public:
	StateSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~StateSystem();


public:
	static int _GenerateTotalState(user::State state, user::SubState substate, user::Direction direction)
	{
		int total = 10000 * (int)state + 100 * (int)substate + 1 * (int)direction;
		return total;
	}
	void _UpdateState_AllPreviousToCurrent();
	void _CheckForDead_All();


private:
	void UpdateState_PreviousToCurrent(StateComponent* stateptr);
	void ChangeCurrentState(StateComponent* stateptr, user::State newstate, user::SubState newsubstate, user::Direction newdirection);
	void ReturnToIdle(StateComponent* stateptr);
	void Set_State(StateComponent* StatePtr, user::State state);
	void Set_SubState(StateComponent* StatePtr, user::SubState substate);
	void Set_Direction(StateComponent* StatePtr, user::Direction direction);
	bool CheckForInterruptability(user::State newstate, user::State currentstate);
		
private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;


	friend class ActionSystem;
	friend class MovementSystem;
};
