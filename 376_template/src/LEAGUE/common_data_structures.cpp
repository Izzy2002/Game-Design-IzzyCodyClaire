#include "LEAGUE/common_data_structures.h"
#include "LEAGUE/engine.h"
#include <SDL2/SDL_image.h>
#include <string>

Drawable::Drawable(){

}

// Destructor.  When this object is destroyed, we need to
// be sure to destroy (give back the memory of) the texture.
Drawable::~Drawable(){
	SDL_DestroyTexture(texture);
}

bool Drawable::loadImage(const char* path){
	// We load an image into a Surface, but this is
	// software rendered.  So, we then make a Texture
	// out of it.  Textures are hardware accelerated.
	surface = IMG_Load(path);
	if(!surface){
		SDL_Log("Cannot load surface: %s", SDL_GetError());
		return false;		
	}
	texture = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), surface);
	if(!texture){
		SDL_Log("Cannot create texture: %s", SDL_GetError());
		return false;
	}
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	return true;
}

void Scene::addDrawable(Drawable& d){
	drawables.push_back(d);
}

void Scene::addUpdateable(Updateable& u){
	updateables.push_back(u);
}

// We are using reference wrappers, as we can't store references
// to an object in a vector (easily) otherwise.  We could choose
// to simply not pass-by-reference, but I want you to learn how
// to do it.
std::vector<std::reference_wrapper<Drawable>> Scene::getDrawables(){
	return drawables;
}

std::vector<std::reference_wrapper<Updateable>> Scene::getUpdateables(){
	return updateables;
}
