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
		mfk::FindObjectInRepoAndKillIt(this, CameraRepository);
	}

	// FUNCTIONS
	virtual void _Init(std::vector<CameraComponent> &repo, float camerazoom, float camerazoommin, float camerazoommax, float camerazoomrate, float cameraspeed, float limitsleft, float limitsright, float limitsup, float limitsdown)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			CameraRepository = &repo;

			// (2) DATA
			CameraZoom = camerazoom;
			CameraZoomMin = camerazoommin;
			CameraZoomMax = camerazoommax;
			CameraZoomRate = camerazoomrate;
			CameraSpeed = cameraspeed;

			HorizontalLimits_LEFT = limitsleft;
			HorizontalLimits_RIGHT = limitsright;
			VerticalLimits_UP = limitsup;
			VerticalLimits_DOWN = limitsdown;
		}
	}


public:
	float CameraZoom;
protected:
	// REPOSITORY
	std::vector<CameraComponent>* CameraRepository;
	// DATA
	
	float CameraZoomMin;
	float CameraZoomMax;
	float CameraZoomRate;
	float CameraSpeed;

	float HorizontalLimits_LEFT;
	float HorizontalLimits_RIGHT;
	float VerticalLimits_UP;
	float VerticalLimits_DOWN;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;	
	friend class CameraSystem;
};

