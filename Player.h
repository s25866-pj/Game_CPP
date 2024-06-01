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
    int pos_Y = 150;
    int width = (int)(32*1.5);
    int height = (int)(20*1.5);
    int speed = 5;
    // Hitbox adjustments
    int hitboxOffsetLeft = 10;
    int hitboxOffsetRight = 5;
    int hitboxWidth = width - hitboxOffsetLeft - hitboxOffsetRight;

    // Gravity and jump
    int jumpStartPos_Y = 0;
    int maxJumpHeight = 50; // Zmniejszona maksymalna wysokość skoku
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
                airSpeed = jumpSpeed;
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
        // Check the pixels directly below the player
        newPixelLH = (pos_X + hitboxOffsetLeft) / pixel;
        newPixelLD = (pos_Y + height + 1) / pixel;
        newPixelRH = (pos_X + hitboxOffsetLeft + hitboxWidth) / pixel;
        newPixelRD = (pos_Y + height + 1) / pixel;

        return masks[newPixelLH][newPixelLD].solid || masks[newPixelRH][newPixelRD].solid;
    }

    bool isEntityOnCeiling() {
        // Check the pixels directly above the player
        newPixelLH = (pos_X + hitboxOffsetLeft) / pixel;
        newPixelLU = (pos_Y - 1) / pixel;
        newPixelRH = (pos_X + hitboxOffsetLeft + hitboxWidth) / pixel;
        newPixelRU = (pos_Y - 1) / pixel;

        return masks[newPixelLH][newPixelLU].solid || masks[newPixelRH][newPixelRU].solid;
    }

    bool isEntityOnLeftWall() {
        // Check the pixels directly to the left of the player
        newPixelLL = (pos_X + hitboxOffsetLeft - 1) / pixel;
        newPixelUL = pos_Y / pixel;
        newPixelDL = (pos_Y + height) / pixel;

        return masks[newPixelLL][newPixelUL].solid || masks[newPixelLL][newPixelDL].solid;
    }

    bool isEntityOnRightWall() {
        // Check the pixels directly to the right of the player
        newPixelRL = (pos_X + hitboxOffsetLeft + hitboxWidth + 1) / pixel;
        newPixelUR = pos_Y / pixel;
        newPixelDR = (pos_Y + height) / pixel;

        return masks[newPixelRL][newPixelUR].solid || masks[newPixelRL][newPixelDR].solid;
    }

    int pixel = 32, oldPixelLH = 0, oldPixelLD = 0, oldPixelRH = 0, oldPixelRD = 0, newPixelLH, newPixelLD, newPixelRH, newPixelRD, newPixelLU, newPixelRU, newPixelLL, newPixelUL, newPixelDL, newPixelRL, newPixelUR, newPixelDR;

    bool checkPlayerPixel() {
        newPixelLH = (pos_X + hitboxOffsetLeft) / pixel;
        newPixelLD = (pos_Y + height) / pixel;
        newPixelRH = (pos_X + hitboxOffsetLeft + hitboxWidth) / pixel;
        newPixelRD = (pos_Y + height) / pixel;

//        cout << masks[newPixelLH][newPixelLD].solid << "|" << masks[newPixelRH][newPixelRD].solid << endl;
        if (masks[newPixelLH][newPixelLD].solid || masks[newPixelRH][newPixelRD].solid) {
            return false;
        } else {
            return true;
        }
    }

    void updatePosition() {
        // Handle horizontal movement
        if (left && !right) {
            pos_X -= speed;
            if (isEntityOnLeftWall()) {
                pos_X += speed; // revert movement
            }
        } else if (!left && right) {
            pos_X += speed;
            if (isEntityOnRightWall()) {
                pos_X -= speed; // revert movement
            }
        }

        // Handle vertical movement
        if (jumping && inAir) {
            pos_Y += jumpSpeed;
            airSpeed = jumpSpeed;
            if (isEntityOnCeiling()) {
                pos_Y -= jumpSpeed; // revert movement
                airSpeed = -airSpeed; // bounce off the ceiling
                jumping = false;
            }
        } else if (!onFloor) {
            pos_Y += airSpeed;
            airSpeed += gravity;
            if (isEntityOnCeiling()) {
                pos_Y -= airSpeed; // revert movement
                airSpeed = -airSpeed; // bounce off the ceiling
            }
        }

        if (pos_Y <= jumpStartPos_Y - maxJumpHeight) {
            jumping = false;
        }

        if (!onFloor) {
            pos_Y += airSpeed;
            airSpeed += gravity;
        }

        // Check if the player is on the floor
        onFloor = isEntityOnFloor();
        if (onFloor) {
            airSpeed = 0;
            inAir = false;
            pos_Y -= 1; // Adjust position to be exactly on the floor
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
