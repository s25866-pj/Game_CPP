#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>

using namespace std;

class Player {
public:
    int pos_X = 0;
    int pos_Y = 0;
    int width = 100;
    int height = 100;
    int speed = 5;
    bool left = false, right = false, moving = false;
    const char* image = "player_sprites.png";
    float airSpeed = 0;
    float gravity = 0.4;
    float jumpSpeed = -2.25;
    float fallSpeedAfterCollision = 0.5;
    bool inAir = false;

    void handleInput(SDL_Keycode sym, bool keyPressed) {
        if (keyPressed) {
            if (sym == SDLK_a) {
                left = true;
            }
            if (sym == SDLK_d) {
                right = true;
            }
        } else {
            if (sym == SDLK_a) {
                left = false;
            }
            if (sym == SDLK_d) {
                right = false;
            }
        }

        // Update moving state based on input
        moving = left || right;
    }

    void updatePosition(const int WindowWidth, const int WindowHeight) {
        // Update Y position for gravity or jumping

        if (inAir) {
            airSpeed += gravity;
            pos_Y += airSpeed;
        } else {
            airSpeed = 0;  // Reset air speed if on the ground
        }

        // Update X position based on movement
        if (left && !right) {
            pos_X -= speed;
        } else if (!left && right) {
            pos_X += speed;
        }
    }
};
SDL_Surface* importIMG(Player& player) {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surface = IMG_Load(player.image);
    if (!surface) {
        std::cerr << "Nie można wczytać obrazka: " << IMG_GetError() << std::endl;
    }
    return surface;
}
SDL_Surface* loadAnimations(Player& player) {
    SDL_Surface* surface = importIMG(player);
    SDL_Surface* oneImage[5];

    for (int i = 0; i < 5; ++i) {
        oneImage[i]=SDL_CreateRGBSurface(0, 64, 40, 32, 0, 0, 0, 0);
        SDL_Rect srcRect = { i * 64, 0, 40, 40 };
        SDL_Rect destRect = { 0, 0, 64, 40 };
        SDL_BlitSurface(surface, &srcRect, oneImage[i], &destRect);
    }
    return oneImage[0];
}



int main(int argc, char* args[]) {
    Player player;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Moja Gra w CPP",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Nie można utworzyć okna: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Nie można utworzyć renderera: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* surface = loadAnimations(player);
    if (!surface) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Nie można utworzyć tekstury: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Rect rect = { player.pos_X, player.pos_Y, player.width, player.height };

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    player.handleInput(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                    player.updatePosition(640, 480); // Pass window size here
                    break;
            }
        }

        rect.x = player.pos_X;
        rect.y = player.pos_Y;

        SDL_SetRenderDrawColor(renderer, 242, 5, 191, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(25);
    }

    std::cout << "Dziękuję za grę";
    SDL_Delay(1000);

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
