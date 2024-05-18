//
// Created by damian on 15.05.24.
//

#ifndef UNTITLED_LOADANIMATION_H
#define UNTITLED_LOADANIMATION_H

#include <SDL2/SDL_surface.h>
#include "Player.h"
#include "ImportIMG.h"

SDL_Surface *loadAnimations(Player &player, int imageIndex, int i1) {
    SDL_Surface* surface = importIMG(player);
    if (!surface) {
        return nullptr;
    }

    // Ustawienie koloru przezroczystego (zakładam, że kolor przezroczysty to magenta - RGB(255, 0, 255))
    Uint32 colorkey = SDL_MapRGB(surface->format, 255, 0, 255);
    SDL_SetColorKey(surface, SDL_TRUE, colorkey);

    SDL_Surface*** oneImage = new SDL_Surface**[7];
    int cols[] = {5, 6, 3, 1, 3, 4, 8};
    for (int i = 0; i < 7; ++i) {
        oneImage[i] = new SDL_Surface*[cols[i]];
        for (int j = 0; j < cols[i]; ++j) {
            oneImage[i][j] = SDL_CreateRGBSurface(0, 64, 40, 32, 0, 0, 0, 0);
            if (!oneImage[i][j]) {
                // Obsługa błędów tworzenia powierzchni
                for (int k = 0; k <= i; ++k) {
                    for (int l = 0; l < cols[k]; ++l) {
                        SDL_FreeSurface(oneImage[k][l]);
                    }
                    delete[] oneImage[k];
                }
                delete[] oneImage;
                SDL_FreeSurface(surface);
                return nullptr;
            }

            // Ustawienie koloru przezroczystego dla nowej powierzchni
            SDL_SetColorKey(oneImage[i][j], SDL_TRUE, colorkey);

            SDL_Rect srcRect = {j * 64, i * 40, 64, 40};  // Ustawienie źródłowego prostokąta na odpowiedniej pozycji
            SDL_Rect destRect = {0, 0, 64, 40};
            SDL_BlitSurface(surface, &srcRect, oneImage[i][j], &destRect);
        }
    }
    SDL_Surface* firstFrame = oneImage[i1][imageIndex];  // Zwrócenie pierwszej ramki z danej animacji

    // Zwolnienie pozostałych powierzchni
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < cols[i]; ++j) {
            if (!(i == i1 && j == imageIndex)) {
                SDL_FreeSurface(oneImage[i][j]);
            }
        }
        delete[] oneImage[i];
    }
    delete[] oneImage;

    SDL_FreeSurface(surface);  // Zwolnienie powierzchni wczytanego obrazu
    return firstFrame;
}
#endif //UNTITLED_LOADANIMATION_H
