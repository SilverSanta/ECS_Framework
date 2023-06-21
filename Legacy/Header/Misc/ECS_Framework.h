#pragma once
#include <iostream>
#include <utility>
#include <unordered_map>
#include <functional>
#include <vector>




//forward declarations
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Window;
class TexturedRectangle;

class MyFramework
{
	//CONSTRUCTORS
public:
	MyFramework(const char* title, std::pair<uint16_t, uint16_t> windowsize, uint8_t framerate);
	~MyFramework();

	//FUNCTIONS
public:
	//Getters
	SDL_Renderer* Get_Renderer() const;
	SDL_Window* Get_Window();
	std::pair<uint16_t, uint16_t> Get_WindowDimensions();


	uint64_t Get_CurrentTickInGame();
	uint64_t Get_CurrentTickInMenu();
	uint64_t Get_DeltaTicksInGame();


	//Run game loop
	void RunGameLoop();
	void StartGame();
	void EndGame();


	//Booleans for main loop
	bool ProgramIsRunning();
	bool InMenu();
	bool GameIsRunning();


	//Setters
	void Set_Resolution(int w, int h);
	void Set_FrameRate(int framerate);
	void Set_ProgramIsRunning(bool isrunning);
	void Set_InMenu(bool inmenu);
	void Set_GameIsRunning(bool isrunning);

	//Handlers of framerate
	void HandleLoopStart_InGame();
	void HandleLoopStart_InMenu();
	
	//Setting callbacks
	void Set_EventCallback(std::function<void(void)> func);
	void Set_RenderCallback(std::function<void(void)> func);
	void Set_LogicCallback(std::function<void(void)> func);
	void EventCallback();
	void LogicCallback();	
	void RenderCallback();

	void Set_MenuEventCallback(std::function<void(void)> func);
	void Set_MenuRenderCallback(std::function<void(void)> func);
	void Set_MenuLogicCallback(std::function<void(void)> func);
	void MenuEventCallback();
	void MenuLogicCallback();
	void MenuRenderCallback();

	//Misc
	void Set_Pixel(SDL_Surface* surface, int x, int y, uint8_t b, uint8_t g, uint8_t r);

	//Ticks
	void Tick_GetCurrentTick();


public:
	//Camera transforms (ingame, menu -> for switching between them)
	std::pair<float, float> CameraTransform_SavedInGame; //LAST LOCATION INGAME BEFORE GOING TO MENU
	std::pair<float, float> CameraTransform_Menu; //LOCATION OF MENU
	std::pair<float, float> CameraTransform_InGameForAnchoring; //ANCHOR LOCATION FOR DRAGGING WITH MOUSE

	//Mouse position (both ingame and in menu
	std::pair<int, int> MousePos;

private:
	//Window dimensions
	int WindowWidth;
	int WindowHeight;

	//For constructor
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screen = nullptr;
	SDL_Renderer* m_renderer = nullptr;	


	//Ticks
	uint64_t Tick_Current{ 0 };

	//Ticks - in game
	uint64_t Tick_CurrentInGame{ 0 };
	uint64_t Tick_PreviousInGame{ 0 };
	uint64_t DeltaTicks_InGame{ 0 };

	//Ticks - in menu
	uint64_t Tick_CurrentInMenu{ 0 };
	uint64_t Tick_PreviousInMenu{ 0 };
	uint64_t DeltaTicks_InMenu{ 0 };

	int FrameRate = 30;
	bool bSufficientTimePassedBetweenFrames = false;

	//Loop bools
	bool bProgramIsRunning = true; // Entire program (so menu loop AND game loop)
	bool bInMenu = true; // if grue-> menu loop, if false -> ingame loop
	bool bGameIsRunning = false; // starts false (game needs to start from menu) and then is TRUE until game is DONE (which should return to main menu)


	//Callbacks
	std::function<void(void)> m_EventCallback;
	std::function<void(void)> m_RenderCallback;
	std::function<void(void)> m_LogicCallback;

	std::function<void(void)> m_MenuEventCallback;
	std::function<void(void)> m_MenuRenderCallback;
	std::function<void(void)> m_MenuLogicCallback;
};

