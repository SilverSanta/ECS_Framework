#pragma once
#include <vector>
#include <unordered_map>
#include "UserNamespace.h"

// FORWARD DECLARATIONS
class Entity;

class EntitiesManager {

public:
	// CONSTRUCTORS	
	EntitiesManager(uint64_t maxentities);
	~EntitiesManager();

	
	// FUNCTIONS
	uint64_t _CreateEntity();
	std::unordered_map<user::ComponentType, uint64_t> _DeleteEntity(uint64_t entityid);
	void PrintData_EntityContainer();
	Entity* Get_EntityById(uint64_t entityid);


	// DATA
public:
	uint64_t m_MaxEntities;
	uint16_t m_IdCounter;
	std::vector<Entity> m_Entities;
};