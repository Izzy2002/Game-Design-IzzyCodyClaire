#ifndef		__H__ENGINE__LEAGUE__
#define		__H__ENGINE__LEAGUE__

#include <mutex>

extern "C" {
	#include <lua.h>
}

#include <SDL2/SDL.h>
#include "common_data_structures.h"

class Engine {
	public:
		// Remove the copy constructor and operator= from the vtable
		// so we can't mess up the instance.
		Engine(Engine& other) = delete;	// Don't allow cloning the singleton
		void operator=(const Engine& other) = delete;	// Can't set equal either

		// Provides the instance when called.
		static Engine* getInstance();

		bool setup();
		void core_loop(Scene& s);
		void shutdown();
		SDL_Renderer* getRenderer();
		static std::vector<SDL_Event> getEvents();
		static lua_State* getLuaState();
	private:
		// Singleton instance
		static Engine* instance;
		
		// Can't safely make a Singleton with multiple threads without a lock
		static std::mutex singleton_lock;
		
		// Shared LUA state
		static lua_State* L;

		// Keep track of per-frame events
		static std::vector<SDL_Event> events;

		// Tracks whether the core loop is running
		bool running = false;
		
		// The main drawing window
		SDL_Window* window;
		
		// The renderer; in charge of all drawing
		SDL_Renderer* renderer;

	protected:
		// Limit the scope of the constructor and destructor so it can't
		// be called directly by other code.
		Engine();
		~Engine();
};
#endif
