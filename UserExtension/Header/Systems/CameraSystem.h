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

	void StartDraggingCameraWithMouse(StateComponent* MouseStatePtr, TransformComponent* CameraTransformPtr, float& anchorpos_x, float& anchorpos_y, int currentmousepos_x, int currentmousepos_y, float& savedcameraX, float& savedcameraY);
	void DraggingCameraWithMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr, float mouseanchorposition_x, float mouseanchorposition_y, int currentmousepos_x, int currerentmousepos_y, int savedcameraX, int savedcameraY);
	void FreeCameraFromMouse(StateComponent* MouseStatePtr, CameraComponent* CameraComponentPtr);
	void MoveCameraInDirection(CameraComponent* CameraPtr, mfk::NormalVector2D movevector, uint64_t deltaTime);
	void UpdateCameraTransformForLimits(CameraComponent* CameraPtr, TransformComponent* TransformPtr);
	

	void SetCameraToMainMenuPosition();
	void SetCameraToInGamePosition();


	
	void ZoomIn(CameraComponent* CameraPtr);
	void ZoomOut(CameraComponent* CameraPtr);
	void ZoomReset(CameraComponent* CameraPtr);



private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
	StateSystem* System_State;
};
