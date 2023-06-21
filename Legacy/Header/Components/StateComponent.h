#pragma once

#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;


class StateComponent : public IComponent {
	// CONSTRUCTORS
public:
	StateComponent()
	{
		//std::cout << "     STATE created\n";
	}
	~StateComponent()
	{
		//std::cout << "     STATE: #" << m_Id << " deleted\n";
	}


	// OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{
			m_Time_Current = currentTime;
		}
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		if (this != nullptr)
		{

		}
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, StateRepository);
	}


	// FUNCTIONS
public:
	virtual void _Init(user::State state, user::SubState substate, user::Direction direction, std::vector<StateComponent>& repo)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			StateRepository = &repo;

			// (2) STATE
			m_CurrentState = state;
			m_CurrentSubState = substate;
			m_CurrentDirection = direction;
		
			m_StateFromPreviousFrame = state;
			m_SubStateFromPreviousFrame = substate;
			m_DirectionFromPreviousFrame = direction;			
		}
	}

protected:
	// (1) REPOSITORY
	std::vector<StateComponent>* StateRepository;
	// (2) DATA
public:
	user::State m_CurrentState;
	user::State m_StateFromPreviousFrame;
	user::SubState m_CurrentSubState;
	user::SubState m_SubStateFromPreviousFrame;
	user::Direction m_CurrentDirection;
	user::Direction m_DirectionFromPreviousFrame;
	uint64_t m_Time_Current;
	uint64_t m_Time_StartOfCurrentState;

	float m_Time_Dying = 0.5f;
	float m_Time_Dead = 0.5f;
	float m_Time_Attacking = 0.5f;
	float m_Time_UsingPotion = 1.f;
		

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
};
