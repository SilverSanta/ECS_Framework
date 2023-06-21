// INCLUDES
#include "ECS_Framework.h"
#include "UserExtension.h"

#include "ResourceManager.h"
#include "EntitiesManager.h"
#include "UserExtension.h"


// POINTERS - GAME
MyFramework* Game;


// POINTERS - INGAME
EntitiesManager* Manager_Entities;
ComponentsManager* Manager_Components;
SystemsManager* Manager_Systems;
UserBlueprints* Blueprints;

uint64_t Mouse_InGame;
uint64_t Camera_InGame;


// POINTERS - MENU
EntitiesManager* Menu_Entities;
ComponentsManager* Menu_Components;
SystemsManager* Menu_Systems;
UserBlueprints* Menu_Blueprints;

uint64_t Mouse_InMenu;
uint64_t Camera_InMenu;


// MENU ENTITIES (IDs)
uint64_t Menu_Background;
uint64_t Menu_Button_Start;
uint64_t Menu_Button_Quit;


// RESOLUTION MAPPER
std::unordered_map<user::Resolution, std::pair<uint16_t, uint16_t>> Mapper_Resolutions
{
	std::make_pair(user::Resolution::r800x600, std::make_pair(800,600)),
	std::make_pair(user::Resolution::r1024x768, std::make_pair(1024,768)),
	std::make_pair(user::Resolution::r1920x1080, std::make_pair(1920,1080))
};

// CALLBACKS
void HandleEvents()
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	SDL_Event event;

	Entity* MousePtr = Manager_Entities->Get_EntityById(Mouse_InGame);
	TransformComponent* Mouse_InGame_TransformPtr = Manager_Components->Get_ComponentPtrFromId(MousePtr->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);
	StateComponent* Mouse_InGame_StatePtr = Manager_Components->Get_ComponentPtrFromId(MousePtr->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

	while (SDL_PollEvent(&event))
	{		
		auto DeltaTime = Game->Get_DeltaTicksInGame();	

		SDL_PumpEvents();
		SDL_GetMouseState(&Game->MousePos.first, &Game->MousePos.second);

		CameraComponent* Camera_InGame_ComponentPtr = Manager_Components->Get_ComponentPtrFromId(Manager_Entities->Get_EntityById(Camera_InGame)->Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->Components_Camera);
		TransformComponent* Camera_InGame_TransformPtr = Manager_Components->Get_ComponentPtrFromId(Manager_Entities->Get_EntityById(Camera_InGame)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

		// EXITING THE PROGRAM
		if (event.type == SDL_QUIT)
		{
			Game->Set_ProgramIsRunning(false);
		}

		// MOUSE MOTION
		if (event.type == SDL_MOUSEMOTION)
		{
			Mouse_InGame_TransformPtr->PreviousFrame_X = Mouse_InGame_TransformPtr->X;
			Mouse_InGame_TransformPtr->PreviousFrame_Y = Mouse_InGame_TransformPtr->Y;

			Mouse_InGame_TransformPtr->X = (float)Game->MousePos.first;
			Mouse_InGame_TransformPtr->Y = (float)Game->MousePos.second;
		}

		// KEYBOARD PRESSES		
		if (event.type == SDL_KEYDOWN) 
		{	
			// CAMERA MOVEMENT -> ARROWS		
			{
				if (Camera_InGame_ComponentPtr != nullptr)
				{
					mfk::NormalVector2D cameramovementvector;
					if (keyboard_state_array[SDL_SCANCODE_LEFT] && keyboard_state_array[SDL_SCANCODE_RIGHT] || keyboard_state_array[SDL_SCANCODE_UP] && keyboard_state_array[SDL_SCANCODE_DOWN])
					{						
						return;
					}
					else if (keyboard_state_array[SDL_SCANCODE_LEFT] && keyboard_state_array[SDL_SCANCODE_UP])
					{
						cameramovementvector.x = -1;
						cameramovementvector.y = -1;
					}
					else if (keyboard_state_array[SDL_SCANCODE_LEFT] && keyboard_state_array[SDL_SCANCODE_DOWN])
					{
						cameramovementvector.x = -1;
						cameramovementvector.y = 1;
					}
					else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_UP])
					{
						cameramovementvector.x = 1;
						cameramovementvector.y = -1;
					}
					else if (keyboard_state_array[SDL_SCANCODE_RIGHT] && keyboard_state_array[SDL_SCANCODE_DOWN])
					{
						cameramovementvector.x = 1;
						cameramovementvector.y = 1;
					}
					else if (keyboard_state_array[SDL_SCANCODE_LEFT])
					{
						cameramovementvector.x = -1;
						cameramovementvector.y = 0;
					}
					else if (keyboard_state_array[SDL_SCANCODE_RIGHT])
					{
						cameramovementvector.x = 1;
						cameramovementvector.y = 0;
					}
					else if (keyboard_state_array[SDL_SCANCODE_UP])
					{
						cameramovementvector.x = 0;
						cameramovementvector.y = -1;
					}
					else if (keyboard_state_array[SDL_SCANCODE_DOWN])
					{
						cameramovementvector.x = 0;
						cameramovementvector.y = 1;
					}
					
					if (cameramovementvector.x != 0 || cameramovementvector.y != 0)
					{
						Manager_Systems->System_Camera->MoveCameraInDirection(Camera_InGame_ComponentPtr, cameramovementvector, DeltaTime);
					}					
				}
			}

			// letters
			if (event.key.keysym.sym == SDLK_m)
			{
				for (auto& Entity : Manager_Entities->m_Entities)
				{
					if (Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL && Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM) != NULL)
					{
						MovementComponent* MovementPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT), Manager_Components->Components_Movement);
						TransformComponent* TransformPtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);
						StateComponent* StatePtr = Manager_Components->Get_ComponentPtrFromId(Entity.Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

						Manager_Systems->System_Action->Action_WalkTo(MovementPtr, TransformPtr, StatePtr, (float)Game->MousePos.first + Camera_InGame_TransformPtr->X, (float)Game->MousePos.second + Camera_InGame_TransformPtr->Y);
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
				Blueprints->Create_Goblin(Mouse_InGame_TransformPtr->X, Mouse_InGame_TransformPtr->Y, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
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
					if (entity.Get_EntityId() != Mouse_InGame)
					{
						Manager_Systems->System_Action->Action_DropDead(entity.Get_EntityId());
					}					
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
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (Game->InMenu() == false)
				{
					CameraComponent* Camera_InMenuPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Menu_Components->Components_Camera);
					TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->Components_Transform);

					// (1) Switch the bInMenu to be TRUE 
					Game->Set_InMenu(true);
					
					// (2) Save INGAME camera position to the GAME
					Game->CameraTransform_SavedInGame = std::make_pair(Camera_InGame_TransformPtr->X, Camera_InGame_TransformPtr->Y);

					// (3) Set transform of INGAME to some garbage far away value
					Camera_InGame_TransformPtr->X = -100000;
					Camera_InGame_TransformPtr->Y = -100000;

					// (4) Set transform of MENU to GAME MENU value
					Camera_InMenu_TransformPtr->X = Game->CameraTransform_Menu.first;
					Camera_InMenu_TransformPtr->Y = Game->CameraTransform_Menu.second;				
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
	Manager_Components->UpdateRepositories(Game->Get_CurrentTickInGame());
	Manager_Systems->System_Movement->_MoveAll(Game->Get_DeltaTicksInGame());
	Manager_Systems->System_Collision->_Update_AllCollisionShapePositions();
	Manager_Systems->System_Animation->_NextFrame_AllSprites();
	Manager_Systems->System_Animation->_Update_AllAnimationsToState();
	Manager_Systems->System_State->_UpdateState_AllPreviousToCurrent();
	Manager_Systems->System_State->_CheckForDead_All();

	Entity* MouseEnt = Manager_Entities->Get_EntityById(Mouse_InGame);
	StateComponent* MouseStatePtr = Manager_Components->Get_ComponentPtrFromId(MouseEnt->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->Components_State);

	Entity* CameraEnt = Manager_Entities->Get_EntityById(Camera_InGame);
	CameraComponent* CameraComponentPtr = Manager_Components->Get_ComponentPtrFromId(CameraEnt->Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->Components_Camera);

}
void HandleRendering()
{
	TransformComponent* Camera_InGame_TransformPtr = Manager_Components->Get_ComponentPtrFromId(Manager_Entities->Get_EntityById(Camera_InGame)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

	// (1) Set Window color - very improbable colour is used for this purpose (magenta)
	SDL_SetRenderDrawColor(Game->Get_Renderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);

	// (2) Clear the view
	SDL_RenderClear(Game->Get_Renderer());

	// (3) Render (send ti buffer)
	Manager_Systems->System_Render->RenderAllSprites(Game->Get_Renderer(), Camera_InGame_TransformPtr);
	Manager_Systems->System_Render->RenderAllGrids(Game->Get_Renderer(), Camera_InGame_TransformPtr);
	Manager_Systems->System_Render->RenderAllCollisionShapes(Game->Get_Renderer(), Camera_InGame_TransformPtr);
		
	// (4) Buffer to screen
	SDL_RenderPresent(Game->Get_Renderer());
}

void HandleMenuEvents()
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	SDL_Event event;

	CameraComponent* Camera_InMenu_ComponentPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Menu_Components->Components_Camera);
	TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->Components_Transform);

	Entity* Mouse_InMenuPtr = Menu_Entities->Get_EntityById(Mouse_InMenu);
	TransformComponent* Mouse_InMenu_TransformPtr = Menu_Components->Get_ComponentPtrFromId(Mouse_InMenuPtr->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->Components_Transform);
	StateComponent* Mouse_InMenu_StatePtr = Menu_Components->Get_ComponentPtrFromId(Mouse_InMenuPtr->Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Menu_Components->Components_State);
	
	while (SDL_PollEvent(&event))
	{
		auto DeltaTime = Game->Get_DeltaTicksInGame();
		SDL_PumpEvents();
		SDL_GetMouseState(&Game->MousePos.first, &Game->MousePos.second);

		// EXITING THE PROGRAM
		if (event.type == SDL_QUIT)
		{
			Game->Set_ProgramIsRunning(false);
		}

		// LEFT-CLICKS IN MENU
		if (event.button.button == SDL_BUTTON_LEFT && event.type != SDL_MOUSEMOTION)
		{
			CollisionComponent* Mouse_InGame_CollisionPtr = Menu_Components->Get_ComponentPtrFromId(Mouse_InMenuPtr->Get_ComponentIdFromEntityByType(user::ComponentType::COLLISION1), Menu_Components->Components_Collision);

			std::cout << "LEFT CLICK\n";			
			for (auto& CollisionComp : Menu_Components->Components_Collision)
			{
				if (CollisionComp.m_Id != Mouse_InGame_CollisionPtr->m_Id)
				{
					if (Menu_Systems->System_Collision->_CollisionCheck(Mouse_InGame_CollisionPtr, &CollisionComp))
					{	
						if (CollisionComp.Get_OwnerId() == Menu_Button_Start)
						{
							Game->StartGame();
							Game->Set_InMenu(false);
														
							Menu_Components->_DeleteComponents(Menu_Entities->_DeleteEntity(Menu_Button_Start));
						}
						else if (CollisionComp.Get_OwnerId() == Menu_Button_Quit)
						{
							Game->Set_ProgramIsRunning(false);
						}
					}
				}				
			}
		}

		// MOUSE MOTION
		if (event.type == SDL_MOUSEMOTION)
		{
			Mouse_InMenu_TransformPtr->PreviousFrame_X = Mouse_InMenu_TransformPtr->X;
			Mouse_InMenu_TransformPtr->PreviousFrame_Y = Mouse_InMenu_TransformPtr->Y;

			Mouse_InMenu_TransformPtr->X = (float)Game->MousePos.first;
			Mouse_InMenu_TransformPtr->Y = (float)Game->MousePos.second;
		}

		// MENU_INGAME SWITCH
		if (event.type == SDL_KEYDOWN)
		{

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (Game->InMenu() == true && Game->GameIsRunning() == true)
				{
					CameraComponent* Camera_InGamePtr = Manager_Components->Get_ComponentPtrFromId(Manager_Entities->Get_EntityById(Camera_InGame)->Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->Components_Camera);
					TransformComponent* Camera_InGame_TransformPtr = Manager_Components->Get_ComponentPtrFromId(Manager_Entities->Get_EntityById(Camera_InGame)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->Components_Transform);

					// (1) Switch the bInMenu to be FALSE
					Game->Set_InMenu(false);					

					// (2) Set transform of MENU to some garbage far away value
					Camera_InMenu_TransformPtr->X = -100000;
					Camera_InMenu_TransformPtr->Y = -100000;

					// (3) Set transform of INGAME to GAME INGAME value
					Camera_InGame_TransformPtr->X = Game->CameraTransform_SavedInGame.first;
					Camera_InGame_TransformPtr->Y = Game->CameraTransform_SavedInGame.second;
				}
			}
		}
	}
}
void HandleMenuLogic()
{
	Menu_Components->UpdateRepositories(Game->Get_CurrentTickInMenu());
	Menu_Systems->System_Movement->_MoveAll(Game->Get_DeltaTicksInGame());
	Menu_Systems->System_Collision->_Update_AllCollisionShapePositions();
	Menu_Systems->System_Animation->_NextFrame_AllSprites();
	Menu_Systems->System_Animation->_Update_AllAnimationsToState();
	Menu_Systems->System_State->_UpdateState_AllPreviousToCurrent();
	Menu_Systems->System_State->_CheckForDead_All();
}
void HandleMenuRendering()
{
	TransformComponent* Camera_Menu_TransformPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->Components_Transform);

	// (1) Set Window color - very improbable colour is used for this purpose (magenta)
	SDL_SetRenderDrawColor(Game->Get_Renderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);

	// (2) Clear the view
	SDL_RenderClear(Game->Get_Renderer());

	// (3) Render (send ti buffer)
	Menu_Systems->System_Render->RenderAllSprites(Game->Get_Renderer(), Camera_Menu_TransformPtr);
	Menu_Systems->System_Render->RenderAllCollisionShapes(Game->Get_Renderer(), Camera_Menu_TransformPtr);

	// (4) Buffer to screen
	SDL_RenderPresent(Game->Get_Renderer());
}

// MAIN
int main(int argc, char* argv[])
{
	// INITIAL PARAMETERS
	const char* title = "SDL2 GAME";

	user::Resolution WindowResolution = user::Resolution::r1024x768;
	std::pair<uint16_t, uint16_t> WindowSize = Mapper_Resolutions.at(WindowResolution);

	uint8_t FrameRate = 30;
	uint16_t MaxEntities = 2000;

	Game = new MyFramework(title, WindowSize, FrameRate);

	Game->CameraTransform_InGameForAnchoring = std::make_pair(0.f, 0.f);
	Game->CameraTransform_SavedInGame = std::make_pair(0.f, 0.f);
	Game->CameraTransform_Menu = std::make_pair(0.f, 0.f);
	Game->MousePos = std::make_pair(0, 0);

	// ALL MANAGERS -> INGAME
	Manager_Entities = new EntitiesManager(MaxEntities);
	Manager_Components = new ComponentsManager(MaxEntities);
	Manager_Systems = new SystemsManager(Manager_Entities, Manager_Components);
	Blueprints = new UserBlueprints(Game, Manager_Components, Manager_Entities);
	
	// ALL MANAGERS -> MENU
	Menu_Entities = new EntitiesManager(MaxEntities);
	Menu_Components = new ComponentsManager(MaxEntities);
	Menu_Systems = new SystemsManager(Menu_Entities, Menu_Components);
	Menu_Blueprints = new UserBlueprints(Game, Menu_Components, Menu_Entities);

	// INGAME OBJECTS
	Mouse_InGame = Blueprints->Create_MouseCursor();
	Camera_InGame = Blueprints->Create_Camera(0.5f, -500.f, 500.f, -300.f, 300.f);


	// MENU OBJECTS
	Mouse_InMenu = Menu_Blueprints->Create_MouseCursor();
	Camera_InMenu = Menu_Blueprints->Create_Camera(0.f, -Game->CameraTransform_Menu.first + WindowSize.first / 2, Game->CameraTransform_Menu.first + WindowSize.first / 2, -Game->CameraTransform_Menu.second + WindowSize.second / 2, Game->CameraTransform_Menu.second + WindowSize.second / 2);


	// =============================== SAMPLE CODE ===============================
	auto SomeGoblin1 = Blueprints->Create_Goblin(32, 32, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
	auto SomeGrid = Blueprints->Create_Grid(16, 16, 20, 20, 32, 32);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto Tile = Blueprints->Create_Tile((float)(16 + j * 64), (float)(16 + i * 64), "Assets/Sprites/Sheets/Tile_GroundBMP.bmp");
		}
	}
	
	TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->Get_ComponentPtrFromId(Menu_Entities->Get_EntityById(Camera_InMenu)->Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->Components_Transform);
	Camera_InMenu_TransformPtr->X = Game->CameraTransform_Menu.first;
	Camera_InMenu_TransformPtr->Y = Game->CameraTransform_Menu.second;
	std::pair<float, float> StartButtonPosition = std::make_pair(Game->CameraTransform_Menu.first + WindowSize.first / 2, Game->CameraTransform_Menu.second + WindowSize.second / 2 - 110);

	Menu_Button_Start = Menu_Blueprints->Create_Button(StartButtonPosition.first, StartButtonPosition.second, 210, 60, 259, 74, "Assets/Sprites/Button_StartGame_BMP.bmp");
	Menu_Button_Quit = Menu_Blueprints->Create_Button(StartButtonPosition.first, StartButtonPosition.second + 240, 130, 60, 162, 74, "Assets/Sprites/Button_Quit_BMP.bmp");
	Menu_Background = Menu_Blueprints->Create_Background(Game->CameraTransform_Menu.first + WindowSize.first / 2, Game->CameraTransform_Menu.second + WindowSize.second / 2, WindowResolution, "Assets/Sprites/MenuBackground_Default_", Mapper_Resolutions);
	// =============================== SAMPLE CODE ===============================
		

	//Function pointers
	Game->Set_EventCallback(HandleEvents);
	Game->Set_LogicCallback(HandleLogic);
	Game->Set_RenderCallback(HandleRendering);

	Game->Set_MenuEventCallback(HandleMenuEvents);
	Game->Set_MenuLogicCallback(HandleMenuLogic);
	Game->Set_MenuRenderCallback(HandleMenuRendering);

	//Mainloop
	Game->RunGameLoop();
	SDL_Quit();
	return 0;
}
