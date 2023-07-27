#pragma once
#include <iostream>

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;

class TestSystem
{
public:
	TestSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~TestSystem();


	void _TestPrint();




private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;
};
