#pragma once

#include "EntitiesManager.h"
#include "UserExtension.h"

#include "MovementSystem.h"
#include "StateSystem.h"


MovementSystem::MovementSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
	m_System_State = statesystem;
}
MovementSystem::~MovementSystem(){}

void MovementSystem::_MoveAll(uint64_t deltaTime)
{
	for (auto& Movement : m_Manager_Components->m_Components_Movement)
	{
		Entity* Owner = m_Manager_Entities->_Get_EntityById(Movement._Get_OwnerId());

		MovementComponent* MovementPtr = &Movement;
		TransformComponent* TransformPtr = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), m_Manager_Components->m_Components_Transform);
		StateComponent* StatePtr = m_Manager_Components->_Get_ComponentPtrFromId(Owner->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);

		if (MovementPtr != nullptr && TransformPtr != nullptr && StatePtr != nullptr)
		{
			_ApplyMovement(MovementPtr, TransformPtr, StatePtr, deltaTime);
		}
	}
}
void MovementSystem::_Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, float angle)
{
	int IntegerAngle = (int)angle;
	int multiplier = IntegerAngle / 360;

	SpritePtr->m_SpriteAngle = angle - multiplier * 360;
	TransformPtr->m_TransformAngle.x = sinf(angle * mfk::Pi / 180);
	TransformPtr->m_TransformAngle.y = -cosf(angle * mfk::Pi / 180);
}
void MovementSystem::_Set_Angle(TransformComponent* TransformPtr, AnimatedSpriteComponent* SpritePtr, mfk::NormalVector2D vectorangle)
{
	vectorangle.NormalBetweenTwoPoints(0, 0, vectorangle.x, vectorangle.y);

	SpritePtr->m_SpriteAngle = _Get_AngleBetweenPoints(0, 0, vectorangle.x, vectorangle.y);
	TransformPtr->m_TransformAngle = vectorangle;
}
void MovementSystem::_ApplyMovement(MovementComponent* MovementPtr, TransformComponent* TransformPtr, StateComponent* StatePtr, uint64_t deltaTime)
{
	if (StatePtr->m_CurrentState == user::State::MOVING)
	{
		if (MovementPtr->m_bHasTarget == true)
		{
			if (MovementPtr->m_NormalVector.LengthBetweenTwoPoints(TransformPtr->m_X, TransformPtr->m_Y, MovementPtr->m_Target_X, MovementPtr->m_Target_Y) > MovementPtr->m_WalkSpeed * deltaTime / 1000)
			{
				m_System_State->_ChangeCurrentState(StatePtr, user::State::MOVING, StatePtr->m_CurrentSubState, _Get_DirectionFromMovementVector(MovementPtr->m_Id));
				_Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

				TransformPtr->m_PreviousFrame_X = TransformPtr->m_X;
				TransformPtr->m_PreviousFrame_Y = TransformPtr->m_Y;

				TransformPtr->m_X += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.x * deltaTime / 1000;
				TransformPtr->m_Y += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.y * deltaTime / 1000;
			}
			else
			{
				m_System_State->_ChangeCurrentState(StatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, _Get_DirectionFromMovementVector(MovementPtr->m_Id));
				_Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

				TransformPtr->m_PreviousFrame_X = TransformPtr->m_X;
				TransformPtr->m_PreviousFrame_Y = TransformPtr->m_Y;
				TransformPtr->m_X = MovementPtr->m_Target_X;
				TransformPtr->m_Y = MovementPtr->m_Target_Y;


				// Resetting the target
				MovementPtr->m_bHasTarget = false;
				MovementPtr->m_Target_X = 0;
				MovementPtr->m_Target_Y = 0;
			}
		}
		else
		{
			m_System_State->_ChangeCurrentState(StatePtr, user::State::MOVING, StatePtr->m_CurrentSubState, _Get_DirectionFromMovementVector(MovementPtr->m_Id));
			_Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);

			TransformPtr->m_PreviousFrame_X = TransformPtr->m_X;
			TransformPtr->m_PreviousFrame_Y = TransformPtr->m_Y;
			TransformPtr->m_X += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.x * deltaTime / 1000;
			TransformPtr->m_Y += MovementPtr->m_CurrentSpeed * MovementPtr->m_NormalVector.y * deltaTime / 1000;
		}
	}
	else if (StatePtr->m_CurrentState == user::State::IDLE)
	{
		// WIP
		_Update_MovementParameters(MovementPtr, StatePtr->m_CurrentSubState);
	}
}
void MovementSystem::_Update_MovementParameters(MovementComponent* MovementPtr, user::SubState substate)
{
	switch (substate)
	{
	case user::SubState::MOVING_WALKING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_WalkSpeed;
		MovementPtr->m_bIsRunning = false;
		MovementPtr->m_bIsWalking = true;;
		MovementPtr->m_bIsSwimming = false;
		MovementPtr->m_bIsFlying = false;
		break;
	case user::SubState::MOVING_RUNNING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_RunSpeed;
		MovementPtr->m_bIsRunning = true;
		MovementPtr->m_bIsWalking = false;
		MovementPtr->m_bIsSwimming = false;
		MovementPtr->m_bIsFlying = false;
		break;
	case user::SubState::MOVING_SWIMMING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_SwimSpeed;
		MovementPtr->m_bIsRunning = false;
		MovementPtr->m_bIsWalking = false;
		MovementPtr->m_bIsSwimming = true;
		MovementPtr->m_bIsFlying = false;
		break;
	case user::SubState::MOVING_FLYING:
		MovementPtr->m_CurrentSpeed = MovementPtr->m_SwimSpeed;
		MovementPtr->m_bIsRunning = false;
		MovementPtr->m_bIsWalking = false;
		MovementPtr->m_bIsSwimming = false;
		MovementPtr->m_bIsFlying = true;
		break;


	default:
		MovementPtr->m_CurrentSpeed = 0.f;
		MovementPtr->m_bIsRunning = false;
		MovementPtr->m_bIsWalking = false;
		MovementPtr->m_bIsSwimming = false;
		MovementPtr->m_bIsFlying = false;
		break;
	}
}
float MovementSystem::_Get_AngleBetweenPoints(float x1, float y1, float x2, float y2)
{
	float dX = x2 - x1;
	float dY = -(y2 - y1);

	float Angle;
	if (dX >= 0 && dY >= 0) { Angle = atanf(dX / dY) * 180 / mfk::Pi; }
	else if (dY <= 0) { Angle = atanf(dX / dY) * 180 / mfk::Pi + 180; }
	else { Angle = 360 + atanf(dX / dY) * 180 / mfk::Pi; }

	return Angle;
}
user::Direction MovementSystem::_Get_DirectionFromMovementVector(uint64_t movementcomponentid)
{
	auto MovementComponentPtr = m_Manager_Components->_Get_ComponentPtrFromId(movementcomponentid, m_Manager_Components->m_Components_Movement);

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
