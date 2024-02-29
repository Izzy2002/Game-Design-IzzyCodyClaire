#include <iostream>

#include "LEAGUE/engine.h"
#include "LEAGUE/defaults.h"

// Mark these extern as they are C libraries,
// and we don't want their names mangled (will make
// it impossible to link).
extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// Setup default values for our static vars.
Engine* Engine::instance{nullptr};
std::mutex Engine::singleton_lock;
std::vector<SDL_Event> Engine::events;
lua_State* Engine::L;

// Constructor.
Engine::Engine(){
	if(setup() == false){
		SDL_Log("FAILED to setup: %s", SDL_GetError());
		shutdown();
		exit(1);
	}
}

/**
 * Returns the Singleton instance of our Engine class.
 *
 * @return Engine* A pointer to the Engine instance.
 */
Engine* Engine::getInstance(){
	std::lock_guard<std::mutex> lock(singleton_lock);
	if( instance == nullptr){
		instance = new Engine();
	}
	return instance;
}

/**
 * Events are stored each frame so that all GameObjects can
 * query for particular ones they are looking for, without
 * consumption of the event.
 *
 * @return std::vector<SDL_Event> A list of all events captured this frame.
 */

std::vector<SDL_Event> Engine::getEvents(){
	return events;
}

/**
 * Initialize our libraries (if possible).  If we are unable
 * to do so, log messages appropriately and return false.
 *
 * @return bool Whether or not the needed libraries could be
 *              started.
 */
bool Engine::setup(){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("Cannot init video: %s", SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		SDL_Log("Cannot initialize image library: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH_DEFAULT, SCREEN_HEIGHT_DEFAULT,
			SDL_WINDOW_SHOWN);
	if(!window){
		SDL_Log("FAILED to create window: %s", SDL_GetError());
		IMG_Quit();
		SDL_Quit();
		return false;
	}
	int flags = SDL_RENDERER_ACCELERATED;
	renderer = SDL_CreateRenderer(window, -1, flags);

	L = luaL_newstate();
	luaL_openlibs(L);

	return true;
}

/**
 * Game objects will need to access the renderer to draw.
 *
 * @return SDL_Renderer* A pointer to the renderer the Engine is using.
 */ 
SDL_Renderer* Engine::getRenderer(){
	return renderer;
}

/**
 * Core loop.  Needs a Scene of GameObjects.
 * Processes inputs, updates objects, and renders outputs.
 * Then, frame limits and starts over.
 *
 * @param Scene& A reference to a Scene of GameObjects.
 */
void Engine::core_loop(Scene& s){
	SDL_Event event;
	running = true;
	// Timing vars
	long double delta = 0;
	long previousTime = SDL_GetPerformanceCounter();
	long currentTime = previousTime;

	while(running){
		// Get events (note SDL2 won't draw a window if we don't at least
		// check for the SDL_QUIT event).
		events.clear();	
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
			}
			events.push_back(event);
		}

		// Process updates.
		auto updateables = s.getUpdateables();
		for(auto obj = updateables.begin(); obj != updateables.end(); ++obj){
			obj->get().update(delta);
		}

		// Render outputs.
		SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
		SDL_RenderClear(renderer);
		auto drawables = s.getDrawables();
		for(auto obj = drawables.begin(); obj != drawables.end(); ++obj){
			obj->get().draw(renderer);
		} 
		SDL_RenderPresent(renderer);

		// What is going on here???
		// SDL_GetPerformanceFrequency() is a platform-specific counter.  It returns the
		// number of its count per second (on my MacOS machine it counts to 1,000,000,000 each
		// second).
		// SDL_GetPerformanceCounter() gives the current counter value.
		// We measure elapsed time by subtracting our current time from some previous time.  We
		// often will call the previous measure something like "previousTime", even though these
		// aren't time units yet.  So, we get the value of the counter and store it in previousTime,
		// then when we want to know how much time has elapsed, we get the current value of the
		// counter, and divide by the counter frequency.  Dividing by the frequency gives us
		// a value in seconds of how much time has passed.
		// To perform frame limiting, we measure how long each frame took to run.  If we are running
		// at 60FPS, then we want 1000 / 60 = 16.66667 seconds per frame.
		// Usually we are going to be running **much** faster than this.  So, at the end of the
		// loop, we need to wait out the rest of the time (waste some milliseconds) to slow the computer
		// down.  So we call SDL_Delay, and pass in the number of milliseconds we want to waste.
		// SDL_Delay is only accurate to the millisecond level.  And, due to OS scheduling of processes
		// can actually take longer than the time we pass.  But - it is usually good enough.

		// Store the last time value
		previousTime = currentTime;

		// Get the current time value
		currentTime = SDL_GetPerformanceCounter();

		// Calculate the elapsed time (we usually call it delta time in gaming).
		// Divide by the number of counts per second to convert to seconds.
		delta = (currentTime - previousTime) / static_cast<long double>(SDL_GetPerformanceFrequency());

		// If we haven't used all of our time alloted to this frame, waste
		// some time.  We don't want our engine to run too fast and consume too many
		// resources.  ESPECIALLY if we are running on a mobile device (just wastes battery).
		//std::cerr << FRAME_TIME_MS << " - " << delta << " = " << (FRAME_TIME_MS - delta) << std::endl;
		if(FRAME_TIME_MS - delta > 0.01f){
			SDL_Delay((FRAME_TIME_MS * 1000.0f) - (delta * 1000.0f));
			//std::cerr << "Waiting "<< (FRAME_TIME_MS - delta) << std::endl;
		}
	}

}

lua_State* Engine::getLuaState(){
	return L;
}

/**
 * Cleanup the memory we used.
 */
void Engine::shutdown(){
	lua_close(Engine::L);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	IMG_Quit();
	SDL_Quit();
}

