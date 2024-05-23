//
// Created by damian on 23.05.24.
//

#ifndef UNTITLED_PIXEL_H
#define UNTITLED_PIXEL_H
#include <SDL2/SDL_image.h>
#include <vector>
int windowW=640,windowH=400;
Uint32 getPixel(SDL_Surface* surface, int x, int y);
SDL_Color generateColor(int i, int j);
int getColorFromImage(char x, int i, int j);
using namespace std;
struct Pixel{
    int pos_X;
    int pos_Y;
    bool solid;
    SDL_Color color;
    int size_X;
    int size_Y;
};
vector<vector<Pixel>>CreteMask(){
    int x= 13, y=20;
    vector<vector<Pixel>> playerMap(20,vector<Pixel>(13));
    for(int i = 0;i<y;i++){
        for (int j=0;j<x;j++){
            if(generateColor(i,j).r==0 && generateColor(i,j).g==0 && generateColor(i,j).g==0){
                playerMap[i][j]=Pixel{i,j,true,generateColor(i,j),windowW/x,windowW/y};
            }else{
                playerMap[i][j]=Pixel{i,j,false,generateColor(i,j),windowW/x,windowW/y};
            }
        }
    }
    return playerMap;
}
SDL_Color generateColor(int i, int j) {
    SDL_Color color;
    color.r = getColorFromImage('r',i,j);
    color.g = getColorFromImage('g',i,j);
    color.b = getColorFromImage('b',i,j);
    color.a = 255; // Fully opaque
    return color;
}

int getColorFromImage(char x, int i, int j) {
    SDL_Surface* imageSurface = IMG_Load("Level_1.png");
    if (!imageSurface) {
        printf("SDL_LoadBMP error: %s\n", SDL_GetError());
        return 1;
    }
    if (!imageSurface) {
    }

    Uint32 pixel = getPixel(imageSurface, i, j);
    Uint8 r, g, b;
    SDL_GetRGB(pixel, imageSurface->format, &r, &g, &b);

    switch (x) {
        case 'r':
            return r;
        case 'g':
            return g;
        case 'b':
            return b;
        default:
            return 0;
    }
}
Uint32 getPixel(SDL_Surface* surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;
        case 2:
            return *(Uint16*)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
            return *(Uint32*)p;
        default:
            return 0;
    }
}

#endif //UNTITLED_PIXEL_H
