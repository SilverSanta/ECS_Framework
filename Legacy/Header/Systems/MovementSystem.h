#pragma once
#include <iostream>
#include "mfk.h"
#include "UserNamespace.h"

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class TransformComponent;
class AnimatedSpriteComponent;
class MovementComponent;
class StateComponent;
class StateSystem;


class MovementSystem
{
public:
	MovementSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem);
	~MovementSystem();


public:
	void _MoveAll(uint32_t deltaTime);
	float Get_AngleBetweenPoints(float x1, float y1, float x2, float y2);
	void Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, float angle);
	void Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, mfk::NormalVector2D vectorangle);


private:
	user::Direction Get_DirectionFromMovementVector(uint64_t movementcomponentid);
	void ApplyMovement(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, uint32_t deltaTime);
	void Update_MovementParameters(MovementComponent* MovementPtr, user::SubState substate);
	
	
private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
	StateSystem* System_State;
};
