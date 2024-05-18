#include <SDL2/SDL_image.h>
#include <iostream>
#include "Player.h"
#include "ImportIMG.h"
#include "LoadAnimation.h"
#include "LevelManager.h"

using namespace std;

int main(int argc, char* args[]) {
    Player player;
    int actionType = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Moja Gra w CPP",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          windowW, windowH,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Nie można utworzyć okna: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Nie można utworzyć renderera: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    LevelManager levelManager;
    if (!levelManager.loadBackground(renderer)) {
        std::cerr << "Failed to load background!" << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    const char* levelReader = "Level_1.png";  // Provide the correct path to your level image
    SDL_Surface* tempSurface = IMG_Load(levelReader);
    if (!tempSurface) {
        std::cerr << "Unable to load image " << levelReader << "! SDL_image Error: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    player.backgroundSurface = tempSurface;

    SDL_Texture* texture = nullptr;
    SDL_Rect rect = { player.pos_X, player.pos_Y, player.width, player.height };
    SDL_Event event;
    bool quit = false;
    float imageCounter = 0;

    while (!quit) {
        if (player.moving) {
            actionType = 1;
        } else {
            actionType = 0;
        }

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    player.handleInput(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                    break;
            }
        }

        player.updatePosition();
        rect.x = player.pos_X;
        rect.y = player.pos_Y;

        SDL_Surface* surface = loadAnimations(player, (int)imageCounter, actionType);
        if (!surface) {
            std::cerr << "Nie można wczytać animacji: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Nie można utworzyć tekstury: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return 1;
        }

        SDL_RenderClear(renderer);
        levelManager.renderBackground(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);

        if (imageCounter < 4) {
            imageCounter += 0.25;
        } else {
            imageCounter = 0;
        }

        SDL_Delay(25);
    }

    std::cout << "Dziękuję za grę" << std::endl;
    SDL_Delay(1000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}