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
	SDL_Renderer* _Get_Renderer() const;
	SDL_Window* _Get_Window();
	std::pair<uint16_t, uint16_t> _Get_WindowDimensions();

	uint64_t _Get_CurrentTickInGame();
	uint64_t _Get_CurrentTickInMenu();
	uint64_t _Get_DeltaTicksInGame();

	//Run game loop
	void _RunGameLoop();
	void _StartGame();
	void _EndGame();

	//Booleans for main loop
	bool _ProgramIsRunning();
	bool _InMenu();
	bool _GameIsRunning();

	//Setters
	void _Set_Resolution(int w, int h);
	void _Set_FrameRate(int framerate);
	void _Set_ProgramIsRunning(bool isrunning);
	void _Set_InMenu(bool inmenu);

	//Handlers of framerate
	void _HandleLoopStart_InGame();
	void _HandleLoopStart_InMenu();
	
	//Setting callbacks
	void _Set_EventCallback(std::function<void(void)> func);
	void _Set_RenderCallback(std::function<void(void)> func);
	void _Set_LogicCallback(std::function<void(void)> func);
	void _EventCallback();
	void _LogicCallback();	
	void _RenderCallback();

	void _Set_MenuEventCallback(std::function<void(void)> func);
	void _Set_MenuRenderCallback(std::function<void(void)> func);
	void _Set_MenuLogicCallback(std::function<void(void)> func);
	void _MenuEventCallback();
	void _MenuLogicCallback();
	void _MenuRenderCallback();

	//Misc
	void _Set_Pixel(SDL_Surface* surface, int x, int y, uint8_t b, uint8_t g, uint8_t r);

	//Ticks
	void _Tick_GetCurrentTick();


public:
	//Camera transforms (ingame, menu -> for switching between them)
	std::pair<float, float> m_CameraTransform_SavedInGame; //LAST LOCATION INGAME BEFORE GOING TO MENU
	std::pair<float, float> m_CameraTransform_Menu; //LOCATION OF MENU
	std::pair<float, float> m_CameraTransform_InGameForAnchoring; //ANCHOR LOCATION FOR DRAGGING WITH MOUSE

	//Mouse position (both ingame and in menu
	std::pair<int, int> m_MousePos;

private:
	//Window dimensions
	int m_WindowWidth;
	int m_WindowHeight;

	//For constructor
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screen = nullptr;
	SDL_Renderer* m_renderer = nullptr;	


	//Ticks
	uint64_t m_Tick_Current{ 0 };

	//Ticks - in game
	uint64_t m_Tick_CurrentInGame{ 0 };
	uint64_t m_Tick_PreviousInGame{ 0 };
	uint64_t m_DeltaTicks_InGame{ 0 };

	//Ticks - in menu
	uint64_t m_Tick_CurrentInMenu{ 0 };
	uint64_t m_Tick_PreviousInMenu{ 0 };
	uint64_t m_DeltaTicks_InMenu{ 0 };

	int m_FrameRate = 30;
	bool m_bSufficientTimePassedBetweenFrames = false;

	//Loop bools
	bool m_bProgramIsRunning = true; // Entire program (so menu loop AND game loop)
	bool m_bInMenu = true; // if grue-> menu loop, if false -> ingame loop
	bool m_bGameIsRunning = false; // starts false (game needs to start from menu) and then is TRUE until game is DONE (which should return to main menu)


	//Callbacks
	std::function<void(void)> m_EventCallback;
	std::function<void(void)> m_RenderCallback;
	std::function<void(void)> m_LogicCallback;

	std::function<void(void)> m_MenuEventCallback;
	std::function<void(void)> m_MenuRenderCallback;
	std::function<void(void)> m_MenuLogicCallback;
};

