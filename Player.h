//
// Created by damian on 15.05.24.
//

#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <SDL2/SDL_keycode.h>
int windowW=640*2,windowH=416*2;
class Player {
public:
    int pos_X = 0;
    int pos_Y = 0;
    int width = 64*2;
    int height = 40*2;
    int speed = 5;
    //gravity and jump
    int jumpStartPos_Y = 0;
    int maxJumpHeight=100;
    float jumpSpeed=-2.25;
    float gravity = 0.04;
    float airSpeed = 0;

    bool left = false, right = false, moving = false,jumping = false,inAir=false,onFloor=false;
    const char* image = "player_sprites.png";
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
    void updatePosition() {
        if (left && !right) {
            pos_X -= speed;
        } else if (!left && right) {
            pos_X += speed;
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

    bool touchSolid() {

        return false;
    }
};
#endif //UNTITLED_PLAYER_H
