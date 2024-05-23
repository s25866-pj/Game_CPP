//
// Created by damian on 15.05.24.
//

#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <SDL2/SDL_keycode.h>
#include <vector>
#include "Pixel.h"
#include "LoadLevel.h"

using namespace std;

class Player {
public:
    int pos_X = 0;
    int pos_Y = 0;
    int width = 64;
    int height = 40;
    int speed = 5;
    //gravity and jump
    int jumpStartPos_Y = 0;
    int maxJumpHeight=100;
    float jumpSpeed=-2.25;
    float gravity = 0.04;
    float airSpeed = 0;

    bool left = false, right = false, moving = false,jumping = false,inAir=false,onFloor=false;
    const char* image = "player_sprites.png";
    vector<vector<Pixel>> masks =CreteMask();
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
                inAir=true;
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

        // Update moving state based on input
        moving = left || right;
    }

    bool isEntityOnFloor() {
        return pos_Y + height >= windowH;
    }

    int pixelW =32,pixelH=32,oldPixelW=0,oldPixelH=0,newPixelW,newPixelH;
    bool checkPlayerPixel(vector<vector<Pixel>> vector1) {
        newPixelH=(int)pos_X/pixelH;
        newPixelW=(int)pos_Y/pixelW;

        if(newPixelH!=oldPixelH || newPixelW!=oldPixelW){
            oldPixelH=newPixelH;
            oldPixelW=newPixelW;
            cout<<vector1[newPixelH][newPixelW].solid<<endl;
            return vector1[newPixelH][newPixelW].solid;

            //cout<<newPixelW<<"-"<<oldPixelW<<" | "<<newPixelH<<"-"<<oldPixelH<<endl;

        }
    }
    void updatePosition() {
        if (left && !right) {
            if(checkPlayerPixel(masks)){
                pos_X -= speed;
            }

        } else if (!left && right) {
            if(checkPlayerPixel(masks)){
                pos_X += speed;
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
#endif //UNTITLED_PLAYER_H
