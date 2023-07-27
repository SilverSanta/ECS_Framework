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
#include "TextComponent.h"
#include "TransformComponent.h"


class LegacyComponentsManager {

	// CONSTRUCTORS
public:
	LegacyComponentsManager(uint16_t maxentities)
	{
		m_MaxEntities = maxentities;

		m_Components_Action.reserve(m_MaxEntities);
		m_Components_AnimatedSprite.reserve(m_MaxEntities);
		m_Components_CellGrid.reserve(m_MaxEntities);
		m_Components_Collision.reserve(m_MaxEntities);
		m_Components_Movement.reserve(m_MaxEntities);
		m_Components_State.reserve(m_MaxEntities);
		m_Components_Transform.reserve(m_MaxEntities);
	}
	~LegacyComponentsManager() {}


	// FUNCTIONS
public:	

	template <class T>
	void _PrintData_ComponentContainer(std::vector<T> container)
	{
		std::cout << "COMPONENTS amount: " << container.size() << ".\n";
		for (const auto object : container)
		{
			const IComponent* Ptr = &object;
			std::cout << "COMPONENT ID: #" << Ptr->m_Id << ".\n";
		}
	}

	template<class T>
	void _UpdateComponents(std::vector<T> &components, uint64_t currenttime)
	{
		for (auto& component : components)
		{
			IComponent* Ptr = static_cast<IComponent*>(&component);
			Ptr->_Update(currenttime);
		}
	}

	template<class T>
	void _RemoveComponent(std::vector<T>& components,uint64_t id)
	{
		typename std::vector<T>::iterator itr = std::find_if(components.begin(), components.end(), [&](T& val) { IComponent* icomp = &val; return icomp->m_Id == id; });
		components.erase(itr);
	}			

	template<class T>
	T* _Get_ComponentPtrFromId(uint64_t componentid, std::vector<T>& components)
	{
		typename std::vector<T>::iterator itr = std::find_if(components.begin(), components.end(), [&](T& val) { T* ent = &val; return ent->m_Id == componentid; });
		T* TPtr = &(*itr);
		return TPtr;
	}

	virtual void _DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted);
	virtual void _UpdateRepositories(uint64_t currenttime);	
	void _SortSpritesByLayer();


	// DATA
public:
	uint16_t m_MaxEntities;
	std::vector<ActionComponent> m_Components_Action;
	std::vector<AnimatedSpriteComponent> m_Components_AnimatedSprite;
	std::vector<CellGridComponent> m_Components_CellGrid;
	std::vector<CollisionComponent> m_Components_Collision;
	std::vector<MovementComponent> m_Components_Movement;
	std::vector<StateComponent> m_Components_State;
	std::vector<TextComponent> m_Components_Text;
	std::vector<TransformComponent> m_Components_Transform;
};