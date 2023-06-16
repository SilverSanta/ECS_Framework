#pragma once

#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
struct SDL_Renderer;


class ActionComponent : public IComponent
{
	//CONSTRUCTORS
public:	
	ActionComponent()
	{	
		//std::cout << "     ACTION created\n";
	}
	~ActionComponent()
	{
		//std::cout << "     ACTION: #" << m_Id << " deleted\n";
	}


	//OVERRIDES
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
		mfk::FindObjectInRepoAndKillIt(this, ActionRepository);
	}

	// FUNCTIONS
	virtual void _Init(std::vector<ActionComponent>& repo)
	{
		if (this != nullptr)
		{
			// (2) REPOSITORY
			ActionRepository = &repo;


			// (1) DATA
			
		}
	}


protected:
	// REPOSITORY
	std::vector<ActionComponent>*ActionRepository;
	// DATA
		

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
};

