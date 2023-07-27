#include "ResourceManager.h"
#include "SDL.h"
#include <iostream>


// CONSTRUCTORS
ResourceManager::ResourceManager(){}
ResourceManager::ResourceManager(ResourceManager const&){}

ResourceManager& ResourceManager::_Get_Instance(){
	static ResourceManager* s_instance = new ResourceManager;
	return *s_instance;
}
SDL_Surface* ResourceManager::_Get_Surface(std::string filepath){
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
int ResourceManager::_Get_MapSize()
{
	int Size = (int)m_surfaces.size();
	return Size;
}
TTF_Font* ResourceManager::_Get_Font(const char* fontpath, int textheight)
{
	TTF_Font* Font;
	std::string storedfontpath = fontpath + '_' + std::to_string(textheight);
	if (TTF_Init() < 0)
	{
		std::cout << " \n======= FAILED TO Init TFF =======\n\n";
	}

	bool bFontPathPresent = (m_fonts.find(storedfontpath) == m_fonts.end()) ? (false) : (true);
	if (bFontPathPresent == true)
	{
		//std::cout << "Fontpath already in the map, font is not loaded again.\n";
		Font = m_fonts.at(storedfontpath);
	}
	else
	{
		//std::cout << "Fontpath added:" << fontpath << " with text height: " << textheight << "\n";
		Font = TTF_OpenFont(fontpath, textheight);
		m_fonts.insert(std::make_pair(storedfontpath, Font));
	}

	return Font;
}

