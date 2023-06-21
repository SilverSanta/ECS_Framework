#pragma once

#include "CameraSystem.h"
#include "UserExtension.h"

CameraSystem::CameraSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
	System_State = statesystem;
}
CameraSystem::~CameraSystem(){}

void CameraSystem::MoveCameraInDirection(CameraComponent* CameraPtr, mfk::NormalVector2D movevector, uint64_t deltaTime)
{
	mfk::NormalVector2D MovementVector = mfk::NormalVector2D(movevector.x, movevector.y);

	Entity* CameraEntity = Manager_Entities->Get_EntityById(CameraPtr->Get_OwnerId());
	auto TransformId = CameraEntity->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);

	float DeltaX = MovementVector.x * deltaTime * CameraPtr->CameraSpeed;
	float DeltaY = MovementVector.y * deltaTime * CameraPtr->CameraSpeed;

	if (TransformId != NULL)
	{
		TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(TransformId, Manager_Components->Components_Transform);

		TransformPtr->X += DeltaX;
		TransformPtr->Y += DeltaY;

		UpdateCameraTransformForLimits(CameraPtr, TransformPtr);
	}
}
void CameraSystem::UpdateCameraTransformForLimits(CameraComponent* CameraComponentPtr, TransformComponent* CameraTransformPtr)
{
	if (CameraTransformPtr->X < CameraComponentPtr->HorizontalLimits_LEFT)
	{
		CameraTransformPtr->X = CameraComponentPtr->HorizontalLimits_LEFT;
	}
	else if (CameraTransformPtr->X > CameraComponentPtr->HorizontalLimits_RIGHT)
	{
		CameraTransformPtr->X = CameraComponentPtr->HorizontalLimits_RIGHT;
	}

	if (CameraTransformPtr->Y < CameraComponentPtr->VerticalLimits_UP)
	{
		CameraTransformPtr->Y = CameraComponentPtr->VerticalLimits_UP;
	}
	else if (CameraTransformPtr->Y > CameraComponentPtr->VerticalLimits_DOWN)
	{
		CameraTransformPtr->Y = CameraComponentPtr->VerticalLimits_DOWN;
	}
}

void CameraSystem::StartDraggingCameraWithMouse(StateComponent* MouseStatePtr, TransformComponent* CameraTransformPtr, float& anchorpos_x, float& anchorpos_y, int currentmousepos_x, int currentmousepos_y, float& savedcameraX, float& savedcameraY)
{
	System_State->ChangeCurrentState(MouseStatePtr, user::State::MOVING, user::SubState::MOVING_DRAGGING, MouseStatePtr->m_CurrentDirection);

	anchorpos_x = (float)currentmousepos_x;
	anchorpos_y = (float)currentmousepos_y;

	savedcameraX = CameraTransformPtr->X;
	savedcameraY = CameraTransformPtr->Y;
}
void CameraSystem::DraggingCameraWithMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr, float mouseanchorposition_x, float mouseanchorposition_y, int currentmousepos_x, int currerentmousepos_y, int savedcameraX, int savedcameraY)
{
	System_State->ChangeCurrentState(MouseStatePtr, user::State::MOVING, user::SubState::MOVING_DRAGGING, MouseStatePtr->m_CurrentDirection);

	uint64_t CameraTransformId = Manager_Entities->Get_EntityById(CameraComponentPtr->Get_OwnerId())->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);
	if (CameraTransformId != NULL)
	{
		TransformComponent* CameraTransformPtr = Manager_Components->Get_ComponentPtrFromId(CameraTransformId, Manager_Components->Components_Transform);

		CameraTransformPtr->X = savedcameraX + (mouseanchorposition_x - (float)currentmousepos_x);
		CameraTransformPtr->Y = savedcameraY + (mouseanchorposition_y - (float)currerentmousepos_y);

		std::cout << CameraTransformPtr->X << std::endl;
		UpdateCameraTransformForLimits(CameraComponentPtr, CameraTransformPtr);
	}
}
void CameraSystem::FreeCameraFromMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr)
{	
	System_State->ChangeCurrentState(MouseStatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, MouseStatePtr->m_CurrentDirection);
}

void CameraSystem::ZoomIn(CameraComponent* CameraPtr)
{
	if (CameraPtr->CameraZoomRate > 0)
	{
		CameraPtr->CameraZoom = std::clamp(CameraPtr->CameraZoom / CameraPtr->CameraZoomRate, CameraPtr->CameraZoomMin, CameraPtr->CameraZoomMax);
	}	
}
void CameraSystem::ZoomOut(CameraComponent* CameraPtr)
{
	if (CameraPtr->CameraZoomRate > 0)
	{
		CameraPtr->CameraZoom = std::clamp(CameraPtr->CameraZoom * CameraPtr->CameraZoomRate, CameraPtr->CameraZoomMin, CameraPtr->CameraZoomMax);
	}
}
void CameraSystem::ZoomReset(CameraComponent* CameraPtr)
{
	CameraPtr->CameraZoom = std::clamp(1.f, CameraPtr->CameraZoomMin, CameraPtr->CameraZoomMax);
}