#include <SDL2/SDL.h>
#include <iostream>
#include "Player.h"
#include "ImportIMG.h"
#include "LoadAnimation.h"
using namespace std;
void addBackground(SDL_Renderer* renderer) {
    // Wczytaj obraz tła
    SDL_Surface* backgroundSurface = IMG_Load("background.jpg"); // Zmień na odpowiednią ścieżkę i nazwę Twojego obrazu tła
    if (!backgroundSurface) {
        std::cerr << "Nie można wczytać obrazu tła: " << SDL_GetError() << std::endl;
        return;
    }

    // Utwórz teksturę z obrazu tła
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (!backgroundTexture) {
        std::cerr << "Nie można utworzyć tekstury tła: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(backgroundSurface);
        return;
    }

    // Renderuj tło na całej powierzchni okna
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    // Zwolnij pamięć zajętą przez powierzchnię obrazu tła
    SDL_FreeSurface(backgroundSurface);
    // Nie zwalniaj tekstury, ponieważ będzie używana w pętli gry
}
int main(int argc, char* args[]) {
    Player player;
    int actionType=0;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Moja Gra w CPP",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          windowW, windowH,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Nie można utworzyć okna: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Nie można utworzyć renderera: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    addBackground(renderer);
    SDL_Texture* texture;
    SDL_Rect rect = { player.pos_X, player.pos_Y, player.width, player.height };

    SDL_Event event;
    bool quit = false;
    float imageCounter=0;
    while (!quit) {
        if(player.moving){
            actionType=1;
        }
        else{
            actionType=0;
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    player.handleInput(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                     // Pass window size here
                    break;
            }
        }
        player.updatePosition();
        if (player.touchSolid()) {
            player.onFloor = true;
            player.inAir = false;
            player.airSpeed = 0;
        } else {
            player.onFloor = false;
        }
        SDL_Surface* surface = loadAnimations(player, (int) imageCounter, actionType);
        if (!surface) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Nie można utworzyć tekstury: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        rect.x = player.pos_X;
        rect.y = player.pos_Y;

        SDL_SetRenderDrawColor(renderer, 242, 5, 191, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        if(imageCounter<4){
            imageCounter+=0.25;
        }else{
            imageCounter=0;
        }
        SDL_Delay(25);
    }

    std::cout << "Dziękuję za grę";
    SDL_Delay(1000);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
