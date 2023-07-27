#pragma once
#include <iostream>
#include "mfk.h"

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class CameraComponent;
class TransformComponent;
class StateSystem;
class StateComponent;

class CameraSystem
{
public:
	CameraSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager, StateSystem* statesystem);
	~CameraSystem();

	void _StartDraggingCameraWithMouse(StateComponent* MouseStatePtr, TransformComponent* CameraTransformPtr, float& anchorpos_x, float& anchorpos_y, int currentmousepos_x, int currentmousepos_y, float& savedcameraX, float& savedcameraY);
	void _DraggingCameraWithMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr, float mouseanchorposition_x, float mouseanchorposition_y, int currentmousepos_x, int currerentmousepos_y, int savedcameraX, int savedcameraY);
	void _FreeCameraFromMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr);
	void _MoveCameraInDirection(CameraComponent* CameraPtr, mfk::NormalVector2D movevector, uint64_t deltaTime);
	void _UpdateCameraTransformForLimits(CameraComponent* CameraPtr, TransformComponent* TransformPtr);
		
	void _ZoomIn(CameraComponent* CameraPtr);
	void _ZoomOut(CameraComponent* CameraPtr);
	void _ZoomReset(CameraComponent* CameraPtr);



private:
	EntitiesManager* m_Manager_Entities;
	ComponentsManager* m_Manager_Components;
	StateSystem* m_System_State;
};
