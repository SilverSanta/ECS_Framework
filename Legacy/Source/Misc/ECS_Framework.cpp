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
		
	m_WindowWidth = windowsize.first;
	m_WindowHeight = windowsize.second;
	m_FrameRate = framerate;	

	m_CameraTransform_SavedInGame = std::make_pair(0.f, 0.f);
	m_CameraTransform_Menu = std::make_pair(10000.f, 0.f);
	m_CameraTransform_InGameForAnchoring = std::make_pair(0.f, 0.f);
	m_MousePos = std::make_pair(0, 0);
	m_bInMenu = true;

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
}

MyFramework::~MyFramework(){

	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Renderer* MyFramework::_Get_Renderer() const{

	return m_renderer;
}
SDL_Window* MyFramework::_Get_Window(){
	return m_window;
}
std::pair<uint16_t, uint16_t> MyFramework::_Get_WindowDimensions(){

	return std::make_pair(m_WindowWidth,m_WindowHeight);
}

uint64_t MyFramework::_Get_CurrentTickInGame(){

	return m_Tick_CurrentInGame;
}
uint64_t MyFramework::_Get_CurrentTickInMenu() {

	return m_Tick_CurrentInMenu;
}
uint64_t MyFramework::_Get_DeltaTicksInGame(){

	return m_DeltaTicks_InGame;
}

bool MyFramework::_ProgramIsRunning() {

	return m_bProgramIsRunning;
}
bool MyFramework::_InMenu()
{
	return m_bInMenu;
}
bool MyFramework::_GameIsRunning() {

	return m_bGameIsRunning;
}

void MyFramework::_RunGameLoop()
{
	while (_ProgramIsRunning() == true)
	{
		if (_InMenu() == true || _GameIsRunning() == false)
		{
			_HandleLoopStart_InMenu();			

			if (m_bSufficientTimePassedBetweenFrames == true)
			{
				_MenuEventCallback();
				_MenuLogicCallback();
				_MenuRenderCallback();
			}
		}
		else if(_GameIsRunning() == true)
		{
			_HandleLoopStart_InGame();

			if (m_bSufficientTimePassedBetweenFrames == true)
			{
				_EventCallback();
				_LogicCallback();
				_RenderCallback();
			}			
		}	
	}
}
void MyFramework::_StartGame()
{
	m_bGameIsRunning = true;
}
void MyFramework::_EndGame()
{
	m_bGameIsRunning = false;
}

void MyFramework::_Set_Resolution(int w, int h){

	m_WindowWidth = w;
	m_WindowHeight = h;
}
void MyFramework::_Set_FrameRate(int framerate){

	m_FrameRate = framerate;
}

void MyFramework::_Set_ProgramIsRunning(bool programisrunning){

	m_bProgramIsRunning = programisrunning;
}
void MyFramework::_Set_InMenu(bool inmenu)
{
	m_bInMenu = inmenu;
}

void MyFramework::_HandleLoopStart_InGame()
{	
	_Tick_GetCurrentTick();
	m_bSufficientTimePassedBetweenFrames = (m_Tick_Current - (m_Tick_CurrentInGame + m_Tick_CurrentInMenu) >= 1000.f / m_FrameRate) ? (true) : (false);
	if (m_bSufficientTimePassedBetweenFrames == true)
	{
		m_Tick_PreviousInGame = m_Tick_CurrentInGame;
		m_Tick_CurrentInGame = m_Tick_Current - m_Tick_CurrentInMenu;
		m_DeltaTicks_InGame = m_Tick_CurrentInGame - m_Tick_PreviousInGame;
	}
}
void MyFramework::_HandleLoopStart_InMenu()
{
	_Tick_GetCurrentTick();
	m_bSufficientTimePassedBetweenFrames = (m_Tick_Current - (m_Tick_CurrentInGame + m_Tick_CurrentInMenu) >= 1000.f / m_FrameRate) ? (true) : (false);
	if (m_bSufficientTimePassedBetweenFrames == true)
	{
		m_Tick_PreviousInMenu = m_Tick_CurrentInMenu;
		m_Tick_CurrentInMenu = m_Tick_Current - m_Tick_CurrentInGame;
		m_DeltaTicks_InMenu = m_Tick_CurrentInMenu - m_Tick_PreviousInMenu;
	}
}

void MyFramework::_Set_EventCallback(std::function<void(void)> func) {

	m_EventCallback = func;
}
void MyFramework::_Set_RenderCallback(std::function<void(void)> func) {

	m_RenderCallback = func;
}
void MyFramework::_Set_LogicCallback(std::function<void(void)> func){

	m_LogicCallback = func;
}
void MyFramework::_EventCallback() {

		m_EventCallback();
	}
void MyFramework::_LogicCallback() {

		m_LogicCallback();
	}
void MyFramework::_RenderCallback()
{	
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	m_RenderCallback();
	SDL_RenderPresent(m_renderer);
}

void MyFramework::_Set_MenuEventCallback(std::function<void(void)> func)
{
	m_MenuEventCallback = func;
}
void MyFramework::_Set_MenuRenderCallback(std::function<void(void)> func)
{
	m_MenuRenderCallback = func;
}
void MyFramework::_Set_MenuLogicCallback(std::function<void(void)> func)
{
	m_MenuLogicCallback = func;
}
void MyFramework::_MenuEventCallback()
{
	m_MenuEventCallback();
}
void MyFramework::_MenuLogicCallback()
{
	m_MenuLogicCallback();
}
void MyFramework::_MenuRenderCallback()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_renderer);
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	m_MenuRenderCallback();
	SDL_RenderPresent(m_renderer);
}

void MyFramework::_Tick_GetCurrentTick()
{
	m_Tick_Current = SDL_GetTicks64();
}

void MyFramework::_Set_Pixel(SDL_Surface* surface, int x, int y, uint8_t b, uint8_t g, uint8_t r) {
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



