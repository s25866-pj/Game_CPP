#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Player.h"
#include "ImportIMG.h"
#include "LoadAnimation.h"
#include "LoadLevel.h"

using namespace std;
SDL_Texture* DrawMap(SDL_Renderer* renderer, vector<vector<Pixel>> &vector1);
int main(int argc, char* args[]) {
    Player player;
    vector<vector<Pixel>> masks =CreteMask();
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
    SDL_Texture* texture;
    SDL_Rect rect = { player.pos_X, player.pos_Y, player.width, player.height };
    SDL_Rect backgroundRect={0,0,windowW,windowH};
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
        if (player.isEntityOnFloor()) {
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

        checkPlayerPixel(player,masks);
        SDL_RenderCopy(renderer, DrawMap(renderer, masks), NULL, &backgroundRect);
//        SDL_RenderClear(renderer);
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
SDL_Texture* DrawMap(SDL_Renderer* renderer, vector<vector<Pixel>> &vector1) {
    int pixelWidth = vector1[0][0].size_X;
    int pixelHeight = vector1[0][0].size_Y;
    int textureWidth = pixelWidth * vector1.size();
    int textureHeight = pixelHeight * vector1[0].size();

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textureWidth, textureHeight);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < vector1.size(); i++) {
        for (size_t j = 0; j < vector1[i].size(); j++) {
            SDL_Rect rect = {static_cast<int>(i * pixelWidth), static_cast<int>(j * pixelHeight), pixelWidth, pixelHeight};
            SDL_Color color = vector1[i][j].color;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_SetRenderTarget(renderer, NULL);
    return texture;
}