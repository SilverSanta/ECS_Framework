#pragma once

#include <unordered_map>
#include <vector>

#include "UserNamespace.h"


// FORWARD DECLARATIONS
class IComponent;
class EntitiesManager;


class Entity {

public:
	// CONSTRUCTORS
	Entity();
	Entity(uint64_t id, EntitiesManager* entitiesmanager);
	~Entity();


	// FUNCTIONS
public:
	uint64_t Get_EntityId();
	uint64_t Get_ComponentIdFromEntityByType(user::ComponentType componenttype);
	uint64_t GenerateComponentId(user::ComponentType type);
	std::unordered_map<user::ComponentType, uint64_t> RemoveComponentIDsFromEntity();
	bool AddComponentToEntity(IComponent* component);


	// DATA
private:
	uint64_t m_Id;
	std::unordered_map<user::ComponentType, uint64_t> m_ComponentIds; // first -> slot (type), second -> ComponentID
	EntitiesManager* m_EntitiesManager;


	// FRIEND
	friend class EntitiesManager;
};