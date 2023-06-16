#pragma once
#include <vector>
#include <iostream>

#include "Entity.h"
#include "EntitiesManager.h"

#include "IComponent.h"
#include "ActionComponent.h"
#include "AnimatedSpriteComponent.h"
#include "CellGridComponent.h"
#include "CollisionComponent.h"
#include "MovementComponent.h"
#include "StateComponent.h"
#include "TransformComponent.h"


class LegacyComponentsManager {

	// CONSTRUCTORS
public:
	LegacyComponentsManager(uint16_t maxentities)
	{
		m_MaxEntities = maxentities;

		Components_Action.reserve(m_MaxEntities);
		Components_AnimatedSprite.reserve(m_MaxEntities);
		Components_CellGrid.reserve(m_MaxEntities);
		Components_Collision.reserve(m_MaxEntities);
		Components_Movement.reserve(m_MaxEntities);
		Components_State.reserve(m_MaxEntities);
		Components_Transform.reserve(m_MaxEntities);
	}
	~LegacyComponentsManager() {}


	// FUNCTIONS
public:	

	template <class T>
	void PrintData_ComponentContainer(std::vector<T> container)
	{
		std::cout << "COMPONENTS amount: " << container.size() << ".\n";
		for (const auto object : container)
		{
			const IComponent* Ptr = &object;
			std::cout << "COMPONENT ID: #" << Ptr->m_Id << ".\n";
		}
	}

	template<class T>
	void UpdateComponents(std::vector<T> &components, uint32_t currenttime)
	{
		for (auto& component : components)
		{
			IComponent* Ptr = static_cast<IComponent*>(&component);
			Ptr->_Update(currenttime);
		}
	}

	template<class T>
	void RemoveComponent(std::vector<T>& components,uint64_t id)
	{
		typename std::vector<T>::iterator itr = std::find_if(components.begin(), components.end(), [&](T& val) { IComponent* icomp = &val; return icomp->m_Id == id; });
		components.erase(itr);
	}
	
	template<class T>
	T* Get_ComponentPtrFromId(uint64_t componentid, std::vector<T>& components)
	{
		typename std::vector<T>::iterator itr = std::find_if(components.begin(), components.end(), [&](T& val) { T* ent = &val; return ent->m_Id == componentid; });
		T* TPtr = &(*itr);
		return TPtr;
	}
		
	virtual void _DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted);
	virtual void UpdateRepositories(uint32_t currenttime);	
	void _SortSpritesByLayer();


	// DATA
public:
	uint16_t m_MaxEntities;
	std::vector<ActionComponent> Components_Action;
	std::vector<AnimatedSpriteComponent> Components_AnimatedSprite;
	std::vector<CellGridComponent> Components_CellGrid;
	std::vector<CollisionComponent> Components_Collision;
	std::vector<MovementComponent> Components_Movement;
	std::vector<StateComponent> Components_State;
	std::vector<TransformComponent> Components_Transform;
};