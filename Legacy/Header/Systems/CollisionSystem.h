#pragma once


//FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class CollisionComponent;


class CollisionSystem
{
public:
	CollisionSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~CollisionSystem();
public:
	void _Update_AllCollisionShapePositions();
	
public:
	bool _CollisionCheck(CollisionComponent* c1, CollisionComponent* c2);
	void _CollisionCheck_Mouse(CollisionComponent* mousecollisioncomp);

private:
	bool CollisionCheck_SquareSquare(CollisionComponent* c1, CollisionComponent* c2);
	bool CollisionCheck_CircleCircle(CollisionComponent* c1, CollisionComponent* c2);
	bool CollisionCheck_SquareCircle(CollisionComponent* c1, CollisionComponent* c2);

private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
};
