#pragma once

#include "RenderSystem.h"

#include "EntitiesManager.h"
#include "UserExtension.h"


RenderSystem::RenderSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
}
RenderSystem::~RenderSystem(){}

void RenderSystem::RenderAllSprites(SDL_Renderer* renderer)
{
	for (auto& SpriteObj : Manager_Components->Components_AnimatedSprite)
	{
		Entity* Owner = Manager_Entities->Get_EntityById(SpriteObj.Get_OwnerId());
		TransformComponent* Transform = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

		if (Transform != nullptr)
		{
			SpriteObj.FrameInGame.x = (int)Transform->X + SpriteObj.OffsetFromOrigin.first;
			SpriteObj.FrameInGame.y = (int)Transform->Y + SpriteObj.OffsetFromOrigin.second;

			SpriteObj._Render(renderer);
		}
	}
}
void RenderSystem::RenderAllSpritesByLayer(SDL_Renderer* renderer)
{
	for (int i = 0; i < (int)user::SpriteLayer::_COUNT; i++)
	{
		for (auto& SpriteObj : Manager_Components->Components_AnimatedSprite)
		{
			if ((int)SpriteObj.Layer == i)
			{
				Entity* Owner = Manager_Entities->Get_EntityById(SpriteObj.Get_OwnerId());
				TransformComponent* Transform = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

				if (Transform != nullptr)
				{
					SpriteObj.FrameInGame.x = (int)Transform->X + SpriteObj.OffsetFromOrigin.first;
					SpriteObj.FrameInGame.y = (int)Transform->Y + SpriteObj.OffsetFromOrigin.second;

					SpriteObj._Render(renderer);
				}
			}
		}
	}
}
void RenderSystem::RenderAllCollisionShapes(SDL_Renderer* renderer)
{
	for (auto& Shape : Manager_Components->Components_Collision)
	{
		Shape._Render(renderer);
	}
}
void RenderSystem::RenderAllGrids(SDL_Renderer* renderer)
{
	for (auto& Grid : Manager_Components->Components_CellGrid)
	{
		Entity* Ent = Manager_Entities->Get_EntityById(Grid.Get_OwnerId());
		uint64_t TransformId = Ent->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);
		if (TransformId != NULL)
		{
			TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(TransformId, Manager_Components->Components_Transform);

			Grid.X = TransformPtr->X;
			Grid.Y = TransformPtr->Y;

			Grid._Render(renderer);
		}
	}
}