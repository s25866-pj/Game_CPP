#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <SDL2/SDL_keycode.h>
#include <vector>
#include "Pixel.h"
#include "LoadLevel.h"

using namespace std;

class Player {
public:
    int pos_X = 50;
    int pos_Y = 50;
    int width = 64;
    int height = 40;
    int speed = 5;
    // gravity and jump
    int jumpStartPos_Y = 0;
    int maxJumpHeight = 100;
    float jumpSpeed = -3.25;
    float gravity = 0.06;
    float airSpeed = 0;

    bool left = false, right = false, moving = false, jumping = false, inAir = false, onFloor = false;
    const char* image = "player_sprites.png";
    vector<vector<Pixel>> masks = CreteMask();

    void handleInput(SDL_Keycode sym, bool keyPressed) {
        if (keyPressed) {
            if (sym == SDLK_a) {
                left = true;
            }
            if (sym == SDLK_d) {
                right = true;
            }
            if (sym == SDLK_SPACE && !jumping && !inAir) {
                jumping = true;
                inAir = true;
            }
        } else {
            if (sym == SDLK_a) {
                left = false;
            }
            if (sym == SDLK_d) {
                right = false;
            }
            if (sym == SDLK_SPACE && jumping) {
                jumpStartPos_Y = pos_Y;
                inAir = true;
                jumping = false;
            }
        }
        moving = left || right;
    }

    bool isEntityOnFloor() {
        return pos_Y + height >= windowH;
    }

    int pixel = 32, oldPixelLH = 0, oldPixelLD = 0, oldPixelRH = 0, oldPixelRD = 0, newPixelLH, newPixelLD, newPixelRH, newPixelRD;

    bool checkPlayerPixel(vector<vector<Pixel>>& vector1) {
        newPixelLH = pos_X / pixel;
        newPixelLD = (pos_Y + height) / pixel;
        newPixelRH = (pos_X + width) / pixel;
        newPixelRD = (pos_Y + height) / pixel;

        cout<<vector1[newPixelLH][newPixelLD].solid<<"|"<<vector1[newPixelRH][newPixelRD].solid<<endl;
        if(vector1[newPixelLH][newPixelLD].solid || vector1[newPixelRH][newPixelRD].solid){
            return false;
        }else{
            return true;
        }


    }

    void updatePosition() {
        if (left && !right) {
            pos_X -= speed;
            if (!checkPlayerPixel(masks)) {
                pos_X += speed; // revert movement
            }
        } else if (!left && right) {
            pos_X += speed;
            if (!checkPlayerPixel(masks)) {
                pos_X -= speed; // revert movement
            }
        }

        if (jumping && inAir) {
            pos_Y += jumpSpeed;
            airSpeed = jumpSpeed;
        } else if (!onFloor) {
            pos_Y += airSpeed;
            airSpeed += gravity;
        }
        if (pos_Y <= jumpStartPos_Y - maxJumpHeight) {
            jumping = false;
        }

        if (!onFloor) {
            pos_Y += airSpeed;
            airSpeed += gravity;
        }
    }
};

SDL_Texture* DrawMap(SDL_Renderer* renderer, vector<vector<Pixel>>& vector1) {
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
#endif // UNTITLED_PLAYER_H
