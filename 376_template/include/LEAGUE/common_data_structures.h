#ifndef		__H__COMMON_DATA_STRUCTURES__LEAGUE__
#define		__H__COMMON_DATA_STRUCTURES__LEAGUE__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <functional>
#include <utility>
#include <vector>

/**
 * Represents any (every?) thing that exists in the game.
 */

class GameObject {
	protected:
		double x;
		double y;
	public:
		double getX();
		double getY();
		std::pair<double, double> getCoords();
		//bool loadScript(const char* path);
};

/**
 * For objects that need to be drawn, track the rectangle
 * and the draw function for that object.
 */

class Drawable : public virtual GameObject {
	public:
		Drawable();
		~Drawable();
		bool loadImage(const char*);
		virtual void draw(SDL_Renderer* renderer) = 0;	// abstract method
	protected:
		SDL_Surface* surface;	// Surfaces are software rendered
		SDL_Texture* texture;	// Textures have hardware acceleration
		SDL_Rect rect;
		unsigned int layer;
};

/*
 * An object that needs to be updated periodically must have
 * a function that takes a delta time since the prior update.
 */

class Updateable : public virtual GameObject {
	public:
		virtual void update(double delta) = 0;	// Abstract method
};

/**
 * Games are composed of Scenes.  A Scene holds all the game objects
 */

class Scene {
	private:
		// Used to keep track of anything that needs to be drawn
		std::vector<std::reference_wrapper<Drawable>> drawables;
		// Keeps track of anything that should be udpated each frame
		std::vector<std::reference_wrapper<Updateable>> updateables;
	public:
		void addDrawable(Drawable& d);
		void addUpdateable(Updateable& u);
		bool removeDrawable(Drawable& d);
		bool removeUpdateabme(Updateable& u);
		std::vector<std::reference_wrapper<Drawable>> getDrawables();
		std::vector<std::reference_wrapper<Updateable>> getUpdateables();
};

#endif
