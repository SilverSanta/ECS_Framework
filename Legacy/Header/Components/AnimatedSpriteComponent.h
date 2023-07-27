#pragma once

#include <algorithm>

#include "SDL.h"
#include "IComponent.h"
#include "ResourceManager.h"
#include "UserNamespace.h"

//FORWARD DECLARATIONS
class Entity;


class AnimatedSpriteComponent : public IComponent
{
	//CONSTRUCTORS	
public:	
	AnimatedSpriteComponent()
	{		
		//std::cout << "     SPRITE created\n";
	}
	~AnimatedSpriteComponent(){
	
		//std::cout << "     SPRITE: #" << m_Id << " deleted\n";
	}
	

	//OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{
			m_Time_Current = currentTime;
		}
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		if (this != nullptr)
		{
			if (m_bUsesTransformAngleForSpriteRotation == true)
			{
				SDL_RenderCopyEx(renderer, m_Spritesheet, &m_FrameOnSpritesheet, &m_FrameInGame, m_SpriteAngle, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
			}
			else
			{
				SDL_RenderCopy(renderer, m_Spritesheet, &m_FrameOnSpritesheet, &m_FrameInGame);
			}
		}
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, m_SpriteRepository);		
	}
public:
	virtual void _Init(SDL_Renderer* renderer, std::vector <AnimatedSpriteComponent> &repo, std::string filepath, std::vector<uint8_t> &framesperanimation, std::unordered_map<int, int> animationmapper,  int sheetframewidth, int sheetframeheight, int ingameframewidth, int ingameframeheight, int offsetx, int offsety,  uint64_t currenttime, float singleframelength, user::SpriteLayer layer, bool usesangle, bool usesstate)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			m_SpriteRepository = &repo;

			// (2) FRAMES
			m_FrameOnSpritesheet.w = sheetframewidth;
			m_FrameOnSpritesheet.h = sheetframeheight;

			m_FrameInGame.w = ingameframewidth;
			m_FrameInGame.h = ingameframeheight;


			// (3) OFFSET FROM ORIGIN AND ROTATION
			m_OffsetFromOrigin.first = offsetx;
			m_OffsetFromOrigin.second = offsety;
			m_bUsesTransformAngleForSpriteRotation = usesangle;
			m_SpriteAngle = 0;

			// (4) SPRITESHEET
			SDL_Surface* m_image = ResourceManager::_Get_Instance()._Get_Surface(filepath);
			m_Spritesheet = SDL_CreateTextureFromSurface(renderer, m_image);
			m_SpritesheetDimensions.first = m_image->w;
			m_SpritesheetDimensions.second = m_image->h;


			// (5) ANIMATION
			m_Mapper = animationmapper;
			m_Time_StartOfCurrentFrame = currenttime;
			m_SingleFrameLength = singleframelength;
			m_FramesPerAnimation = framesperanimation;
			m_CurrentFrameIndex = 0;
			m_CurrentAnimationIndex = 1;
			m_NumberOfAnimations = (int)m_FramesPerAnimation.size();
			m_FrameOnSpritesheet.y = m_CurrentAnimationIndex * sheetframeheight;
			m_Layer = layer;
			m_bUsesStateForTransform = usesstate;		
		}
	}


protected:
	// REPOSITORY
	std::vector <AnimatedSpriteComponent>* m_SpriteRepository;
	// DATA
	std::pair<int, int> m_OffsetFromOrigin;
	SDL_Texture* m_Spritesheet;
	std::pair<int, int> m_SpritesheetDimensions;
	SDL_Rect m_FrameOnSpritesheet;
	SDL_Rect m_FrameInGame;	
	uint64_t m_Time_Current;
	uint64_t m_Time_StartOfCurrentFrame;
	float m_SingleFrameLength;
	int m_NumberOfAnimations;
	uint8_t m_CurrentAnimationIndex;
	uint8_t m_CurrentFrameIndex;
	std::vector <uint8_t> m_FramesPerAnimation;
	std::unordered_map<int, int> m_Mapper;
	user::SpriteLayer m_Layer;
	bool m_bUsesTransformAngleForSpriteRotation;
	bool m_bUsesStateForTransform;
	float m_SpriteAngle;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class AnimationSystem;
	friend class RenderSystem;
	friend class MovementSystem;
};

