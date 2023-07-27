#pragma once

#include "IComponent.h"

// FORWARD DECLARATIONS
class Entity;

class MovementComponent : public IComponent {

	// CONSTRUCTORS
public:
	MovementComponent()
	{
		//std::cout << "     MOVEMENT created\n";
	}
	~MovementComponent()
	{
		//std::cout << "     MOVEMENT: #" << m_Id << " deleted\n";
	}


	// OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{

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
		mfk::FindObjectInRepoAndKillIt(this, m_MovementRepository);
	}
public:
	virtual void _Init(float initx, float inity, float runspeed, float walkspeed, float swimspeed, float flyspeed, std::vector<MovementComponent> &repo)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_MovementRepository = &repo;

			// (2) MOVEMENT
			m_NormalVector.NormalBetweenTwoPoints(0.f, 0.f, initx, inity);
					
			m_CurrentSpeed = 0.f;
			m_bIsRunning = false;
			m_bIsWalking = false;
			m_bIsSwimming = false;
			m_bIsFlying = false;

			m_bHasTarget = false;
			m_Target_Y = 0;
			m_Target_X = 0;

			m_bIsAbleToMove = true;
			m_RunSpeed = runspeed;		
			m_WalkSpeed = walkspeed;	
			m_SwimSpeed = swimspeed;	
			m_FlySpeed = flyspeed;
		}
	}
	

protected:
	// REPOSITORY
	std::vector<MovementComponent>* m_MovementRepository;
	// DATA
public:
	mfk::NormalVector2D m_NormalVector;

protected:
	bool m_bHasTarget;
	float m_Target_X;
	float m_Target_Y;

	float m_CurrentSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_SwimSpeed;
	float m_FlySpeed;
	float m_SpeedFactor;

	bool m_bIsAbleToMove;
	bool m_bIsRunning;
	bool m_bIsWalking;	
	bool m_bIsSwimming;
	bool m_bIsFlying;


	// FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;

	friend class MovementSystem;
	friend class ActionSystem;
	friend class StateSystem;
};
