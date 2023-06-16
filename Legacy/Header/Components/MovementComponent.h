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
	virtual void _Update(uint32_t currentTime) override
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
		mfk::FindObjectInRepoAndKillIt(this, MovementRepository);
	}
public:
	virtual void _Init(float initx, float inity, float runspeed, float walkspeed, float swimspeed, float flyspeed, std::vector<MovementComponent> &repo)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			MovementRepository = &repo;

			// (2) MOVEMENT
			m_NormalVector.NormalBetweenTwoPoints(0.f, 0.f, initx, inity);
					
			m_CurrentSpeed = 0.f;
			bIsRunning = false;
			bIsWalking = false;
			bIsSwimming = false;
			bIsFlying = false;

			bHasTarget = false;
			Target_Y = 0;
			Target_X = 0;

			bIsAbleToMove = true;
			m_RunSpeed = runspeed;		
			m_WalkSpeed = walkspeed;	
			m_SwimSpeed = swimspeed;	
			m_FlySpeed = flyspeed;
		}
	}
	

protected:
	// REPOSITORY
	std::vector<MovementComponent>* MovementRepository;
	// DATA
public:
	mfk::NormalVector2D m_NormalVector;

protected:
	bool bHasTarget;
	float Target_X;
	float Target_Y;

	float m_CurrentSpeed;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_SwimSpeed;
	float m_FlySpeed;
	float m_SpeedFactor;

	bool bIsAbleToMove;
	bool bIsRunning;
	bool bIsWalking;	
	bool bIsSwimming;
	bool bIsFlying;


	// FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;

	friend class MovementSystem;
	friend class ActionSystem;
	friend class StateSystem;
};
