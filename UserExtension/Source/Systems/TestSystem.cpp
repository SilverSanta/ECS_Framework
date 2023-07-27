#pragma once

#include "TestSystem.h"

TestSystem::TestSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
}
TestSystem::~TestSystem(){}


void TestSystem::_TestPrint()
{
	std::cout << "TEST SYSTEM\n";
}