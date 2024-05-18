#ifndef UNTITLED_PLAYER_H
#define UNTITLED_PLAYER_H

#include <SDL2/SDL.h>

const int windowW = 640 * 2;
const int windowH = 400 * 2;

class Player {
public:
    int pos_X = 100;
    int pos_Y = 400;
    int width = 64;
    int height = 40;
    int speed = 5;
    // gravity and jump
    int jumpStartPos_Y = 0;
    int maxJumpHeight = 100;
    float jumpSpeed = -2.25f;
    float gravity = 0.04f;
    float airSpeed = 0;

    bool left = false, right = false, moving = false, jumping = false, inAir = false, onFloor = false;
    const char* image = "player_sprites.png";

    SDL_Surface* backgroundSurface;

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
                jumpStartPos_Y = pos_Y;
                airSpeed = jumpSpeed;
            }
        } else {
            if (sym == SDLK_a) {
                left = false;
            }
            if (sym == SDLK_d) {
                right = false;
            }
        }
        moving = left || right;
    }

    void updatePosition() {
        int new_X = pos_X;
        int new_Y = pos_Y;

        if (left && !right) {
            new_X -= speed;
        } else if (!left && right) {
            new_X += speed;
        }

        if (jumping && inAir) {
            new_Y += static_cast<int>(airSpeed);
            if (pos_Y <= jumpStartPos_Y - maxJumpHeight) {
                jumping = false;
            }
        } else if (!onFloor) {
            new_Y += static_cast<int>(airSpeed);
            airSpeed += gravity;
        }

        if (canMoveTo(new_X, new_Y)) {
            pos_X = new_X;
            pos_Y = new_Y;
        } else {
            airSpeed = 0; // Stop falling if hitting a solid block
        }
    }

    bool canMoveTo(int new_X, int new_Y) {
        // Check all corners of the player sprite
        return isWhitePixel(backgroundSurface, new_X, new_Y) &&
               isWhitePixel(backgroundSurface, new_X + width - 1, new_Y) &&
               isWhitePixel(backgroundSurface, new_X, new_Y + height - 1) &&
               isWhitePixel(backgroundSurface, new_X + width - 1, new_Y + height - 1);
    }

    bool isWhitePixel(SDL_Surface* surface, int x, int y) {
        if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
            return false;
        }
        Uint32 pixel = ((Uint32*)surface->pixels)[(y * surface->w) + x];
        Uint8 r, g, b;
        SDL_GetRGB(pixel, surface->format, &r, &g, &b);
        std::cout<<"biale tlo"<<std::endl;
        return (r == 255 && g == 255 && b == 255);
    }
};

#endif // UNTITLED_PLAYER_H
