#pragma once

#include "SDL.h"
#include "IComponent.h"
#include "ResourceManager.h"

// FORWARD DECLARATIONS
class Entity;
struct SDL_Renderer;


class TextComponent : public IComponent
{
	//CONSTRUCTORS
public:	
	TextComponent()
	{	
		//std::cout << "	 TEXT created\n";
	}
	~TextComponent()
	{
		//std::cout << "     TEXT: #" << m_Id << " deleted\n";
	}


	//OVERRIDES
public:
	virtual void _Update(uint64_t currentTime) override
	{
		if (this != nullptr)
		{

		}
	}
	virtual void _Render(SDL_Renderer* renderer) override
	{
		SDL_RenderCopy(renderer, TextTexture, NULL, &Text_Rectangle);
	}	

	virtual void _DeleteFromRepository() override
	{
		mfk::FindObjectInRepoAndKillIt(this, TextRepository);
	}
	 
	// FUNCTIONS
	virtual void _Init(std::vector<TextComponent> &repo, SDL_Renderer* renderer,  std::string text,const char* fontpath, int textheight, std::pair<int, int> offset, mfk::ColourList colour, user::Alignment position, user::Layer textlayer)
	{
		if (this != nullptr)
		{
			// (1) REPOSITORY
			TextRepository = &repo;

			// (2) DATA
			Text = text;
			OffsetFromOrigin = offset;
			TextHeight = textheight;
			Colour = colour;
			TextLayer = textlayer;

			_SwitchTextTo(Text.data(), fontpath, TextHeight, Colour,position, renderer);
		}
	}

	void _SwitchTextTo(const char* newtext, const char* newpath, int newtextheight, mfk::ColourList newcolour,user::Alignment position, SDL_Renderer* renderer)
	{
		FontPath = newpath;
		Position = position;

		SDL_Color Colour{ 0,0,0,0 };
		Colour.r = mfk::Colour::Get_ColourTable(newcolour).red;
		Colour.g = mfk::Colour::Get_ColourTable(newcolour).green;
		Colour.b = mfk::Colour::Get_ColourTable(newcolour).blue;

		TTF_Font* font = ResourceManager::_Get_Instance()._Get_Font(newpath, newtextheight);
		
		SDL_Surface* textsurface = TTF_RenderText_Solid(font, newtext, Colour);

		TextTexture = SDL_CreateTextureFromSurface(renderer, textsurface);

		TTF_SizeText(font, newtext, &Text_Rectangle.w, &Text_Rectangle.h);
	}
	

public:
	// REPOSITORY
	std::vector<TextComponent>* TextRepository;
	std::string Text;
	SDL_Texture* TextTexture;
	SDL_Rect Text_Rectangle;
	std::pair<int, int> OffsetFromOrigin;
	int TextHeight;
	mfk::ColourList Colour;
	user::Alignment Position;
	user::Layer TextLayer;
	std::string FontPath;

	// DATA
	

	//FRIEND CLASSES
	friend class Entity;
	friend class EntitiesManager;
	friend class ComponentsManager;	
	friend class RenderSystem;
};

