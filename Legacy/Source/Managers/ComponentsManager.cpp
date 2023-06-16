// INCLUDES
#include "ComponentsManager.h"


// FUNCTION DEFINITIONS
void LegacyComponentsManager::_SortSpritesByLayer()
{
	auto* SpritesPtr = &Components_AnimatedSprite;

	if ((int)Components_AnimatedSprite.size() > 1)
	{
		for (int i = 1; i < (int)SpritesPtr->size(); i++)
		{
			auto key = SpritesPtr->at(i);
			int keyLayerIndex = (int)key.Layer;
			int j = i - 1;

			while (j >= 0 && (int)SpritesPtr->at(j).Layer > keyLayerIndex)
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
			RemoveComponent(Components_Action, component.second);
			break;
		case user::ComponentType::ANIMATEDSPRITE1:
		case user::ComponentType::ANIMATEDSPRITE2:
		case user::ComponentType::ANIMATEDSPRITE3:
		case user::ComponentType::ANIMATEDSPRITE4:
		case user::ComponentType::ANIMATEDSPRITE5:
			RemoveComponent(Components_AnimatedSprite, component.second);
			break;
		case user::ComponentType::CELLGRID:
			RemoveComponent(Components_CellGrid, component.second);
			break;
		case user::ComponentType::COLLISION1:
		case user::ComponentType::COLLISION2:
		case user::ComponentType::COLLISION3:
		case user::ComponentType::COLLISION4:
		case user::ComponentType::COLLISION5:
			RemoveComponent(Components_Collision, component.second);
			break;
		case user::ComponentType::MOVEMENT:
			RemoveComponent(Components_Movement, component.second);
			break;
		case user::ComponentType::STATE:
			RemoveComponent(Components_State, component.second);
			break;
		case user::ComponentType::TRANSFORM:
			RemoveComponent(Components_Transform, component.second);
			break;
		}
	}
}
void LegacyComponentsManager::UpdateRepositories(uint32_t currenttime)
{
	UpdateComponents(Components_Action, currenttime);
	UpdateComponents(Components_AnimatedSprite, currenttime);
	UpdateComponents(Components_CellGrid, currenttime);
	UpdateComponents(Components_Collision, currenttime);
	UpdateComponents(Components_Movement, currenttime);
	UpdateComponents(Components_State, currenttime);
	UpdateComponents(Components_Transform, currenttime);
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
	// USER-DEFINED
}




