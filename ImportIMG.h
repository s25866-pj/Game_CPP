//
// Created by damian on 15.05.24.
//

#ifndef UNTITLED_IMPORTIMG_H
#define UNTITLED_IMPORTIMG_H

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "Player.h"

SDL_Surface* importIMG(Player& player) {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surface = IMG_Load(player.image);
    if (!surface) {
        std::cerr << "Nie można wczytać obrazka: " << IMG_GetError() << std::endl;
    }
    return surface;
}

#endif //UNTITLED_IMPORTIMG_H
