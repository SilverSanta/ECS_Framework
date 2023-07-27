#pragma once

#include <string>
#include <unordered_map>

#include "mfk.h"
#include "SDL_ttf.h"

// FORWARD DECLARATIONS
struct SDL_Surface;

class ResourceManager {

public:
	static ResourceManager& _Get_Instance();
	SDL_Surface* _Get_Surface(std::string filepath);
	int _Get_MapSize();
	TTF_Font* _Get_Font(const char* fontpath, int textheight);

private:
	ResourceManager();
	ResourceManager(ResourceManager const&);
	ResourceManager operator=(ResourceManager const&);
	std::unordered_map <std::string, SDL_Surface*> m_surfaces;
	std::unordered_map < std::string, TTF_Font* > m_fonts;
};
