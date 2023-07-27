#pragma once

#include "CollisionSystem.h"
#include "EntitiesManager.h"
#include "UserExtension.h"


CollisionSystem::CollisionSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
}
CollisionSystem::~CollisionSystem() {}

void CollisionSystem::_Update_AllCollisionShapePositions()
{
	for (auto& m_CollisionShape : m_Manager_Components->m_Components_Collision)
	{
		Entity* Owner = m_Manager_Entities->_Get_EntityById(m_CollisionShape._Get_OwnerId());
		TransformComponent* TransformPtr = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), m_Manager_Components->m_Components_Transform);

		if (TransformPtr != nullptr)
		{
			m_CollisionShape.m_CollisionBox.x = (int)TransformPtr->m_X + m_CollisionShape.m_OffsetFromOrigin.first;
			m_CollisionShape.m_CollisionBox.y = (int)TransformPtr->m_Y + m_CollisionShape.m_OffsetFromOrigin.second;

			m_CollisionShape.m_CollisionSphere.m_X = (int)TransformPtr->m_X + m_CollisionShape.m_OffsetFromOrigin.first;
			m_CollisionShape.m_CollisionSphere.m_Y = (int)TransformPtr->m_Y + m_CollisionShape.m_OffsetFromOrigin.second;
			m_CollisionShape.m_CollisionSphere.Update();
		}
	}
}
void CollisionSystem::_CollisionCheck_Mouse(CollisionComponent* mousecollisioncomp)
{
	for (auto& collisioncomp : m_Manager_Components->m_Components_Collision)
	{
		if (&collisioncomp != mousecollisioncomp)
		{
			if (_CollisionCheck(mousecollisioncomp, &collisioncomp))
			{
				std::cout << "COLLISION DETECTED \n";
			}
			else
			{
				std::cout << "NOOOO \n";
			}
		}
	}
}
bool CollisionSystem::_CollisionCheck(CollisionComponent* c1, CollisionComponent* c2)
{
	bool bCheck;
	if (c1->m_CollisionShape == Shape::m_CollisionBox && c2->m_CollisionShape == Shape::m_CollisionBox)
	{
		bCheck = _CollisionCheck_SquareSquare(c1, c2);
		if (bCheck == true)
		{
			std::cout << "BOX-BOX\n";
		}
	}
	else if (c1->m_CollisionShape == Shape::Sphere && c2->m_CollisionShape == Shape::Sphere)
	{
		bCheck = _CollisionCheck_CircleCircle(c1, c2);
		if (bCheck == true)
		{
			std::cout << "SPHERE-SPHERE\n";
		}
	}
	else
	{
		bCheck = _CollisionCheck_SquareCircle(c1, c2);
		if (bCheck == true)
		{
			std::cout << "BOX-SPHERE\n";
		}
	}

	return bCheck;
}

bool CollisionSystem::_CollisionCheck_SquareSquare(CollisionComponent* c1, CollisionComponent* c2)
{
	if ((c1->m_CollisionBox.x + c1->m_CollisionBox.w) <= c2->m_CollisionBox.x) { return false; }
	else if (c1->m_CollisionBox.x >= (c2->m_CollisionBox.x + c2->m_CollisionBox.w)) { return false; }
	else if ((c1->m_CollisionBox.y + c1->m_CollisionBox.h) <= c2->m_CollisionBox.y) { return false; }
	else if (c1->m_CollisionBox.y >= (c2->m_CollisionBox.y + c2->m_CollisionBox.h)) { return false; }
	else { return true; }
}
bool CollisionSystem::_CollisionCheck_CircleCircle(CollisionComponent* c1, CollisionComponent* c2)
{
	int x1c = c1->m_CollisionSphere.m_X + c1->m_CollisionSphere.R;
	int y1c = c1->m_CollisionSphere.m_Y + c1->m_CollisionSphere.R;

	int x2c = c2->m_CollisionSphere.m_X + c2->m_CollisionSphere.R;
	int y2c = c2->m_CollisionSphere.m_Y + c2->m_CollisionSphere.R;

	if (((x2c - x1c) * (x2c - x1c) + (y2c - y1c) * (y2c - y1c)) >= (c1->m_CollisionSphere.R + c2->m_CollisionSphere.R) * (c1->m_CollisionSphere.R + c2->m_CollisionSphere.R))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CollisionSystem::_CollisionCheck_SquareCircle(CollisionComponent* c1, CollisionComponent* c2)
{
	CollisionComponent* Square = (c1->m_CollisionShape == Shape::m_CollisionBox) ? (c1) : (c2);
	CollisionComponent* Circle = (c1->m_CollisionShape == Shape::Sphere) ? (c1) : (c2);

	int BoxWidth = Square->m_CollisionBox.w;
	int BoxHeight = Square->m_CollisionBox.h;
	int SquareCentre_X = Square->m_CollisionBox.x + BoxWidth / 2;
	int SquareCentre_Y = Square->m_CollisionBox.y + BoxHeight / 2;

	int CircleRadius = Circle->m_CollisionSphere.R;
	int CircleCentre_X = Circle->m_CollisionSphere.m_X + CircleRadius;
	int CircleCentre_Y = Circle->m_CollisionSphere.m_Y + CircleRadius;

	int distance_X = abs(CircleCentre_X - SquareCentre_X);
	int distance_Y = abs(CircleCentre_Y - SquareCentre_Y);

	if (distance_X > (BoxWidth / 2 + CircleRadius)) { return false; }
	if (distance_Y > (BoxHeight / 2 + CircleRadius)) { return false; }
	if (distance_X <= BoxWidth / 2) { return true; }
	if (distance_Y <= BoxHeight / 2) { return true; }

	int Dist_Sq = (distance_X - BoxWidth / 2) * (distance_X - BoxWidth / 2) + (distance_Y - BoxHeight / 2) * (distance_Y - BoxHeight / 2);
	return(Dist_Sq <= CircleRadius * CircleRadius);
}
