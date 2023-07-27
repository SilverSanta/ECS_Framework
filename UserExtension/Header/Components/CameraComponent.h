#pragma once

#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
struct SDL_Renderer;


class CameraComponent : public IComponent
{
	//CONSTRUCTORS
public:	
	CameraComponent()
	{	
		//std::cout << "     CAMERA created\n";
	}
	~CameraComponent()
	{
		//std::cout << "     CAMERA: #" << m_Id << " deleted\n";
	}


	//OVERRIDES
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
		mfk::FindObjectInRepoAndKillIt(this, m_CameraRepository);
	}

	// FUNCTIONS
	virtual void _Init(std::vector<CameraComponent> &repo, float camerazoom, float camerazoommin, float camerazoommax, float camerazoomrate, float cameraspeed, float limitsleft, float limitsright, float limitsup, float limitsdown)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_CameraRepository = &repo;

			// (2) DATA
			CameraZoom = camerazoom;
			m_CameraZoomMin = camerazoommin;
			m_CameraZoomMax = camerazoommax;
			m_CameraZoomRate = camerazoomrate;
			m_CameraSpeed = cameraspeed;

			m_HorizontalLimits_LEFT = limitsleft;
			m_HorizontalLimits_RIGHT = limitsright;
			m_VerticalLimits_UP = limitsup;
			m_VerticalLimits_DOWN = limitsdown;
		}
	}


public:
	float CameraZoom;
protected:
	// REPOSITORY
	std::vector<CameraComponent>* m_CameraRepository;
	// DATA
	
	float m_CameraZoomMin;
	float m_CameraZoomMax;
	float m_CameraZoomRate;
	float m_CameraSpeed;

	float m_HorizontalLimits_LEFT;
	float m_HorizontalLimits_RIGHT;
	float m_VerticalLimits_UP;
	float m_VerticalLimits_DOWN;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;	
	friend class CameraSystem;
};

