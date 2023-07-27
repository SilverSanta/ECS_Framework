#pragma once

#include "CameraSystem.h"
#include "UserExtension.h"

CameraSystem::CameraSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
	m_System_State = statesystem;
}
CameraSystem::~CameraSystem(){}

void CameraSystem::_MoveCameraInDirection(CameraComponent* CameraPtr, mfk::NormalVector2D movevector, uint64_t deltaTime)
{
	mfk::NormalVector2D MovementVector = mfk::NormalVector2D(movevector.x, movevector.y);

	Entity* CameraEntity = m_Manager_Entities->_Get_EntityById(CameraPtr->_Get_OwnerId());
	auto TransformId = CameraEntity->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);

	float DeltaX = MovementVector.x * deltaTime * CameraPtr->m_CameraSpeed;
	float DeltaY = MovementVector.y * deltaTime * CameraPtr->m_CameraSpeed;

	if (TransformId != NULL)
	{
		TransformComponent* TransformPtr = m_Manager_Components->_Get_ComponentPtrFromId(TransformId, m_Manager_Components->m_Components_Transform);

		TransformPtr->m_X += DeltaX;
		TransformPtr->m_Y += DeltaY;

		_UpdateCameraTransformForLimits(CameraPtr, TransformPtr);
	}
}
void CameraSystem::_UpdateCameraTransformForLimits(CameraComponent* CameraComponentPtr, TransformComponent* CameraTransformPtr)
{
	if (CameraTransformPtr->m_X < CameraComponentPtr->m_HorizontalLimits_LEFT)
	{
		CameraTransformPtr->m_X = CameraComponentPtr->m_HorizontalLimits_LEFT;
	}
	else if (CameraTransformPtr->m_X > CameraComponentPtr->m_HorizontalLimits_RIGHT)
	{
		CameraTransformPtr->m_X = CameraComponentPtr->m_HorizontalLimits_RIGHT;
	}

	if (CameraTransformPtr->m_Y < CameraComponentPtr->m_VerticalLimits_UP)
	{
		CameraTransformPtr->m_Y = CameraComponentPtr->m_VerticalLimits_UP;
	}
	else if (CameraTransformPtr->m_Y > CameraComponentPtr->m_VerticalLimits_DOWN)
	{
		CameraTransformPtr->m_Y = CameraComponentPtr->m_VerticalLimits_DOWN;
	}
}

void CameraSystem::_StartDraggingCameraWithMouse(StateComponent* MouseStatePtr, TransformComponent* CameraTransformPtr, float& anchorpos_x, float& anchorpos_y, int currentmousepos_x, int currentmousepos_y, float& savedcameraX, float& savedcameraY)
{
	m_System_State->_ChangeCurrentState(MouseStatePtr, user::State::MOVING, user::SubState::MOVING_DRAGGING, MouseStatePtr->m_CurrentDirection);

	anchorpos_x = (float)currentmousepos_x;
	anchorpos_y = (float)currentmousepos_y;

	savedcameraX = CameraTransformPtr->m_X;
	savedcameraY = CameraTransformPtr->m_Y;
}
void CameraSystem::_DraggingCameraWithMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr, float mouseanchorposition_x, float mouseanchorposition_y, int currentmousepos_x, int currerentmousepos_y, int savedcameraX, int savedcameraY)
{
	m_System_State->_ChangeCurrentState(MouseStatePtr, user::State::MOVING, user::SubState::MOVING_DRAGGING, MouseStatePtr->m_CurrentDirection);

	uint64_t CameraTransformId = m_Manager_Entities->_Get_EntityById(CameraComponentPtr->_Get_OwnerId())->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM);
	if (CameraTransformId != NULL)
	{
		TransformComponent* CameraTransformPtr = m_Manager_Components->_Get_ComponentPtrFromId(CameraTransformId, m_Manager_Components->m_Components_Transform);

		CameraTransformPtr->m_X = savedcameraX + (mouseanchorposition_x - (float)currentmousepos_x);
		CameraTransformPtr->m_Y = savedcameraY + (mouseanchorposition_y - (float)currerentmousepos_y);

		std::cout << CameraTransformPtr->m_X << std::endl;
		_UpdateCameraTransformForLimits(CameraComponentPtr, CameraTransformPtr);
	}
}
void CameraSystem::_FreeCameraFromMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr)
{	
	m_System_State->_ChangeCurrentState(MouseStatePtr, user::State::IDLE, user::SubState::IDLE_DEFAULT, MouseStatePtr->m_CurrentDirection);
}

void CameraSystem::_ZoomIn(CameraComponent* CameraPtr)
{
	if (CameraPtr->m_CameraZoomRate > 0)
	{
		CameraPtr->CameraZoom = std::clamp(CameraPtr->CameraZoom / CameraPtr->m_CameraZoomRate, CameraPtr->m_CameraZoomMin, CameraPtr->m_CameraZoomMax);
	}	
}
void CameraSystem::_ZoomOut(CameraComponent* CameraPtr)
{
	if (CameraPtr->m_CameraZoomRate > 0)
	{
		CameraPtr->CameraZoom = std::clamp(CameraPtr->CameraZoom * CameraPtr->m_CameraZoomRate, CameraPtr->m_CameraZoomMin, CameraPtr->m_CameraZoomMax);
	}
}
void CameraSystem::_ZoomReset(CameraComponent* CameraPtr)
{
	CameraPtr->CameraZoom = std::clamp(1.f, CameraPtr->m_CameraZoomMin, CameraPtr->m_CameraZoomMax);
}