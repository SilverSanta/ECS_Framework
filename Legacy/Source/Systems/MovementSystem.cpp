#pragma once

#include "EntitiesManager.h"
#include "UserExtension.h"

#include "MovementSystem.h"
#include "StateSystem.h"


MovementSystem::MovementSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
	System_State = statesystem;
}
MovementSystem::~MovementSystem(){}

void MovementSystem::_MoveAll(uint64_t deltaTime)
{
	for (auto& Movement : Manager_Components->Components_Movement)
	{
		Entity* Owner = Manager_Entities->Get_EntityById(Movement.Get_OwnerId());

		MovementComponent* MovementPtr = &Movement;
		TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);
		StateComponent* StatePtr = Manager_Components->Get_ComponentPtrFromId(Owner->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

		if (MovementPtr != nullptr && TransformPtr != nullptr && StatePtr != nullptr)
		{
			ApplyMovement(MovementPtr, TransformPtr, StatePtr, deltaTime);
		}
	}
}
void MovementSystem::Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, float angle)
{
	int IntegerAngle = (int)angle;
	int multiplier = IntegerAngle / 360;

	SpritePtr->SpriteAngle = angle - multiplier * 360;
	TransformPtr->TransformAngle.x = sinf(angle * mfk::Pi / 180);
	TransformPtr->TransformAngle.y = -cosf(angle * mfk::Pi / 180);
}
void MovementSystem::Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, mfk::NormalVector2D vectorangle)
{
	vectorangle.NormalBetweenTwoPoints(0, 0, vectorangle.x, vectorangle.y);

	SpritePtr->SpriteAngle = Get_AngleBetweenPoints(0, 0, vectorangle.x, vectorangle.y);
	TransformPtr->TransformAngle = vectorangle;
}
void MovementSystem::ApplyMovement(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, uint64_t deltaTime)
{
	if (StatePtr->m_CurrentState == user::State::MOVING)
	{
		if (MovementPtr->bHasTarget == true)
		{
			if (MovementPtr->m_NormalVector.LengthBetweenTwoPoints(TransformPtr->X, TransformPtr->Y, MovementPtr->Target_X, MovementPtr->Target_Y) > MovementPtr->m_WalkSpeed * deltaTime / 1000)
			{
				System_State->ChangeCurrentState(StatePtr, user::State::MOVING, StatePtr->m_CurrentSubState, Get_DirectionFromMovementVector(MovementPtr->m_Id));
				Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

				TransformPtr->PreviousFrame_X = TransformPtr->X;
				TransformPtr->PreviousFrame_Y = TransformPtr->Y;

				TransformPtr->X += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.x * deltaTime / 1000;
				TransformPtr->Y += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.y * deltaTime / 1000;
			}
			else
			{
				System_State->ChangeCurrentState(StatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, Get_DirectionFromMovementVector(MovementPtr->m_Id));
				Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

				TransformPtr->PreviousFrame_X = TransformPtr->X;
				TransformPtr->PreviousFrame_Y = TransformPtr->Y;
				TransformPtr->X = MovementPtr->Target_X;
				TransformPtr->Y = MovementPtr->Target_Y;


				// Resetting the target
				MovementPtr->bHasTarget = false;
				MovementPtr->Target_X = 0;
				MovementPtr->Target_Y = 0;
			}
		}
		else
		{
			System_State->ChangeCurrentState(StatePtr, user::State::MOVING, StatePtr->m_CurrentSubState, Get_DirectionFromMovementVector(MovementPtr->m_Id));
			Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

			TransformPtr->PreviousFrame_X = TransformPtr->X;
			TransformPtr->PreviousFrame_Y = TransformPtr->Y;
			TransformPtr->X += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.x * deltaTime / 1000;
			TransformPtr->Y += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.y * deltaTime / 1000;
		}
	}
	else if (StatePtr->m_CurrentState == user::State::IDLE)
	{
		// WIP
		Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);
	}
}
void MovementSystem::Update_MovementParameters(MovementComponent* MovementPtr, user::SubState substate)
{
	switch (substate)
	{
	case user::SubState::MOVING_WALKING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_WalkSpeed;
		MovementPtr->bIsRunning = false;
		MovementPtr->bIsWalking = true;;
		MovementPtr->bIsSwimming = false;
		MovementPtr->bIsFlying = false;
		break;
	case user::SubState::MOVING_RUNNING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_RunSpeed;
		MovementPtr->bIsRunning = true;
		MovementPtr->bIsWalking = false;
		MovementPtr->bIsSwimming = false;
		MovementPtr->bIsFlying = false;
		break;
	case user::SubState::MOVING_SWIMMING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_SwimSpeed;
		MovementPtr->bIsRunning = false;
		MovementPtr->bIsWalking = false;
		MovementPtr->bIsSwimming = true;
		MovementPtr->bIsFlying = false;
		break;
	case user::SubState::MOVING_FLYING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_SwimSpeed;
		MovementPtr->bIsRunning = false;
		MovementPtr->bIsWalking = false;
		MovementPtr->bIsSwimming = false;
		MovementPtr->bIsFlying = true;
		break;


	default:
		MovementPtr->m_CurrentSpeed = 0.f;
		MovementPtr->bIsRunning = false;
		MovementPtr->bIsWalking = false;
		MovementPtr->bIsSwimming = false;
		MovementPtr->bIsFlying = false;
		break;
	}
}
float MovementSystem::Get_AngleBetweenPoints(float x1, float y1, float x2, float y2)
{
	float dX = x2 - x1;
	float dY = -(y2 - y1);

	float Angle;
	if (dX >= 0 && dY >= 0) { Angle = atanf(dX / dY) * 180 / mfk::Pi; }
	else if (dY <= 0) { Angle = atanf(dX / dY) * 180 / mfk::Pi + 180; }
	else { Angle = 360 + atanf(dX / dY) * 180 / mfk::Pi; }

	return Angle;
}
user::Direction MovementSystem::Get_DirectionFromMovementVector(uint64_t movementcomponentid)
{
	auto MovementComponentPtr = Manager_Components->Get_ComponentPtrFromId(movementcomponentid, Manager_Components->Components_Movement);

	if (MovementComponentPtr->m_NormalVector.x >= mfk::Sqrt2 / 2)
	{
		return user::Direction::Facing_RIGHT;
	}
	else if (MovementComponentPtr->m_NormalVector.x <= -mfk::Sqrt2 / 2)
	{
		return user::Direction::Facing_LEFT;
	}
	else if (MovementComponentPtr->m_NormalVector.y >= mfk::Sqrt2 / 2)
	{
		return user::Direction::Facing_DOWN;
	}
	else
	{
		return user::Direction::Facing_UP;
	}
}
