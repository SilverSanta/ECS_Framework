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

		std::cout << "SDL could not bi initialized: " << SDL_GetError() << "\n\n";
	}
	else {

		std::cout << "SDL video system is ready to go\n\n";
	}

	WindowWidth = windowsize.first;
	WindowHeight = windowsize.second;
	FrameRate = framerate;	

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_SHOWN);
	//screen = SDL_GetWindowSurface(window);
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
bool MyFramework::Get_IsRunning(){

	return bGameIsRunning;
}
uint32_t MyFramework::Get_CurrentTime(){

	return CurrentTime;
	std::cout << CurrentTime << std::endl;
}
uint32_t MyFramework::Get_DeltaTicks(){

	return DeltaTicks;
}

void MyFramework::RunGameLoop(){

	while (IsRunning()) {
		
		HandleLoopStart();
		if (MoveToNextFrame()) {
			//User input
			m_EventCallback();

			//Game logic
			m_LogicCallback();

			//Rendering
			SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(m_renderer);
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
			m_RenderCallback();
			SDL_RenderPresent(m_renderer);
		}
		HandleLoopEnd();
	}
}
bool MyFramework::IsRunning(){

	return bGameIsRunning;
}

bool MyFramework::MoveToNextFrame(){

	return bNextFrame;
}

void MyFramework::Set_Resolution(int w, int h){

	WindowWidth = w;
	WindowHeight = h;
}
void MyFramework::Set_FrameRate(int framerate){

	FrameRate = framerate;
}
void MyFramework::Set_Tick(uint32_t &tick){

	tick = SDL_GetTicks();
}
void MyFramework::Set_IsRunning(bool isrunning){

	bGameIsRunning = isrunning;
}

void MyFramework::HandleLoopStart(){
	
	CurrentTime = SDL_GetTicks();
	Tick_Second = SDL_GetTicks();

	DeltaTicks = Tick_Second - Tick_First;

	bNextFrame = (DeltaTicks > 1000.f / FrameRate) ? (true) : (false);
}
void MyFramework::HandleLoopEnd(){
	
	if (bNextFrame == true) {

		Tick_First = Tick_Second;
	}
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
void MyFramework::RenderCallback() {

		m_RenderCallback();
	}
void MyFramework::MenuLoop() {

}