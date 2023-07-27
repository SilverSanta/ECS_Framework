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
	static int _GenerateTotalState(user::State state, user::SubState substate, user::Direction direction);
	void _UpdateState_AllPreviousToCurrent();
	void _CheckForDead_All();


private:
	void _UpdateState_PreviousToCurrent(StateComponent* stateptr);
	void _ChangeCurrentState(StateComponent* stateptr, user::State newstate, user::SubState newsubstate, user::Direction newdirection);
	void _ReturnToIdle(StateComponent* stateptr);
	void _Set_State(StateComponent* StatePtr, user::State state);
	void _Set_SubState(StateComponent* StatePtr, user::SubState substate);
	void _Set_Direction(StateComponent* StatePtr, user::Direction direction);
	bool _CheckForInterruptability(user::State newstate, user::State currentstate);
		
private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;


	friend class ActionSystem;
	friend class MovementSystem;
	friend class CameraSystem;
};
