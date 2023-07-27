#pragma once

#include "RenderSystem.h"

#include "EntitiesManager.h"
#include "UserExtension.h"


RenderSystem::RenderSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
}
RenderSystem::~RenderSystem(){}


void RenderSystem::_RenderSprite(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform)
{	
	Entity* Owner = m_Manager_Entities->_Get_EntityById(Sprite->_Get_OwnerId());
	TransformComponent* Transform = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), m_Manager_Components->m_Components_Transform);

	if (Transform != nullptr)
	{
		if (Transform->m_bSusceptibleToCameraMovement == true && Transform->m_bSusceptibleToCameraZoom == false)
		{
			Sprite->m_FrameInGame.x = (int)Transform->m_X + Sprite->m_OffsetFromOrigin.first - (int)CameraTransform->m_X;
			Sprite->m_FrameInGame.y = (int)Transform->m_Y + Sprite->m_OffsetFromOrigin.second - (int)CameraTransform->m_Y;

			Sprite->_Render(renderer);

			Sprite->m_FrameInGame.x += (int)CameraTransform->m_X;
			Sprite->m_FrameInGame.y += (int)CameraTransform->m_Y;
		}
		else if (Transform->m_bSusceptibleToCameraMovement == true && Transform->m_bSusceptibleToCameraZoom == true)
		{
			Sprite->m_FrameInGame.x = (int)Transform->m_X + Sprite->m_OffsetFromOrigin.first - (int)CameraTransform->m_X;
			Sprite->m_FrameInGame.y = (int)Transform->m_Y + Sprite->m_OffsetFromOrigin.second - (int)CameraTransform->m_Y;

			Sprite->_Render(renderer);

			Sprite->m_FrameInGame.x += (int)CameraTransform->m_X;
			Sprite->m_FrameInGame.y += (int)CameraTransform->m_Y;
		}
		else
		{
			Sprite->m_FrameInGame.x = (int)Transform->m_X + Sprite->m_OffsetFromOrigin.first;
			Sprite->m_FrameInGame.y = (int)Transform->m_Y + Sprite->m_OffsetFromOrigin.second;

			Sprite->_Render(renderer);
		}
	}
}

void RenderSystem::_RenderAllSprites(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& SpriteObj : m_Manager_Components->m_Components_AnimatedSprite)
	{
		_RenderSprite(renderer, &SpriteObj, CameraTransform);
	}
}
void RenderSystem::_RenderAllSpritesByLayer(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform)
{
	for (int i = 0; i < (int)user::SpriteLayer::_COUNT; i++)
	{
		for (auto& SpriteObj : m_Manager_Components->m_Components_AnimatedSprite)
		{
			if ((int)Sprite->m_Layer == i)
			{
				_RenderSprite(renderer, &SpriteObj, CameraTransform);
			}
		}
	}
}

void RenderSystem::_RenderCollisionShape(SDL_Renderer* renderer, CollisionComponent* m_CollisionShape, TransformComponent* CameraTransform)
{
	if (m_CollisionShape->m_bShowShape == true)
	{
		Entity* Owner = m_Manager_Entities->_Get_EntityById(m_CollisionShape->_Get_OwnerId());
		TransformComponent* Transform = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), m_Manager_Components->m_Components_Transform);

		if (Transform->m_bSusceptibleToCameraMovement == true && Transform->m_bSusceptibleToCameraZoom == false)
		{
			if (m_CollisionShape->m_CollisionShape == Shape::m_CollisionBox)
			{
				m_CollisionShape->m_CollisionBox.x -= (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionBox.y -= (int)CameraTransform->m_Y;

				m_CollisionShape->_Render(renderer);

				m_CollisionShape->m_CollisionBox.x += (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionBox.y += (int)CameraTransform->m_Y;
			}
			else if (m_CollisionShape->m_CollisionShape == Shape::Sphere)
			{
				m_CollisionShape->m_CollisionSphere.m_X -= (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionSphere.m_Y -= (int)CameraTransform->m_Y;
				m_CollisionShape->m_CollisionSphere.Update();
				m_CollisionShape->_Render(renderer);

				m_CollisionShape->m_CollisionSphere.m_X += (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionSphere.m_Y += (int)CameraTransform->m_Y;
			}
		}
		else if (Transform->m_bSusceptibleToCameraMovement == true && Transform->m_bSusceptibleToCameraZoom == true)
		{
			if (m_CollisionShape->m_CollisionShape == Shape::m_CollisionBox)
			{
				m_CollisionShape->m_CollisionBox.x -= (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionBox.y -= (int)CameraTransform->m_Y;

				m_CollisionShape->_Render(renderer);

				m_CollisionShape->m_CollisionBox.x += (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionBox.y += (int)CameraTransform->m_Y;
			}
			else if (m_CollisionShape->m_CollisionShape == Shape::Sphere)
			{
				m_CollisionShape->m_CollisionSphere.m_X -= (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionSphere.m_Y -= (int)CameraTransform->m_Y;
				m_CollisionShape->m_CollisionSphere.Update();
				m_CollisionShape->_Render(renderer);

				m_CollisionShape->m_CollisionSphere.m_X += (int)CameraTransform->m_X;
				m_CollisionShape->m_CollisionSphere.m_Y += (int)CameraTransform->m_Y;
			}
		}
		else
		{			
			m_CollisionShape->_Render(renderer);			
		}
	}
}
void RenderSystem::_RenderAllCollisionShapes(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& Shape : m_Manager_Components->m_Components_Collision)
	{
		_RenderCollisionShape(renderer, &Shape, CameraTransform);







	}
}

void RenderSystem::_RenderAllGrids(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& Grid : m_Manager_Components->m_Components_CellGrid)
	{
		Entity* Ent = m_Manager_Entities->_Get_EntityById(Grid._Get_OwnerId());
		uint64_t TransformId = Ent->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);
		if (TransformId != NULL)
		{
			TransformComponent* TransformPtr = m_Manager_Components->_Get_ComponentPtrFromId(TransformId, m_Manager_Components->m_Components_Transform);

			Grid.m_X = TransformPtr->m_X - (int)CameraTransform->m_X;
			Grid.m_Y = TransformPtr->m_Y - (int)CameraTransform->m_Y;

			Grid._Render(renderer);

			Grid.m_X += (int)CameraTransform->m_X;
			Grid.m_Y += (int)CameraTransform->m_Y;
		}
	}
}

void RenderSystem::_RenderText(SDL_Renderer* renderer, TextComponent* Text, TransformComponent* CameraTransform)
{
	Entity* Owner = m_Manager_Entities->_Get_EntityById(Text->_Get_OwnerId());
	TransformComponent* Transform = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), m_Manager_Components->m_Components_Transform);

	if (Transform != nullptr)
	{
		std::pair<int, int> AlignmentOffset;
		int w = Text->Text_Rectangle.w;
		int h = Text->Text_Rectangle.h;

		switch (Text->Position)
		{
		case user::Alignment::TOP_LEFT:
			AlignmentOffset = std::make_pair(0, 0);
			break;
		case user::Alignment::TOP_CENTRE:
			AlignmentOffset = std::make_pair(-w / 2, 0);
			break;
		case user::Alignment::TOP_RIGHT:
			AlignmentOffset = std::make_pair(-w, 0);
			break;
		case user::Alignment::MIDDLE_LEFT:
			AlignmentOffset = std::make_pair(0, -h / 2);
			break;
		case user::Alignment::MIDDLE_CENTRE:
			AlignmentOffset = std::make_pair(-w / 2, -h / 2);
			break;
		case user::Alignment::MIDDLE_RIGHT:
			AlignmentOffset = std::make_pair(-w, -h / 2);
			break;
		case user::Alignment::BOT_LEFT:
			AlignmentOffset = std::make_pair(0, -h);
			break;
		case user::Alignment::BOT_CENTRE:
			AlignmentOffset = std::make_pair(-w / 2, -h);
			break;
		case user::Alignment::BOT_RIGHT:
			AlignmentOffset = std::make_pair(-w, -h);
			break;
		}

		if (Transform->m_bSusceptibleToCameraMovement == true)
		{
			Text->Text_Rectangle.x = (int)Transform->m_X + Text->OffsetFromOrigin.first + AlignmentOffset.first - (int)CameraTransform->m_X;
			Text->Text_Rectangle.y = (int)Transform->m_Y + Text->OffsetFromOrigin.second + AlignmentOffset.second - (int)CameraTransform->m_Y;

			Text->_Render(renderer);

			Text->Text_Rectangle.x += (int)CameraTransform->m_X;
			Text->Text_Rectangle.y += (int)CameraTransform->m_Y;
		}
		else
		{
			Text->Text_Rectangle.x = (int)Transform->m_X + Text->OffsetFromOrigin.first + AlignmentOffset.first;
			Text->Text_Rectangle.y = (int)Transform->m_Y + Text->OffsetFromOrigin.second + AlignmentOffset.second;

			Text->_Render(renderer);
		}
	}
}
void RenderSystem::_RenderAllTexts(SDL_Renderer* renderer, TransformComponent* CameraTransform)
{
	for (auto& TextObj : m_Manager_Components->m_Components_Text)
	{
		_RenderText(renderer, &TextObj, CameraTransform);
	}
}