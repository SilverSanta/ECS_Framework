#pragma once

#include "TestSystem.h"

TestSystem::TestSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
}
TestSystem::~TestSystem(){}


void TestSystem::TestPrint()
{
	std::cout << "TEST SYSTEM\n";
}