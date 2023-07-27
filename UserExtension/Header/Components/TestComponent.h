#pragma once

#include "IComponent.h"


// FORWARD DECLARATIONS
class Entity;
struct SDL_Renderer;


class TestComponent : public IComponent
{
	//CONSTRUCTORS
public:	
	TestComponent()
	{	
		//std::cout << "     TEST created\n";
	}
	~TestComponent()
	{
		//std::cout << "     TEST: #" << m_Id << " deleted\n";
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
		mfk::FindObjectInRepoAndKillIt(this, m_TestRepository);
	}

	// FUNCTIONS
	virtual void _Init(std::vector<TestComponent>& repo, int testint)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_TestRepository = &repo;

			// (2) DATA
			m_TestInt = testint;
		}
	}


protected:
	// REPOSITORY
	std::vector<TestComponent>*m_TestRepository;
	// DATA
	int m_TestInt;

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;	
};

