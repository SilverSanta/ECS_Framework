#pragma once

#include <string>
#include <unordered_map>

// FORWARD DECLARATIONS
struct SDL_Surface;

class ResourceManager {

public:
	static ResourceManager& Get_Instance();
	SDL_Surface* Get_Surface(std::string filepath);
	int Get_MapSize();


private:
	ResourceManager();
	ResourceManager(ResourceManager const&);
	ResourceManager operator=(ResourceManager const&);
	std::unordered_map <std::string, SDL_Surface*> m_surfaces;
};
