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

	Entity* MousePtr = Manager_Entities->_Get_EntityById(Mouse_InGame);
	TransformComponent* Mouse_InGame_TransformPtr = Manager_Components->_Get_ComponentPtrFromId(MousePtr->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->m_Components_Transform);
	StateComponent* Mouse_InGame_StatePtr = Manager_Components->_Get_ComponentPtrFromId(MousePtr->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->m_Components_State);

	while (SDL_PollEvent(&event))
	{		
		auto DeltaTime = Game->_Get_DeltaTicksInGame();	

		SDL_PumpEvents();
		SDL_GetMouseState(&Game->m_MousePos.first, &Game->m_MousePos.second);

		CameraComponent* Camera_InGame_ComponentPtr = Manager_Components->_Get_ComponentPtrFromId(Manager_Entities->_Get_EntityById(Camera_InGame)->_Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->m_Components_Camera);
		TransformComponent* Camera_InGame_TransformPtr = Manager_Components->_Get_ComponentPtrFromId(Manager_Entities->_Get_EntityById(Camera_InGame)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->m_Components_Transform);

		// EXITING THE PROGRAM
		if (event.type == SDL_QUIT)
		{
			Game->_Set_ProgramIsRunning(false);
		}

		// MOUSE MOTION
		if (event.type == SDL_MOUSEMOTION)
		{
			Mouse_InGame_TransformPtr->m_PreviousFrame_X = Mouse_InGame_TransformPtr->m_X;
			Mouse_InGame_TransformPtr->m_PreviousFrame_Y = Mouse_InGame_TransformPtr->m_Y;

			Mouse_InGame_TransformPtr->m_X = (float)Game->m_MousePos.first;
			Mouse_InGame_TransformPtr->m_Y = (float)Game->m_MousePos.second;
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
						Manager_Systems->m_System_Camera->_MoveCameraInDirection(Camera_InGame_ComponentPtr, cameramovementvector, DeltaTime);
					}					
				}
			}

			// letters
			if (event.key.keysym.sym == SDLK_m)
			{
				for (auto& Entity : Manager_Entities->m_Entities)
				{
					if (Entity._Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT) != NULL && Entity._Get_ComponentIdFromEntityByType(user::ComponentType::STATE) != NULL && Entity._Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM) != NULL)
					{
						MovementComponent* MovementPtr = Manager_Components->_Get_ComponentPtrFromId(Entity._Get_ComponentIdFromEntityByType(user::ComponentType::MOVEMENT), Manager_Components->m_Components_Movement);
						TransformComponent* TransformPtr = Manager_Components->_Get_ComponentPtrFromId(Entity._Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->m_Components_Transform);
						StateComponent* StatePtr = Manager_Components->_Get_ComponentPtrFromId(Entity._Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->m_Components_State);

						Manager_Systems->m_System_Action->_Action_WalkTo(MovementPtr, TransformPtr, StatePtr, (float)Game->m_MousePos.first + Camera_InGame_TransformPtr->m_X, (float)Game->m_MousePos.second + Camera_InGame_TransformPtr->m_Y);
					}
				}
			}
			else if (event.key.keysym.sym == SDLK_a)
			{
				for (auto& entity : Manager_Entities->m_Entities)
				{
					Manager_Systems->m_System_Action->_Action_Attack(entity._Get_EntityId(), entity._Get_EntityId());
				}
			}
			else if (event.key.keysym.sym == SDLK_c)
			{
				Blueprints->_Create_Goblin((float)Game->m_MousePos.first + Camera_InGame_TransformPtr->m_X, (float)Game->m_MousePos.second + Camera_InGame_TransformPtr->m_Y, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
			}
			else if (event.key.keysym.sym == SDLK_d)
			{
				for (auto& entity : Manager_Entities->m_Entities)
				{			
					if (entity._Get_EntityId() != Mouse_InGame)
					{
						Manager_Systems->m_System_Action->_Action_DropDead(entity._Get_EntityId());
					}					
				}
			}
			else if (event.key.keysym.sym == SDLK_g)
			{
				for (auto& Grid : Manager_Components->m_Components_CellGrid)
				{
					if (Grid.m_bShowGrid == true) { Grid.m_bShowGrid = false; }
					else { Grid.m_bShowGrid = true; }
				}
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (Game->_InMenu() == false)
				{
					CameraComponent* Camera_InMenuPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Menu_Components->m_Components_Camera);
					TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->m_Components_Transform);

					// (1) Switch the bInMenu to be TRUE 
					Game->_Set_InMenu(true);
					
					// (2) Save INGAME camera position to the GAME
					Game->m_CameraTransform_SavedInGame = std::make_pair(Camera_InGame_TransformPtr->m_X, Camera_InGame_TransformPtr->m_Y);

					// (3) Set transform of INGAME to some garbage far away value
					Camera_InGame_TransformPtr->m_X = -100000;
					Camera_InGame_TransformPtr->m_Y = -100000;

					// (4) Set transform of MENU to GAME MENU value
					Camera_InMenu_TransformPtr->m_X = Game->m_CameraTransform_Menu.first;
					Camera_InMenu_TransformPtr->m_Y = Game->m_CameraTransform_Menu.second;				
				}				
			}
		}		
	}
}
void HandleLogic()
{
	Manager_Components->_UpdateRepositories(Game->_Get_CurrentTickInGame());
	Manager_Systems->m_System_Movement->_MoveAll(Game->_Get_DeltaTicksInGame());
	Manager_Systems->m_System_Collision->_Update_AllCollisionShapePositions();
	Manager_Systems->m_System_Animation->_NextFrame_AllSprites();
	Manager_Systems->m_System_Animation->_Update_AllAnimationsToState();
	Manager_Systems->m_System_State->_UpdateState_AllPreviousToCurrent();
	Manager_Systems->m_System_State->_CheckForDead_All();

	Entity* MouseEnt = Manager_Entities->_Get_EntityById(Mouse_InGame);
	StateComponent* MouseStatePtr = Manager_Components->_Get_ComponentPtrFromId(MouseEnt->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Manager_Components->m_Components_State);

	Entity* CameraEnt = Manager_Entities->_Get_EntityById(Camera_InGame);
	CameraComponent* CameraComponentPtr = Manager_Components->_Get_ComponentPtrFromId(CameraEnt->_Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->m_Components_Camera);

}
void HandleRendering()
{
	TransformComponent* Camera_InGame_TransformPtr = Manager_Components->_Get_ComponentPtrFromId(Manager_Entities->_Get_EntityById(Camera_InGame)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->m_Components_Transform);

	// (1) Set Window color - very improbable colour is used for this purpose (magenta)
	SDL_SetRenderDrawColor(Game->_Get_Renderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);

	// (2) Clear the view
	SDL_RenderClear(Game->_Get_Renderer());

	// (3) Render (send ti buffer)
	Manager_Systems->m_System_Render->_RenderAllSprites(Game->_Get_Renderer(), Camera_InGame_TransformPtr);
	Manager_Systems->m_System_Render->_RenderAllTexts(Game->_Get_Renderer(), Camera_InGame_TransformPtr);
	Manager_Systems->m_System_Render->_RenderAllGrids(Game->_Get_Renderer(), Camera_InGame_TransformPtr);
	Manager_Systems->m_System_Render->_RenderAllCollisionShapes(Game->_Get_Renderer(), Camera_InGame_TransformPtr);

		
	// (4) Buffer to screen
	SDL_RenderPresent(Game->_Get_Renderer());
}

void HandleMenuEvents()
{
	const Uint8* keyboard_state_array = SDL_GetKeyboardState(NULL);
	SDL_Event event;

	CameraComponent* Camera_InMenu_ComponentPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Menu_Components->m_Components_Camera);
	TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->m_Components_Transform);

	Entity* Mouse_InMenuPtr = Menu_Entities->_Get_EntityById(Mouse_InMenu);
	TransformComponent* Mouse_InMenu_TransformPtr = Menu_Components->_Get_ComponentPtrFromId(Mouse_InMenuPtr->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->m_Components_Transform);
	StateComponent* Mouse_InMenu_StatePtr = Menu_Components->_Get_ComponentPtrFromId(Mouse_InMenuPtr->_Get_ComponentIdFromEntityByType(user::ComponentType::STATE), Menu_Components->m_Components_State);
	
	while (SDL_PollEvent(&event))
	{
		auto DeltaTime = Game->_Get_DeltaTicksInGame();
		SDL_PumpEvents();
		SDL_GetMouseState(&Game->m_MousePos.first, &Game->m_MousePos.second);

		// EXITING THE PROGRAM
		if (event.type == SDL_QUIT)
		{
			Game->_Set_ProgramIsRunning(false);
		}

		// LEFT-CLICKS IN MENU
		if (event.button.button == SDL_BUTTON_LEFT && event.type != SDL_MOUSEMOTION)
		{
			CollisionComponent* Mouse_InGame_CollisionPtr = Menu_Components->_Get_ComponentPtrFromId(Mouse_InMenuPtr->_Get_ComponentIdFromEntityByType(user::ComponentType::COLLISION1), Menu_Components->m_Components_Collision);

			std::cout << "LEFT CLICK\n";			
			for (auto& CollisionComp : Menu_Components->m_Components_Collision)
			{
				if (CollisionComp.m_Id != Mouse_InGame_CollisionPtr->m_Id)
				{
					if (Menu_Systems->m_System_Collision->_CollisionCheck(Mouse_InGame_CollisionPtr, &CollisionComp))
					{	
						if (CollisionComp._Get_OwnerId() == Menu_Button_Start)
						{
							Game->_StartGame();
							Game->_Set_InMenu(false);
														
							Menu_Components->_DeleteComponents(Menu_Entities->_DeleteEntity(Menu_Button_Start));
						}
						else if (CollisionComp._Get_OwnerId() == Menu_Button_Quit)
						{
							Game->_Set_ProgramIsRunning(false);
						}
					}
				}				
			}
		}

		// MOUSE MOTION
		if (event.type == SDL_MOUSEMOTION)
		{
			Mouse_InMenu_TransformPtr->m_PreviousFrame_X = Mouse_InMenu_TransformPtr->m_X;
			Mouse_InMenu_TransformPtr->m_PreviousFrame_Y = Mouse_InMenu_TransformPtr->m_Y;

			Mouse_InMenu_TransformPtr->m_X = (float)Game->m_MousePos.first;
			Mouse_InMenu_TransformPtr->m_Y = (float)Game->m_MousePos.second;
		}

		// MENU_INGAME SWITCH
		if (event.type == SDL_KEYDOWN)
		{

			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				if (Game->_InMenu() == true && Game->_GameIsRunning() == true)
				{
					CameraComponent* Camera_InGamePtr = Manager_Components->_Get_ComponentPtrFromId(Manager_Entities->_Get_EntityById(Camera_InGame)->_Get_ComponentIdFromEntityByType(user::ComponentType::CAMERA), Manager_Components->m_Components_Camera);
					TransformComponent* Camera_InGame_TransformPtr = Manager_Components->_Get_ComponentPtrFromId(Manager_Entities->_Get_EntityById(Camera_InGame)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Manager_Components->m_Components_Transform);

					// (1) Switch the bInMenu to be FALSE
					Game->_Set_InMenu(false);					

					// (2) Set transform of MENU to some garbage far away value
					Camera_InMenu_TransformPtr->m_X = -100000;
					Camera_InMenu_TransformPtr->m_Y = -100000;

					// (3) Set transform of INGAME to GAME INGAME value
					Camera_InGame_TransformPtr->m_X = Game->m_CameraTransform_SavedInGame.first;
					Camera_InGame_TransformPtr->m_Y = Game->m_CameraTransform_SavedInGame.second;
				}
			}
		}
	}
}
void HandleMenuLogic()
{
	Menu_Components->_UpdateRepositories(Game->_Get_CurrentTickInMenu());
	Menu_Systems->m_System_Movement->_MoveAll(Game->_Get_DeltaTicksInGame());
	Menu_Systems->m_System_Collision->_Update_AllCollisionShapePositions();
	Menu_Systems->m_System_Animation->_NextFrame_AllSprites();
	Menu_Systems->m_System_Animation->_Update_AllAnimationsToState();
	Menu_Systems->m_System_State->_UpdateState_AllPreviousToCurrent();
	Menu_Systems->m_System_State->_CheckForDead_All();
}
void HandleMenuRendering()
{
	TransformComponent* Camera_Menu_TransformPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->m_Components_Transform);

	// (1) Set Window color - very improbable colour is used for this purpose (magenta)
	SDL_SetRenderDrawColor(Game->_Get_Renderer(), 255, 0, 255, SDL_ALPHA_OPAQUE);

	// (2) Clear the view
	SDL_RenderClear(Game->_Get_Renderer());

	// (3) Render (send ti buffer)
	Menu_Systems->m_System_Render->_RenderAllSprites(Game->_Get_Renderer(), Camera_Menu_TransformPtr);
	Menu_Systems->m_System_Render->_RenderAllCollisionShapes(Game->_Get_Renderer(), Camera_Menu_TransformPtr);

	// (4) Buffer to screen
	SDL_RenderPresent(Game->_Get_Renderer());
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

	Game->m_CameraTransform_InGameForAnchoring = std::make_pair(0.f, 0.f);
	Game->m_CameraTransform_SavedInGame = std::make_pair(0.f, 0.f);
	Game->m_CameraTransform_Menu = std::make_pair(0.f, 0.f);
	Game->m_MousePos = std::make_pair(0, 0);

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
	Mouse_InGame = Blueprints->_Create_MouseCursor();
	Camera_InGame = Blueprints->_Create_Camera(0.5f, -500.f, 500.f, -300.f, 300.f);


	// MENU OBJECTS
	Mouse_InMenu = Menu_Blueprints->_Create_MouseCursor();
	Camera_InMenu = Menu_Blueprints->_Create_Camera(0.f, -Game->m_CameraTransform_Menu.first + WindowSize.first / 2, Game->m_CameraTransform_Menu.first + WindowSize.first / 2, -Game->m_CameraTransform_Menu.second + WindowSize.second / 2, Game->m_CameraTransform_Menu.second + WindowSize.second / 2);


	// =============================== SAMPLE CODE ===============================
	auto SomeGoblin1 = Blueprints->_Create_Goblin(32, 32, "Assets/Sprites/Sheets/goblinBMP.bmp", "Assets/Sprites/Sheets/HealthBarBMP.bmp");
	auto SomeGrid = Blueprints->_Create_Grid(16, 16, 20, 20, 32, 32);
	auto SomeText = Blueprints->_Create_Text(400, 300, true, 72, "THIS IS JUST A SAMPLE", "Assets/Fonts/VT323-Regular.ttf", user::Alignment::MIDDLE_CENTRE, mfk::ColourList::RED_OPAQUE, user::Layer::TEXT);
	auto Instruction_C = Blueprints->_Create_Text(400, 360, true, 72, "Press 'C' to create a goblin", "Assets/Fonts/VT323-Regular.ttf", user::Alignment::MIDDLE_CENTRE, mfk::ColourList::WHITE_OPAQUE, user::Layer::TEXT);
	auto Instruction_M = Blueprints->_Create_Text(400, 420, true, 72, "Press 'M' to Move all units", "Assets/Fonts/VT323-Regular.ttf", user::Alignment::MIDDLE_CENTRE, mfk::ColourList::WHITE_OPAQUE, user::Layer::TEXT);
	auto Instruction_D = Blueprints->_Create_Text(400, 480, true, 72, "Press 'D' to kill all units", "Assets/Fonts/VT323-Regular.ttf", user::Alignment::MIDDLE_CENTRE, mfk::ColourList::WHITE_OPAQUE, user::Layer::TEXT);
	auto Instruction_A = Blueprints->_Create_Text(400, 540, true, 72, "Press 'A' to attack", "Assets/Fonts/VT323-Regular.ttf", user::Alignment::MIDDLE_CENTRE, mfk::ColourList::WHITE_OPAQUE, user::Layer::TEXT);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			auto Tile = Blueprints->_Create_Tile((float)(16 + j * 64), (float)(16 + i * 64), "Assets/Sprites/Sheets/Tile_GroundBMP.bmp");
		}
	}
	
	TransformComponent* Camera_InMenu_TransformPtr = Menu_Components->_Get_ComponentPtrFromId(Menu_Entities->_Get_EntityById(Camera_InMenu)->_Get_ComponentIdFromEntityByType(user::ComponentType::TRANSFORM), Menu_Components->m_Components_Transform);
	Camera_InMenu_TransformPtr->m_X = Game->m_CameraTransform_Menu.first;
	Camera_InMenu_TransformPtr->m_Y = Game->m_CameraTransform_Menu.second;
	std::pair<float, float> StartButtonPosition = std::make_pair(Game->m_CameraTransform_Menu.first + WindowSize.first / 2, Game->m_CameraTransform_Menu.second + WindowSize.second / 2 - 110);

	Menu_Button_Start = Menu_Blueprints->_Create_Button(StartButtonPosition.first, StartButtonPosition.second, 210, 60, 259, 74, "Assets/Sprites/Button_StartGame_BMP.bmp");
	Menu_Button_Quit = Menu_Blueprints->_Create_Button(StartButtonPosition.first, StartButtonPosition.second + 240, 130, 60, 162, 74, "Assets/Sprites/Button_Quit_BMP.bmp");
	Menu_Background = Menu_Blueprints->_Create_Background(Game->m_CameraTransform_Menu.first + WindowSize.first / 2, Game->m_CameraTransform_Menu.second + WindowSize.second / 2, WindowResolution, "Assets/Sprites/MenuBackground_Default_", Mapper_Resolutions);
	// =============================== SAMPLE CODE ===============================
		

	//Function pointers
	Game->_Set_EventCallback(HandleEvents);
	Game->_Set_LogicCallback(HandleLogic);
	Game->_Set_RenderCallback(HandleRendering);

	Game->_Set_MenuEventCallback(HandleMenuEvents);
	Game->_Set_MenuLogicCallback(HandleMenuLogic);
	Game->_Set_MenuRenderCallback(HandleMenuRendering);

	//Mainloop
	Game->_RunGameLoop();
	SDL_Quit();
	return 0;
}
