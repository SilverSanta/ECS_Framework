#pragma once

#include "CollisionSystem.h"
#include "EntitiesManager.h"
#include "UserExtension.h"


CollisionSystem::CollisionSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
}
CollisionSystem::~CollisionSystem() {}

void CollisionSystem::_Update_AllCollisionShapePositions()
{
	for (auto& CollisionShape : Manager_Components->Components_Collision)
	{
		Entity* Owner = Manager_Entities->Get_EntityById(CollisionShape.Get_OwnerId());
		TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

		if (TransformPtr != nullptr)
		{
			CollisionShape.CollisionBox.x = (int)TransformPtr->X + CollisionShape.OffsetFromOrigin.first;
			CollisionShape.CollisionBox.y = (int)TransformPtr->Y + CollisionShape.OffsetFromOrigin.second;

			CollisionShape.CollisionSphere.X = (int)TransformPtr->X + CollisionShape.OffsetFromOrigin.first;
			CollisionShape.CollisionSphere.Y = (int)TransformPtr->Y + CollisionShape.OffsetFromOrigin.second;
			CollisionShape.CollisionSphere.Update();
		}
	}
}
void CollisionSystem::_CollisionCheck_Mouse(CollisionComponent* mousecollisioncomp)
{
	for (auto& collisioncomp : Manager_Components->Components_Collision)
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
	if (c1->CollisionShape == Shape::CollisionBox && c2->CollisionShape == Shape::CollisionBox)
	{
		bCheck = CollisionCheck_SquareSquare(c1, c2);
		if (bCheck == true)
		{
			std::cout << "BOX-BOX\n";
		}
	}
	else if (c1->CollisionShape == Shape::Sphere && c2->CollisionShape == Shape::Sphere)
	{
		bCheck = CollisionCheck_CircleCircle(c1, c2);
		if (bCheck == true)
		{
			std::cout << "SPHERE-SPHERE\n";
		}
	}
	else
	{
		bCheck = CollisionCheck_SquareCircle(c1, c2);
		if (bCheck == true)
		{
			std::cout << "BOX-SPHERE\n";
		}
	}

	return bCheck;
}

bool CollisionSystem::CollisionCheck_SquareSquare(CollisionComponent* c1, CollisionComponent* c2)
{
	if ((c1->CollisionBox.x + c1->CollisionBox.w) <= c2->CollisionBox.x) { return false; }
	else if (c1->CollisionBox.x >= (c2->CollisionBox.x + c2->CollisionBox.w)) { return false; }
	else if ((c1->CollisionBox.y + c1->CollisionBox.h) <= c2->CollisionBox.y) { return false; }
	else if (c1->CollisionBox.y >= (c2->CollisionBox.y + c2->CollisionBox.h)) { return false; }
	else { return true; }
}
bool CollisionSystem::CollisionCheck_CircleCircle(CollisionComponent* c1, CollisionComponent* c2)
{
	int x1c = c1->CollisionSphere.X + c1->CollisionSphere.R;
	int y1c = c1->CollisionSphere.Y + c1->CollisionSphere.R;

	int x2c = c2->CollisionSphere.X + c2->CollisionSphere.R;
	int y2c = c2->CollisionSphere.Y + c2->CollisionSphere.R;

	if (((x2c - x1c) * (x2c - x1c) + (y2c - y1c) * (y2c - y1c)) >= (c1->CollisionSphere.R + c2->CollisionSphere.R) * (c1->CollisionSphere.R + c2->CollisionSphere.R))
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CollisionSystem::CollisionCheck_SquareCircle(CollisionComponent* c1, CollisionComponent* c2)
{
	CollisionComponent* Square = (c1->CollisionShape == Shape::CollisionBox) ? (c1) : (c2);
	CollisionComponent* Circle = (c1->CollisionShape == Shape::Sphere) ? (c1) : (c2);

	int BoxWidth = Square->CollisionBox.w;
	int BoxHeight = Square->CollisionBox.h;
	int SquareCentre_X = Square->CollisionBox.x + BoxWidth / 2;
	int SquareCentre_Y = Square->CollisionBox.y + BoxHeight / 2;

	int CircleRadius = Circle->CollisionSphere.R;
	int CircleCentre_X = Circle->CollisionSphere.X + CircleRadius;
	int CircleCentre_Y = Circle->CollisionSphere.Y + CircleRadius;

	int distance_X = abs(CircleCentre_X - SquareCentre_X);
	int distance_Y = abs(CircleCentre_Y - SquareCentre_Y);

	if (distance_X > (BoxWidth / 2 + CircleRadius)) { return false; }
	if (distance_Y > (BoxHeight / 2 + CircleRadius)) { return false; }
	if (distance_X <= BoxWidth / 2) { return true; }
	if (distance_Y <= BoxHeight / 2) { return true; }

	int Dist_Sq = (distance_X - BoxWidth / 2) * (distance_X - BoxWidth / 2) + (distance_Y - BoxHeight / 2) * (distance_Y - BoxHeight / 2);
	return(Dist_Sq <= CircleRadius * CircleRadius);
}
