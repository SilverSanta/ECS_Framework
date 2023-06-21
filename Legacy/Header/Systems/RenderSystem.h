#pragma once


// FORWARD DECLARATIONS
class EntitiesManager;
class ComponentsManager;
class AnimatedSpriteComponent;
class TransformComponent;
class CollisionComponent;
struct SDL_Renderer;

class RenderSystem
{
public:
	RenderSystem(ComponentsManager* componentsmanager, EntitiesManager* entitiesmanager);
	~RenderSystem();
	void RenderSprite(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform);
	void RenderAllSprites(SDL_Renderer* renderer, TransformComponent* CameraTransform);
	void RenderAllSpritesByLayer(SDL_Renderer* renderer, AnimatedSpriteComponent* Sprite, TransformComponent* CameraTransform);

	void RenderCollisionShape(SDL_Renderer* renderer, CollisionComponent* CollisionShape, TransformComponent* CameraTransform);
	void RenderAllCollisionShapes(SDL_Renderer* renderer, TransformComponent* CameraTransform);
	void RenderAllGrids(SDL_Renderer* renderer, TransformComponent* CameraTransform);
	

private:
	EntitiesManager* Manager_Entities;
	ComponentsManager* Manager_Components;
};
