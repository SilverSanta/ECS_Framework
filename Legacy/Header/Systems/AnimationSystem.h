#pragma once
#include <iostream>

// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class AnimatedSpriteComponent;

class AnimationSystem
{
public:
	AnimationSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~AnimationSystem();


public:
	void _Update_AllAnimationsToState();
	void _NextFrame_AllSprites();	
private:
	void NextFrame(AnimatedSpriteComponent* Sprite);
	void ChangeAnimationTo(AnimatedSpriteComponent* SpritePtr, int animationindex);
	void MapAnimationToState(AnimatedSpriteComponent* Sprite);
	void Set_FrameIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index);
	void Set_AnimationIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index);


private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
};
