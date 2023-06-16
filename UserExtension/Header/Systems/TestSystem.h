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


	void TestPrint();




private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
};
