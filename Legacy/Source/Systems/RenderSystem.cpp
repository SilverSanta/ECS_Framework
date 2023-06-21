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


void RenderSystem::RenderSprite(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform)
{	
	Entity* Owner = Manager_Entities->Get_EntityById(Sprite->Get_OwnerId());
	TransformComponent* Transform = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

	if (Transform != nullptr)
	{
		if (Transform->bSusceptibleToCameraMovement == true && Transform->bSusceptibleToCameraZoom == false)
		{
			Sprite->FrameInGame.x = (int)Transform->X + Sprite->OffsetFromOrigin.first - (int)CameraTransform->X;
			Sprite->FrameInGame.y = (int)Transform->Y + Sprite->OffsetFromOrigin.second - (int)CameraTransform->Y;

			Sprite->_Render(renderer);

			Sprite->FrameInGame.x += (int)CameraTransform->X;
			Sprite->FrameInGame.y += (int)CameraTransform->Y;
		}
		else if (Transform->bSusceptibleToCameraMovement == true && Transform->bSusceptibleToCameraZoom == true)
		{
			Sprite->FrameInGame.x = (int)Transform->X + Sprite->OffsetFromOrigin.first - (int)CameraTransform->X;
			Sprite->FrameInGame.y = (int)Transform->Y + Sprite->OffsetFromOrigin.second - (int)CameraTransform->Y;

			Sprite->_Render(renderer);

			Sprite->FrameInGame.x += (int)CameraTransform->X;
			Sprite->FrameInGame.y += (int)CameraTransform->Y;
		}
		else
		{
			Sprite->FrameInGame.x = (int)Transform->X + Sprite->OffsetFromOrigin.first;
			Sprite->FrameInGame.y = (int)Transform->Y + Sprite->OffsetFromOrigin.second;

			Sprite->_Render(renderer);
		}
	}
}

void RenderSystem::RenderAllSprites(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& SpriteObj : Manager_Components->Components_AnimatedSprite)
	{
		RenderSprite(renderer, &SpriteObj, CameraTransform);
	}
}
void RenderSystem::RenderAllSpritesByLayer(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform)
{
	for (int i = 0; i < (int)user::SpriteLayer::_COUNT; i++)
	{
		for (auto& SpriteObj : Manager_Components->Components_AnimatedSprite)
		{
			if ((int)Sprite->Layer == i)
			{
				RenderSprite(renderer, &SpriteObj, CameraTransform);
			}
		}
	}
}
void RenderSystem::RenderCollisionShape(SDL_Renderer* renderer, CollisionComponent* CollisionShape, TransformComponent* CameraTransform)
{
	if (CollisionShape->bShowShape == true)
	{
		Entity* Owner = Manager_Entities->Get_EntityById(CollisionShape->Get_OwnerId());
		TransformComponent* Transform = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

		if (Transform->bSusceptibleToCameraMovement == true && Transform->bSusceptibleToCameraZoom == false)
		{
			if (CollisionShape->CollisionShape == Shape::CollisionBox)
			{
				CollisionShape->CollisionBox.x -= (int)CameraTransform->X;
				CollisionShape->CollisionBox.y -= (int)CameraTransform->Y;

				CollisionShape->_Render(renderer);

				CollisionShape->CollisionBox.x += (int)CameraTransform->X;
				CollisionShape->CollisionBox.y += (int)CameraTransform->Y;
			}
			else if (CollisionShape->CollisionShape == Shape::Sphere)
			{
				CollisionShape->CollisionSphere.X -= (int)CameraTransform->X;
				CollisionShape->CollisionSphere.Y -= (int)CameraTransform->Y;
				CollisionShape->CollisionSphere.Update();
				CollisionShape->_Render(renderer);

				CollisionShape->CollisionSphere.X += (int)CameraTransform->X;
				CollisionShape->CollisionSphere.Y += (int)CameraTransform->Y;
			}
		}
		else if (Transform->bSusceptibleToCameraMovement == true && Transform->bSusceptibleToCameraZoom == true)
		{
			if (CollisionShape->CollisionShape == Shape::CollisionBox)
			{
				CollisionShape->CollisionBox.x -= (int)CameraTransform->X;
				CollisionShape->CollisionBox.y -= (int)CameraTransform->Y;

				CollisionShape->_Render(renderer);

				CollisionShape->CollisionBox.x += (int)CameraTransform->X;
				CollisionShape->CollisionBox.y += (int)CameraTransform->Y;
			}
			else if (CollisionShape->CollisionShape == Shape::Sphere)
			{
				CollisionShape->CollisionSphere.X -= (int)CameraTransform->X;
				CollisionShape->CollisionSphere.Y -= (int)CameraTransform->Y;
				CollisionShape->CollisionSphere.Update();
				CollisionShape->_Render(renderer);

				CollisionShape->CollisionSphere.X += (int)CameraTransform->X;
				CollisionShape->CollisionSphere.Y += (int)CameraTransform->Y;
			}
		}
		else
		{			
			CollisionShape->_Render(renderer);			
		}
	}
}
void RenderSystem::RenderAllCollisionShapes(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& Shape : Manager_Components->Components_Collision)
	{
		RenderCollisionShape(renderer, &Shape, CameraTransform);







	}
}
void RenderSystem::RenderAllGrids(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& Grid : Manager_Components->Components_CellGrid)
	{
		Entity* Ent = Manager_Entities->Get_EntityById(Grid.Get_OwnerId());
		uint64_t TransformId = Ent->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);
		if (TransformId != NULL)
		{
			TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(TransformId, Manager_Components->Components_Transform);

			Grid.X = TransformPtr->X - (int)CameraTransform->X;
			Grid.Y = TransformPtr->Y - (int)CameraTransform->Y;

			Grid._Render(renderer);

			Grid.X += (int)CameraTransform->X;
			Grid.Y += (int)CameraTransform->Y;
		}
	}
}