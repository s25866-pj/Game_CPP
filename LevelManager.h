#ifndef UNTITLED_LEVELMANAGER_H
#define UNTITLED_LEVELMANAGER_H

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <SDL2/SDL.h>

int SPRITE_WIDTH = 32, SPRITE_HEIGHT = 32;

using namespace std;

const char* levelReader = "Level_1.png";

class LevelManager {
public:
    SDL_Texture* backgroundTexture;

    LevelManager();
    ~LevelManager();

    bool loadBackground(SDL_Renderer* renderer);
    void renderBackground(SDL_Renderer* renderer);
};

LevelManager::LevelManager() : backgroundTexture(nullptr) {}

LevelManager::~LevelManager() {
    if (backgroundTexture != nullptr) {
        SDL_DestroyTexture(backgroundTexture);
    }
}

bool LevelManager::loadBackground(SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(levelReader);
    if (!tempSurface) {
        std::cerr << "Unable to load image " << levelReader << "! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    if (!backgroundTexture) {
        std::cerr << "Unable to create texture from " << levelReader << "! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void LevelManager::renderBackground(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
}

#endif //UNTITLED_LEVELMANAGER_H
