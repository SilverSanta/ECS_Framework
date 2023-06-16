#include "ResourceManager.h"
#include "SDL.h"
#include <iostream>


// CONSTRUCTORS
ResourceManager::ResourceManager(){}
ResourceManager::ResourceManager(ResourceManager const&){}

ResourceManager& ResourceManager::Get_Instance(){
	static ResourceManager* s_instance = new ResourceManager;
	return *s_instance;
}
SDL_Surface* ResourceManager::Get_Surface(std::string filepath){
	SDL_Surface* Surface;

	//(1)check if the filepath is already in the map
	bool bFilepathPresent = (m_surfaces.find(filepath) == m_surfaces.end()) ? (false) : (true);
	
	//(2)if it is, use the surface from the map (pointer to the surface)
	
	if (bFilepathPresent) {
		std::cout << "Filepath already in the map, surface is not loaded again.\n";
		Surface = m_surfaces.at(filepath);
	}
	//(3)if not in the map, add it to the map and return the surface
	else {
		std::cout << "Filepath added, surface loaded.\n";
		Surface = SDL_LoadBMP(filepath.c_str());
		SDL_SetColorKey(Surface, true, SDL_MapRGB(Surface->format, 0, 255, 255));
		std::pair<std::string, SDL_Surface*> NewEntry = std::make_pair(filepath, Surface);
		m_surfaces.insert(NewEntry);
	}

	return Surface;
}
int ResourceManager::Get_MapSize()
{
	int Size = (int)m_surfaces.size();
	return Size;
}


