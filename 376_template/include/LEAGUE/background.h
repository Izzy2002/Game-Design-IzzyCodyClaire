#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL2/SDL.h>

class Background {
public:
    Background();
    ~Background();

    // Load the background image from a file
    bool load(const char* filename, SDL_Renderer* renderer);

    // Render the background
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
    SDL_Rect rect;
};

#endif

