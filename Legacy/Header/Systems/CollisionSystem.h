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
	bool _CollisionCheck_SquareSquare(CollisionComponent* c1, CollisionComponent* c2);
	bool _CollisionCheck_CircleCircle(CollisionComponent* c1, CollisionComponent* c2);
	bool _CollisionCheck_SquareCircle(CollisionComponent* c1, CollisionComponent* c2);

private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;
};
