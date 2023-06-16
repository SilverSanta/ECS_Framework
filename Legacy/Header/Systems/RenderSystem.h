#pragma once


// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
struct SDL_Renderer;

class RenderSystem
{
public:
	RenderSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~RenderSystem();

	void RenderAllSprites(SDL_Renderer* renderer);
	void RenderAllSpritesByLayer(SDL_Renderer* renderer);
	void RenderAllCollisionShapes(SDL_Renderer* renderer);
	void RenderAllGrids(SDL_Renderer* renderer);
	

private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
};
