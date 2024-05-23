//
// Created by damian on 23.05.24.
//

#ifndef UNTITLED_LOADLEVEL_H
#define UNTITLED_LOADLEVEL_H
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>


#include "Pixel.h"

using namespace std;


SDL_Color generateColor(int i, int j);

int getColorFromImage(char x,int i, int j);

const char* image = "Level_1.png";
SDL_Surface* CreateBackground(){
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surface = IMG_Load(image);
    if (!surface) {
        std::cerr << "Nie można wczytać obrazka: " << IMG_GetError() << std::endl;
    }
    return surface;
}





#endif //UNTITLED_LOADLEVEL_H
