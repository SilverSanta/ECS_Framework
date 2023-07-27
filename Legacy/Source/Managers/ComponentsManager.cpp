// INCLUDES
#include "ComponentsManager.h"


// FUNCTION DEFINITIONS
void LegacyComponentsManager::_SortSpritesByLayer()
{
	auto* SpritesPtr = &m_Components_AnimatedSprite;

	if ((int)m_Components_AnimatedSprite.size() > 1)
	{
		for (int i = 1; i < (int)SpritesPtr->size(); i++)
		{
			auto key = SpritesPtr->at(i);
			int keyLayerIndex = (int)key.m_Layer;
			int j = i - 1;

			while (j >= 0 && (int)SpritesPtr->at(j).m_Layer > keyLayerIndex)
			{
				SpritesPtr->at(j + 1) = SpritesPtr->at(j);
				j = j - 1;
			}
			SpritesPtr->at(j + 1) = key;
		}
	}
}


// USER-DEFINED
void LegacyComponentsManager::_DeleteComponents(std::unordered_map<user::ComponentType, uint64_t> IDsOfComponentsToBeDeleted)
{
	for (const auto& component : IDsOfComponentsToBeDeleted)
	{
		switch (component.first)
		{
		case user::ComponentType::ACTION:
			_RemoveComponent(m_Components_Action, component.second);
			break;
		case user::ComponentType::ANIMATEDSPRITE1:
		case user::ComponentType::ANIMATEDSPRITE2:
		case user::ComponentType::ANIMATEDSPRITE3:
		case user::ComponentType::ANIMATEDSPRITE4:
		case user::ComponentType::ANIMATEDSPRITE5:
			_RemoveComponent(m_Components_AnimatedSprite, component.second);
			break;
		case user::ComponentType::CELLGRID:
			_RemoveComponent(m_Components_CellGrid, component.second);
			break;
		case user::ComponentType::COLLISION1:
		case user::ComponentType::COLLISION2:
		case user::ComponentType::COLLISION3:
		case user::ComponentType::COLLISION4:
		case user::ComponentType::COLLISION5:
			_RemoveComponent(m_Components_Collision, component.second);
			break;
		case user::ComponentType::MOVEMENT:
			_RemoveComponent(m_Components_Movement, component.second);
			break;
		case user::ComponentType::STATE:
			_RemoveComponent(m_Components_State, component.second);
			break;
		case user::ComponentType::TEXT:
			_RemoveComponent(m_Components_Text, component.second);
			break;
		case user::ComponentType::TRANSFORM:
			_RemoveComponent(m_Components_Transform, component.second);
			break;
		}
	}
}
void LegacyComponentsManager::_UpdateRepositories(uint64_t currenttime)
{
	_UpdateComponents(m_Components_Action, currenttime);
	_UpdateComponents(m_Components_AnimatedSprite, currenttime);
	_UpdateComponents(m_Components_CellGrid, currenttime);
	_UpdateComponents(m_Components_Collision, currenttime);
	_UpdateComponents(m_Components_Movement, currenttime);
	_UpdateComponents(m_Components_State, currenttime);
	_UpdateComponents(m_Components_Text, currenttime);
	_UpdateComponents(m_Components_Transform, currenttime);
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
}




