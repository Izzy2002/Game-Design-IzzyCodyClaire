#include "LEAGUE/background.h"
#include <SDL2/SDL_image.h>

Background::Background() : texture(nullptr) {}

Background::~Background() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

bool Background::load(const char* filename, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filename);
    if (!surface) {
        // Error loading image
        return false;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        // Error creating texture
        SDL_FreeSurface(surface);
        return false;
    }
    SDL_FreeSurface(surface);

    // Set the size and position of the background
    SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
    rect.x = 0;
    rect.y = 0;

    return true;
}

void Background::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}


