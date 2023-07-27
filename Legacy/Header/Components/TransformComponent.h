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
		mfk::FindObjectInRepoAndKillIt(this, m_TransformRepository);
	}


	// FUNCTIONS
public:
	virtual void _Init(float x, float y, bool usescameratransform, bool usescamerazoom, std::vector<TransformComponent> &repo)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_TransformRepository = &repo;


			// (2) TRANSFORM
			m_X = x;
			m_Y = y;
			m_PreviousFrame_X = x;
			m_PreviousFrame_Y = y;

			m_TransformAngle.x = 0;
			m_TransformAngle.y = 0;

			// (3) CAMERA
			m_bSusceptibleToCameraMovement = usescameratransform;
			m_bSusceptibleToCameraZoom = usescamerazoom;
		}
	}


protected:
	// REPOSITORY
	std::vector<TransformComponent>* m_TransformRepository;
	// DATA
public:
	float m_X;
	float m_Y;
	float m_PreviousFrame_X;
	float m_PreviousFrame_Y;

	bool m_bSusceptibleToCameraMovement;
	bool m_bSusceptibleToCameraZoom;
	
	mfk::NormalVector2D m_TransformAngle;
	

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
};
