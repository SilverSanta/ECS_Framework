#include "SDL.h"
#include "glad.h"
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <unordered_map>


#include "ECS_Framework.h"
#include "ResourceManager.h"

MyFramework::MyFramework(const char* title, std::pair<uint16_t, uint16_t> windowsize, uint8_t framerate){

	SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		std::cout << "SDL could not be initialized: " << SDL_GetError() << "\n\n";
	}
	else {

		std::cout << "SDL video system is ready to go\n\n";
	}
		
	WindowWidth = windowsize.first;
	WindowHeight = windowsize.second;
	FrameRate = framerate;	

	CameraTransform_SavedInGame = std::make_pair(0.f, 0.f);
	CameraTransform_Menu = std::make_pair(10000.f, 0.f);
	CameraTransform_InGameForAnchoring = std::make_pair(0.f, 0.f);
	MousePos = std::make_pair(0, 0);
	bInMenu = true;

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

MyFramework::~MyFramework(){

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Renderer* MyFramework::Get_Renderer() const{

	return m_renderer;
}
SDL_Window* MyFramework::Get_Window(){
	return m_window;
}
std::pair<uint16_t, uint16_t> MyFramework::Get_WindowDimensions(){

	return std::make_pair(WindowWidth,WindowHeight);
}

uint64_t MyFramework::Get_CurrentTickInGame(){

	return Tick_CurrentInGame;
}
uint64_t MyFramework::Get_CurrentTickInMenu() {

	return Tick_CurrentInMenu;
}
uint64_t MyFramework::Get_DeltaTicksInGame(){

	return DeltaTicks_InGame;
}

bool MyFramework::ProgramIsRunning() {

	return bProgramIsRunning;
}
bool MyFramework::InMenu()
{
	return bInMenu;
}
bool MyFramework::GameIsRunning() {

	return bGameIsRunning;
}

void MyFramework::RunGameLoop()
{
	while (ProgramIsRunning() == true)
	{
		if (InMenu() == true || GameIsRunning() == false)
		{
			HandleLoopStart_InMenu();			

			if (bSufficientTimePassedBetweenFrames == true)
			{
				MenuEventCallback();
				MenuLogicCallback();
				MenuRenderCallback();
			}
		}
		else if(GameIsRunning() == true)
		{
			HandleLoopStart_InGame();

			if (bSufficientTimePassedBetweenFrames == true)
			{
				EventCallback();
				LogicCallback();
				RenderCallback();
			}			
		}	
	}
}
void MyFramework::StartGame()
{
	bGameIsRunning = true;
}
void MyFramework::EndGame()
{
	bGameIsRunning = false;
}

void MyFramework::Set_Resolution(int w, int h){

	WindowWidth = w;
	WindowHeight = h;
}
void MyFramework::Set_FrameRate(int framerate){

	FrameRate = framerate;
}

void MyFramework::Set_ProgramIsRunning(bool programisrunning){

	bProgramIsRunning = programisrunning;
}
void MyFramework::Set_InMenu(bool inmenu)
{
	bInMenu = inmenu;
}

void MyFramework::HandleLoopStart_InGame()
{	
	Tick_GetCurrentTick();
	bSufficientTimePassedBetweenFrames = (Tick_Current - (Tick_CurrentInGame + Tick_CurrentInMenu) >= 1000.f / FrameRate) ? (true) : (false);
	if (bSufficientTimePassedBetweenFrames == true)
	{
		Tick_PreviousInGame = Tick_CurrentInGame;
		Tick_CurrentInGame = Tick_Current - Tick_CurrentInMenu;
		DeltaTicks_InGame = Tick_CurrentInGame - Tick_PreviousInGame;
	}
}
void MyFramework::HandleLoopStart_InMenu()
{
	Tick_GetCurrentTick();
	bSufficientTimePassedBetweenFrames = (Tick_Current - (Tick_CurrentInGame + Tick_CurrentInMenu) >= 1000.f / FrameRate) ? (true) : (false);
	if (bSufficientTimePassedBetweenFrames == true)
	{
		Tick_PreviousInMenu = Tick_CurrentInMenu;
		Tick_CurrentInMenu = Tick_Current - Tick_CurrentInGame;
		DeltaTicks_InMenu = Tick_CurrentInMenu - Tick_PreviousInMenu;
	}
}

void MyFramework::Set_EventCallback(std::function<void(void)> func) {

	m_EventCallback = func;
}
void MyFramework::Set_RenderCallback(std::function<void(void)> func) {

	m_RenderCallback = func;
}
void MyFramework::Set_LogicCallback(std::function<void(void)> func){

	m_LogicCallback = func;
}
void MyFramework::EventCallback() {

		m_EventCallback();
	}
void MyFramework::LogicCallback() {

		m_LogicCallback();
	}
void MyFramework::RenderCallback()
{	
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	m_RenderCallback();
	SDL_RenderPresent(m_renderer);
}

void MyFramework::Set_MenuEventCallback(std::function<void(void)> func)
{
	m_MenuEventCallback = func;
}
void MyFramework::Set_MenuRenderCallback(std::function<void(void)> func)
{
	m_MenuRenderCallback = func;
}
void MyFramework::Set_MenuLogicCallback(std::function<void(void)> func)
{
	m_MenuLogicCallback = func;
}
void MyFramework::MenuEventCallback()
{
	m_MenuEventCallback();
}
void MyFramework::MenuLogicCallback()
{
	m_MenuLogicCallback();
}
void MyFramework::MenuRenderCallback()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	m_MenuRenderCallback();
	SDL_RenderPresent(m_renderer);
}

void MyFramework::Tick_GetCurrentTick()
{
	Tick_Current = SDL_GetTicks64();
}

void MyFramework::Set_Pixel(SDL_Surface* surface, int x, int y, uint8_t b, uint8_t g, uint8_t r) {
	SDL_LockSurface(surface);
	std::cout << "left mouse was pressed (" << x << ", " << y << ")\n";
	uint8_t* pixelArray = (uint8_t*)surface->pixels;

	int EntireRowLength = surface->pitch;
	int XScalarBytesPerPixel = surface->format->BytesPerPixel;

	pixelArray[y * EntireRowLength + x * XScalarBytesPerPixel + 0] = b;
	pixelArray[y * EntireRowLength + x * XScalarBytesPerPixel + 1] = r;
	pixelArray[y * EntireRowLength + x * XScalarBytesPerPixel + 2] = g;

	SDL_UnlockSurface(surface);
}



