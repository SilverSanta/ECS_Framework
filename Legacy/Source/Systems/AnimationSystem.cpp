#include "AnimationSystem.h"

#include "EntitiesManager.h"
#include "UserExtension.h"
#include "StateSystem.h"


AnimationSystem::AnimationSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	Manager_Entities = entitiesmanager;
	Manager_Components = componentsmanager;
}
AnimationSystem::~AnimationSystem() {


}

void AnimationSystem::_Update_AllAnimationsToState()
{
	for (auto& Sprite : Manager_Components->Components_AnimatedSprite)
	{			
		Entity* Entity = Manager_Entities->Get_EntityById(Sprite.Get_OwnerId());

		// ENTITY HAS STATE
		if (Entity->Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
		{
			StateComponent* State = Manager_Components->Get_ComponentPtrFromId(Entity->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

			if (State->m_CurrentState != State->m_StateFromPreviousFrame || State->m_CurrentSubState != State->m_SubStateFromPreviousFrame || State->m_CurrentDirection != State->m_DirectionFromPreviousFrame)
			{
				MapAnimationToState(&Sprite);
			}
		}
	}
}
void AnimationSystem::_NextFrame_AllSprites()
{
	for (auto& Sprite : Manager_Components->Components_AnimatedSprite)
	{
		Entity* Entity = Manager_Entities->Get_EntityById(Sprite.Get_OwnerId());

		// ENTITY HAS STATE
		if (Entity->Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
		{
			StateComponent* State = Manager_Components->Get_ComponentPtrFromId(Entity->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

			if (State != nullptr)
			{
				NextFrame(&Sprite);
			}
		}
		// ENTITY DOES NOT HAVE STATE
		else
		{
			NextFrame(&Sprite);
		}
	}
}

void AnimationSystem::Set_FrameIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index)
{
	SpritePtr->CurrentFrameIndex = index % SpritePtr->FramesPerAnimation.at(SpritePtr->CurrentAnimationIndex);
	SpritePtr->FrameOnSpritesheet.x = SpritePtr->CurrentFrameIndex * SpritePtr->FrameOnSpritesheet.w;
}
void AnimationSystem::Set_AnimationIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index)
{
	SpritePtr->CurrentAnimationIndex = index % SpritePtr->NumberOfAnimations;
	SpritePtr->FrameOnSpritesheet.y = SpritePtr->CurrentAnimationIndex * SpritePtr->FrameOnSpritesheet.h;

	Set_FrameIndex(SpritePtr, 0);
}

void AnimationSystem::NextFrame(AnimatedSpriteComponent* Sprite)
{
	//Check if there are more frames than just 1 and if enough time has passed
	if ((Sprite->FrameOnSpritesheet.w < Sprite->SpritesheetDimensions.first) && (Sprite->Time_Current - Sprite->Time_StartOfCurrentFrame >= Sprite->SingleFrameLength*1000))
	{
		Set_FrameIndex(Sprite, Sprite->CurrentFrameIndex + 1);		
		Sprite->Time_StartOfCurrentFrame = Sprite->Time_Current;
	}
}
void AnimationSystem::ChangeAnimationTo(AnimatedSpriteComponent* SpritePtr, int animationindex)
{
	if (animationindex > SpritePtr->NumberOfAnimations)
	{
		Set_AnimationIndex(SpritePtr, 0);
	}
	else
	{
		Set_AnimationIndex(SpritePtr, animationindex);
	}
}
void AnimationSystem::MapAnimationToState(AnimatedSpriteComponent* SpritePtr)
{
	Entity* Entity = Manager_Entities->Get_EntityById(SpritePtr->Get_OwnerId());
	StateComponent* StatePtr = Manager_Components->Get_ComponentPtrFromId(Entity->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

	if (StatePtr != nullptr && SpritePtr->bUsesStateForTransform == true)
	{
		int CurrentTotalState = StateSystem::_GenerateTotalState(StatePtr->m_CurrentState, StatePtr->m_CurrentSubState, StatePtr->m_CurrentDirection);
		int AnimationIndexFromMapper;
		if (SpritePtr->Mapper.find(CurrentTotalState) == SpritePtr->Mapper.end())
		{
			AnimationIndexFromMapper = 0;
		}
		else
		{			
			AnimationIndexFromMapper = SpritePtr->Mapper.at(CurrentTotalState);
		}

		ChangeAnimationTo(SpritePtr, AnimationIndexFromMapper);
	}
}