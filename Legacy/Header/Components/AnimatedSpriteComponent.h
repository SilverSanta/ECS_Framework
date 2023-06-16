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
	virtual void _Update(uint32_t currentTime) override
	{
		if (this != nullptr)
		{
			Time_Current = currentTime;
		}
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		if (this != nullptr)
		{
			if (bUsesTransformAngleForSpriteRotation == true)
			{
				SDL_RenderCopyEx(renderer, Spritesheet, &FrameOnSpritesheet, &FrameInGame, SpriteAngle, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
			}
			else
			{
				SDL_RenderCopy(renderer, Spritesheet, &FrameOnSpritesheet, &FrameInGame);
			}
		}
	}
	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, SpriteRepository);		
	}
public:
	virtual void _Init(SDL_Renderer* renderer, std::vector <AnimatedSpriteComponent> &repo, std::string filepath, std::vector<uint8_t> &framesperanimation, std::unordered_map<int, int> animationmapper,  int sheetframewidth, int sheetframeheight, int ingameframewidth, int ingameframeheight, int offsetx, int offsety,  uint32_t currenttime, float singleframelength, user::SpriteLayer layer, bool usesangle, bool usesstate)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			SpriteRepository = &repo;

			// (2) FRAMES
			FrameOnSpritesheet.w = sheetframewidth;
			FrameOnSpritesheet.h = sheetframeheight;

			FrameInGame.w = ingameframewidth;
			FrameInGame.h = ingameframeheight;


			// (3) OFFSET FROM ORIGIN AND ROTATION
			OffsetFromOrigin.first = offsetx;
			OffsetFromOrigin.second = offsety;
			bUsesTransformAngleForSpriteRotation = usesangle;
			SpriteAngle = 0;

			// (4) SPRITESHEET
			SDL_Surface* image = ResourceManager::Get_Instance().Get_Surface(filepath);
			Spritesheet = SDL_CreateTextureFromSurface(renderer, image);
			SpritesheetDimensions.first = image->w;
			SpritesheetDimensions.second = image->h;


			// (5) ANIMATION
			Mapper = animationmapper;
			Time_StartOfCurrentFrame = currenttime;
			SingleFrameLength = singleframelength;
			FramesPerAnimation = framesperanimation;
			CurrentFrameIndex = 0;
			CurrentAnimationIndex = 1;
			NumberOfAnimations = (int)FramesPerAnimation.size();
			FrameOnSpritesheet.y = CurrentAnimationIndex * sheetframeheight;
			Layer = layer;
			bUsesStateForTransform = usesstate;
		}
	}


protected:
	// REPOSITORY
	std::vector <AnimatedSpriteComponent>* SpriteRepository;
	// DATA
	std::pair<int, int> OffsetFromOrigin;
	SDL_Texture* Spritesheet;
	std::pair<int, int> SpritesheetDimensions;
	SDL_Rect FrameOnSpritesheet;
	SDL_Rect FrameInGame;	
	uint32_t Time_Current;
	uint32_t Time_StartOfCurrentFrame;
	float SingleFrameLength;
	int NumberOfAnimations;
	uint8_t CurrentAnimationIndex;
	uint8_t CurrentFrameIndex;
	std::vector <uint8_t> FramesPerAnimation;
	std::unordered_map<int, int> Mapper;
	user::SpriteLayer Layer;
	bool bUsesTransformAngleForSpriteRotation;
	bool bUsesStateForTransform;
	float SpriteAngle;


	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;
	friend class LegacyComponentsManager;
	friend class AnimationSystem;
	friend class RenderSystem;
	friend class MovementSystem;
};

