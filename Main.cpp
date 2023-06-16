// INCLUDES
#include "ECS_Framework.h"
#include "UserExtension.h"

#include "ResourceManager.h"
#include "EntitiesManager.h"
#include "UserExtension.h"


// POINTERS
MyFramework* Game;
EntitiesManager* Manager_Entities;
ComponentsManager* Manager_Components;
SystemsManager* Manager_Systems;
UserBlueprints* Blueprints;


// GLOBAL VARIABLES
uint64_t Mouse;
int MarkedMousePos_X = 0;
int MarkedMousePos_Y = 0;


// CALLBACKS
void HandleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{		
		auto DeltaTime = Game->Get_DeltaTicks();
		int MousePos_X, MousePos_Y;		
		SDL_PumpEvents();
		SDL_GetMouseState(&MousePos_X, &MousePos_Y);


		// EXITING THE PROGRAM
		if (event.type == SDL_QUIT)
		{
			Game->Set_IsRunning(false);
		}

		// MOUSE MOTION
		if (event.type == SDL_MOUSEMOTION)
		{
			Entity* MouseEnt = Manager_Entities->Get_EntityById(Mouse);
			TransformComponent* MouseTransform = Manager_Components->Get_ComponentPtrFromId(MouseEnt->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

			MouseTransform->PreviousFrame_X = MouseTransform->X;
			MouseTransform->PreviousFrame_Y = MouseTransform->Y;

			MouseTransform->X = (float)MousePos_X;
			MouseTransform->Y = (float)MousePos_Y;	
		}

		// MOUSE PRESSES
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			MarkedMousePos_X = MousePos_X;
			MarkedMousePos_Y = MousePos_Y;
		}
		if (event.button.button == SDL_BUTTON_MIDDLE)
		{

		}
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			
		}

		// KEYBOARD PRESSES		
		if (event.type == SDL_KEYDOWN) 
		{						
			if (event.key.keysym.sym == SDLK_m)
			{
				for (auto& Entity : Manager_Entities->m_Entities)
				{
					if (Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM) != NULL)
					{
						MovementComponent* MovementPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT), Manager_Components->Components_Movement);
						TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);
						StateComponent* StatePtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

						Manager_Systems->System_Action->Action_WalkTo(MovementPtr, TransformPtr, StatePtr, (float)MousePos_X, (float)MousePos_Y);
					}
				}
			}
			else if (event.key.keysym.sym == SDLK_s)
			{
				for (auto& Entity: Manager_Entities->m_Entities)
				{
					if (Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM) != NULL)
					{
						MovementComponent* MovementPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT), Manager_Components->Components_Movement);
						TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);
						StateComponent* StatePtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

						Manager_Systems->System_Action->Action_StopMoving(MovementPtr, StatePtr);
					}
				}		
			}
			else if (event.key.keysym.sym == SDLK_a)
			{
				for (auto& entity : Manager_Entities->m_Entities)
				{
					Manager_Systems->System_Action->Action_Attack(entity.Get_EntityId(), entity.Get_EntityId());
				}
			}
			else if (event.key.keysym.sym == SDLK_c)
			{
				Blueprints->Create_SkellyArcher((float)MousePos_X, (float)MousePos_Y, "Assets/Sprites/Sheets/skelly_archerBMP.bmp");
			}
			else if (event.key.keysym.sym == SDLK_p)
			{
				for (auto& entity : Manager_Entities->m_Entities)
				{
					Manager_Systems->System_Action->Action_UsePotion(entity.Get_EntityId());
				}
			}
			else if (event.key.keysym.sym == SDLK_d)
			{
				for (auto& entity : Manager_Entities->m_Entities)
				{
					Manager_Systems->System_Action->Action_DropDead(entity.Get_EntityId());
				}
			}
			else if (event.key.keysym.sym == SDLK_g)
			{
				for (auto& Grid : Manager_Components->Components_CellGrid)
				{
					if (Grid.bShowGrid == true) { Grid.bShowGrid = false; }
					else { Grid.bShowGrid = true; }
				}
			}
		}

		// KEYBOARD RELEASES		
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_1)
			{
				std::cout << "1 is released\n";
			}
		}
	}
}
void HandleLogic()
{
	uint32_t CurrentTime = Game->Get_CurrentTime();

	Manager_Components->UpdateRepositories(CurrentTime);
	Manager_Systems->System_Movement->_MoveAll(Game->Get_DeltaTicks());
	Manager_Systems->System_Collision->_Update_AllCollisionShapePositions();
	Manager_Systems->System_Animation->_NextFrame_AllSprites();
	Manager_Systems->System_Animation->_Update_AllAnimationsToState();
	Manager_Systems->System_State->_UpdateState_AllPreviousToCurrent();
	Manager_Systems->System_State->_CheckForDead_All();
}
void HandleRendering() {

	// (1) Set Window color - very improbable colour is used for this purpose (magenta)
	SDL_SetRenderDrawColor(Game->Get_Renderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);

	// (2) Clear the view
	SDL_RenderClear(Game->Get_Renderer());

	// (3) Render (send ti buffer)
	Manager_Systems->System_Render->RenderAllSprites(Game->Get_Renderer());
	//Manager_Systems->System_Render->RenderAllSpritesByLayer(Game->Get_Renderer());
	Manager_Systems->System_Render->RenderAllGrids(Game->Get_Renderer());
	Manager_Systems->System_Render->RenderAllCollisionShapes(Game->Get_Renderer());
		
	// (4) Buffer to screen
	SDL_RenderPresent(Game->Get_Renderer());
}
void HandleCameraInit() {

	std::cout << "___HANDLE CAMERA (START)___\n";

	//CAMERA INIT SYSTEM???

	std::cout << "___HANDLE CAMERA ( END )___\n";
}


// MAIN
int main(int argc, char* argv[]) {

	// INITIAL PARAMETERS
	const char* title = "SDL2 GAME";
	std::pair<uint16_t, uint16_t> WindowSize = std::make_pair(1024, 768);
	uint8_t FrameRate = 30;
	uint16_t MaxEntities = 2000;

	// GAME
	Game = new MyFramework(title, WindowSize, FrameRate);

	// MANAGERS
	Manager_Entities = new EntitiesManager(MaxEntities);
	Manager_Components = new ComponentsManager(MaxEntities);
	Manager_Systems = new SystemsManager(Manager_Entities, Manager_Components);

	// BLUEPRINTS
	Blueprints = new UserBlueprints(Game, Manager_Components, Manager_Entities);









	// =============================== EXAMPLE CODE ===============================
	Mouse = Blueprints->Create_MouseCursor();
	auto SomeGoblin1 = Blueprints->Create_Goblin(32, 32, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
	auto SomeGoblin2 = Blueprints->Create_Goblin(128, 128, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
	auto SomeGoblin3 = Blueprints->Create_Goblin(256, 256, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
	auto SomeSkellyArcher = Blueprints->Create_SkellyArcher(512, 512, "Assets/Sprites/Sheets/skelly_archerBMP.bmp");		
	auto SomeGrid = Blueprints->Create_Grid(16, 16, 20, 20, 32, 32);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto Tile = Blueprints->Create_Tile((float)(16 + j * 64), (float)(16 + i * 64), "Assets/Sprites/Sheets/Tile_GroundBMP.bmp");
		}
	}
	// =============================== EXAMPLE CODE ===============================










	//Function pointers
	Game->Set_EventCallback(HandleEvents);
	Game->Set_LogicCallback(HandleLogic);
	Game->Set_RenderCallback(HandleRendering);

	//Mainloop
	Game->RunGameLoop();
	SDL_Quit();
	return 0;
}
