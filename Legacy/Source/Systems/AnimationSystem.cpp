#include "AnimationSystem.h"

#include "EntitiesManager.h"
#include "UserExtension.h"
#include "StateSystem.h"


AnimationSystem::AnimationSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager)
{
	m_Manager_Entities = entitiesmanager;
	m_Manager_Components = componentsmanager;
}
AnimationSystem::~AnimationSystem() {


}

void AnimationSystem::_Update_AllAnimationsToState()
{
	for (auto& Sprite : m_Manager_Components->m_Components_AnimatedSprite)
	{			
		Entity* Entity = m_Manager_Entities->_Get_EntityById(Sprite._Get_OwnerId());

		// ENTITY HAS STATE
		if (Entity->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
		{
			StateComponent* State = m_Manager_Components->_Get_ComponentPtrFromId(Entity->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);

			if (State->m_CurrentState != State->m_StateFromPreviousFrame || State->m_CurrentSubState != State->m_SubStateFromPreviousFrame || State->m_CurrentDirection != State->m_DirectionFromPreviousFrame)
			{
				_MapAnimationToState(&Sprite);
			}
		}
	}
}
void AnimationSystem::_NextFrame_AllSprites()
{
	for (auto& Sprite : m_Manager_Components->m_Components_AnimatedSprite)
	{
		Entity* Entity = m_Manager_Entities->_Get_EntityById(Sprite._Get_OwnerId());

		// ENTITY HAS STATE
		if (Entity->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL)
		{
			StateComponent* State = m_Manager_Components->_Get_ComponentPtrFromId(Entity->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);

			if (State != nullptr)
			{
				_NextFrame(&Sprite);
			}
		}
		// ENTITY DOES NOT HAVE STATE
		else
		{
			_NextFrame(&Sprite);
		}
	}
}

void AnimationSystem::_Set_FrameIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index)
{
	SpritePtr->m_CurrentFrameIndex = index % SpritePtr->m_FramesPerAnimation.at(SpritePtr->m_CurrentAnimationIndex);
	SpritePtr->m_FrameOnSpritesheet.x = SpritePtr->m_CurrentFrameIndex * SpritePtr->m_FrameOnSpritesheet.w;
}
void AnimationSystem::_Set_AnimationIndex(AnimatedSpriteComponent* SpritePtr, uint8_t index)
{
	SpritePtr->m_CurrentAnimationIndex = index % SpritePtr->m_NumberOfAnimations;
	SpritePtr->m_FrameOnSpritesheet.y = SpritePtr->m_CurrentAnimationIndex * SpritePtr->m_FrameOnSpritesheet.h;

	_Set_FrameIndex(SpritePtr, 0);
}

void AnimationSystem::_NextFrame(AnimatedSpriteComponent* Sprite)
{
	//Check if there are more frames than just 1 and if enough time has passed
	if ((Sprite->m_FrameOnSpritesheet.w < Sprite->m_SpritesheetDimensions.first) && (Sprite->m_Time_Current - Sprite->m_Time_StartOfCurrentFrame >= Sprite->m_SingleFrameLength*1000))
	{
		_Set_FrameIndex(Sprite, Sprite->m_CurrentFrameIndex + 1);		
		Sprite->m_Time_StartOfCurrentFrame = Sprite->m_Time_Current;
	}
}
void AnimationSystem::_ChangeAnimationTo(AnimatedSpriteComponent* SpritePtr, int animationindex)
{
	if (animationindex > SpritePtr->m_NumberOfAnimations)
	{
		_Set_AnimationIndex(SpritePtr, 0);
	}
	else
	{
		_Set_AnimationIndex(SpritePtr, animationindex);
	}
}
void AnimationSystem::_MapAnimationToState(AnimatedSpriteComponent* SpritePtr)
{
	Entity* Entity = m_Manager_Entities->_Get_EntityById(SpritePtr->_Get_OwnerId());
	StateComponent* StatePtr = m_Manager_Components->_Get_ComponentPtrFromId(Entity->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), m_Manager_Components->m_Components_State);

	if (StatePtr != nullptr && SpritePtr->m_bUsesStateForTransform == true)
	{
		int CurrentTotalState = StateSystem::_GenerateTotalState(StatePtr->m_CurrentState, StatePtr->m_CurrentSubState, StatePtr->m_CurrentDirection);
		int AnimationIndexFromMapper;
		if (SpritePtr->m_Mapper.find(CurrentTotalState) == SpritePtr->m_Mapper.end())
		{
			AnimationIndexFromMapper = 0;
		}
		else
		{			
			AnimationIndexFromMapper = SpritePtr->m_Mapper.at(CurrentTotalState);
		}

		_ChangeAnimationTo(SpritePtr, AnimationIndexFromMapper);
	}
}