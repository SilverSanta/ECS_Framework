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
	bool Get_IsRunning();
	uint32_t Get_CurrentTime();
	uint32_t Get_DeltaTicks();

	//Run game loop
	void RunGameLoop();

	//Booleans for frame checks
	bool IsRunning();
	bool MoveToNextFrame();

	//Setters
	void Set_Resolution(int w, int h);
	void Set_FrameRate(int framerate);
	void Set_Tick(uint32_t &tick);
	void Set_IsRunning(bool isrunning);

	//Handlers of framerate
	void HandleLoopStart();
	void HandleLoopEnd();
	
	//Setting callbacks
	void Set_EventCallback(std::function<void(void)> func);
	void Set_RenderCallback(std::function<void(void)> func);
	void Set_LogicCallback(std::function<void(void)> func);
	void EventCallback();
	void LogicCallback();
	void RenderCallback();

	//Loops
	void MenuLoop();
	void Set_Pixel(SDL_Surface* surface, int x, int y, uint8_t b, uint8_t g, uint8_t r);

private:
	//Window dimensions
	int WindowWidth;
	int WindowHeight;

	//For constructor
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screen = nullptr;
	SDL_Renderer* m_renderer = nullptr;
	
	//Frame rate
	uint32_t CurrentTime;
	uint32_t Tick_First;
	uint32_t Tick_Second;
	uint32_t DeltaTicks;

	int FrameRate = 10;
	bool bNextFrame = true;

	//Loop bools
	bool bGameIsRunning = true;
	bool bMenuIsOpen = false;

	//Callbacks
	std::function<void(void)> m_EventCallback;
	std::function<void(void)> m_RenderCallback;
	std::function<void(void)> m_LogicCallback;
};

