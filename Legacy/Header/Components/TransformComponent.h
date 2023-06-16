#pragma once

#include "IComponent.h"

// FORWARD DECLARATIONS
class Entity;


class TransformComponent: public IComponent{

	// CONSTRUCTORS
public:
	TransformComponent()
	{
		//std::cout << "     TRANSFORM created\n";
	}
	~TransformComponent()
	{
		//std::cout << "     TRANSFORM: #" << m_Id << " deleted\n";
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
		mfk::FindObjectInRepoAndKillIt(this, TransformRepository);
	}


	// FUNCTIONS
public:
	virtual void _Init(float x, float y, std::vector<TransformComponent> &repo)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			TransformRepository = &repo;


			// (2) TRANSFORM
			X = x;
			Y = y;
			PreviousFrame_X = x;
			PreviousFrame_Y = y;

			TransformAngle.x = 0;
			TransformAngle.y = 0;
		}
	}


protected:
	// REPOSITORY
	std::vector<TransformComponent>* TransformRepository;
	// DATA
public:
	float X;
	float Y;
	float PreviousFrame_X;
	float PreviousFrame_Y;
	
	mfk::NormalVector2D TransformAngle;
	

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
};
