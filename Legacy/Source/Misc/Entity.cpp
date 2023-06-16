#include <iostream>

#include "mfk.h"
#include "Entity.h"
#include "IComponent.h"

//CONSTRUCTORS

Entity::Entity(uint64_t id, EntitiesManager* entitiesmanager) {
	
	// (1) supply private data
	m_Id = id;
	m_EntitiesManager = entitiesmanager;	

	// (2) -> creating all empty slots into component map
	uint8_t ComponentSlots = (uint8_t)user::ComponentType::_NONE;
	for (uint8_t i = 0; i < ComponentSlots; i++)
	{
		m_ComponentIds.insert(std::make_pair((user::ComponentType)i, NULL));
	}
	std::cout << "   ENTITY #" << m_Id << " created\n";
}
Entity::~Entity()
{
	std::cout << "   ENTITY #" << m_Id << " deleted\n";
}

// GETTERS
uint64_t Entity::Get_EntityId()
{
	return m_Id;
}
uint64_t Entity::Get_ComponentIdFromEntityByType(user::ComponentType componenttype)
{
	int key = (int)componenttype;
		
	uint64_t ComponentID;

	if (m_ComponentIds.at(componenttype) != NULL)
	{
		ComponentID = m_ComponentIds.at(componenttype);
		return ComponentID;
	}
	else
	{
		return NULL;
	}
}


// MANAGEMENT
bool Entity::AddComponentToEntity(IComponent* component)
{
	if (component->m_OwnerId == this->m_Id)
	{
		int ComponentType = (int)component->m_Type;

		if (m_ComponentIds.at(component->m_Type) != NULL)
		{
			std::cout << "Slot occupied\n";
			return false;
		}
		else
		{
			m_ComponentIds.at(component->m_Type) = component->m_Id;
			return true;
		}
	}
	else
	{
		std::cout << "Component does not belong to this entity\n";
		return false;
	}
}
std::unordered_map<user::ComponentType, uint64_t> Entity::RemoveComponentIDsFromEntity()
{
	std::unordered_map<user::ComponentType, uint64_t> ComponentsToBeDeletedByID;

	for (auto const& Component : m_ComponentIds)
	{
		if (Component.second != NULL)
		{
			ComponentsToBeDeletedByID.insert(std::make_pair(Component.first, Component.second));

			std::cout << "Slot: " << (int)Component.first << " released from component #" << Component.second << "\n";
		}
		else
		{
			std::cout << "Slot: " << (int)Component.first << " is empty\n";
		}
	}
	return ComponentsToBeDeletedByID;
}
uint64_t Entity::GenerateComponentId(user::ComponentType type)
{
	uint64_t Id = (m_Id - 1) * (int)user::ComponentType::_NONE + (int)type;
	return Id;
}

