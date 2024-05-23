//
// Created by damian on 23.05.24.
//

#ifndef UNTITLED_PIXEL_H
#define UNTITLED_PIXEL_H
#include <SDL2/SDL_image.h>
struct Pixel{
    int pos_X;
    int pos_Y;
    bool solid;
    SDL_Color color;
    int size_X;
    int size_Y;
};
#endif //UNTITLED_PIXEL_H
