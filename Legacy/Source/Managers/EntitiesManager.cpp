// INCLUDES
#include <iostream>
#include "Entity.h"
#include "EntitiesManager.h"


// CONSTRUCTOR
EntitiesManager::EntitiesManager(uint64_t maxentities)
{
	m_MaxEntities = maxentities;
	m_IdCounter = 0;
	m_Entities.reserve(m_MaxEntities);
}
EntitiesManager::~EntitiesManager(){}


// FUNCTION DEFINITIONS
uint64_t EntitiesManager::_CreateEntity()
{
	if (m_Entities.size() < m_MaxEntities)
	{
		m_IdCounter++;
		Entity* NewEntity = new Entity(m_IdCounter, this);
		m_Entities.emplace_back(*NewEntity);

		return m_IdCounter;
	}
	else
	{
		std::cout << "Maximal entities exceeded\n";
		return NULL;
	}
}
std::unordered_map<user::ComponentType, uint64_t> EntitiesManager::_DeleteEntity(Entity* entity)
{
	// (1) Create a vector of pointers to all components from the entity
	std::unordered_map<user::ComponentType, uint64_t> ComponentsToBeDeletedByID = entity->RemoveComponentIDsFromEntity();

	// (2) Remove the entity from the vector (kill it)
	bool bFound = false;
	int i = 0;
	while (bFound == false)
	{
		if ((int)entity->Get_EntityId() == (int)m_Entities.at(i).Get_EntityId())
		{
			m_Entities.erase(m_Entities.begin() + i);
			bFound = true;
			break;
		}
		else
		{
			i++;
		}
	}
	return ComponentsToBeDeletedByID;
}
void EntitiesManager::PrintData_EntityContainer()
{
	std::cout << "ENTITIES amount: " << m_Entities.size() << ".\n";
	for (const auto& object : m_Entities)
	{
		std::cout << "ENTITY ID: #" << object.m_Id << ".\n";
	}
}
Entity* EntitiesManager::Get_EntityById(uint64_t entityid)
{
	Entity* entity;

	std::vector<Entity>::iterator itr = std::find_if(m_Entities.begin(), m_Entities.end(), [&](Entity& val) {return val.Get_EntityId() == entityid; });

	entity = &(*itr);

	if (entity != nullptr)
	{
		return entity;
		std::cout << entity->Get_EntityId() << std::endl;
	}
	else
	{
		return nullptr;
	}
}