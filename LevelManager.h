//
// Created by damian on 15.05.24.
//

#ifndef UNTITLED_LEVELMANAGER_H
#define UNTITLED_LEVELMANAGER_H

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>

const char* image = "outside_sprites.png";
const char* levelReader = "Level_1.png";
Uint32 get_pixel(SDL_Surface *surface, int x, int y) {
    // Pobranie formatu powierzchni
    SDL_PixelFormat *fmt = surface->format;

    // Obliczenie adresu piksela w pamięci
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(Uint32);

    // Wartość piksela
    return *(Uint32 *)p;
}
int ** GetLevelData();
    SDL_Surface* createLevel(){
        SDL_Surface* surface = nullptr;

        // Wczytanie obrazu
        SDL_Surface* levelSurface = IMG_Load(levelReader);
        if (!levelSurface) {
            // Obsługa błędu wczytywania obrazu
            return nullptr;
        }

        // Utworzenie powierzchni SDL o takich samych wymiarach jak obraz
        surface = SDL_CreateRGBSurface(0, levelSurface->w, levelSurface->h, 32, 0, 0, 0, 0);
        if (!surface) {
            // Obsługa błędu tworzenia powierzchni SDL
            SDL_FreeSurface(levelSurface);
            return nullptr;
        }

        // Kopiowanie pikseli z obrazu levelSurface do surface
        SDL_BlitSurface(levelSurface, nullptr, surface, nullptr);

        // Zwolnienie obrazu levelSurface, ponieważ jego piksele zostały już skopiowane
        SDL_FreeSurface(levelSurface);

        // Iterowanie po pikselach powierzchni i pobranie koloru
        for (int y = 0; y < surface->h; ++y) {
            for (int x = 0; x < surface->w; ++x) {
                Uint32 pixel = get_pixel(surface, x, y);

            }
        }

        return surface;
    }
int ** GetLevelData(){
    int** array=new int*[13];
    for (int i = 0; i < 13; ++i) {
        array[i]=new int[20];
    }
return array;
}
#endif //UNTITLED_LEVELMANAGER_H
