#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>

using namespace std;
int windowW=640,windowH=400;
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

    bool isEntityOnFlor() {
        if(pos_X>0){
            return false;
        }
        return true;
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
//        if (pos_Y <= jumpStartPos_Y - maxJumpHeight) {
//            jumping = false;
//        }

        if (!onFloor) {
            pos_Y += airSpeed;
            airSpeed += gravity;
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
SDL_Surface *loadAnimations(Player &player, int imageIndex, int i1) {
    SDL_Surface* surface = importIMG(player);
    SDL_Surface*** oneImage = new SDL_Surface**[7];
    int cols[] = {5, 6, 3, 1, 3, 4, 8};
    for (int i = 0; i < 7; ++i) {
        oneImage[i] = new SDL_Surface*[cols[i]];
        for (int j = 0; j < cols[i]; ++j) {
            oneImage[i][j] = SDL_CreateRGBSurface(0, 64, 40, 32, 0, 0, 0, 0);
            SDL_Rect srcRect = {j * 64, i * 40, 64, 40};  // Ustawienie źródłowego prostokąta na odpowiedniej pozycji
            SDL_Rect destRect = {0, 0, 64, 40};
            SDL_BlitSurface(surface, &srcRect, oneImage[i][j], &destRect);
        }
    }
    SDL_Surface* firstFrame = oneImage[i1][imageIndex];  // Zwrócenie pierwszej ramki z danej animacji
    delete[] oneImage;  // Zwolnienie pamięci
    SDL_FreeSurface(surface);  // Zwolnienie powierzchni wczytanego obrazu
    return firstFrame;
}
int main(int argc, char* args[]) {
    Player player;
    int actionType=0;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Moja Gra w CPP",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          windowW, windowH,
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
    SDL_Texture* texture;
    SDL_Rect rect = { player.pos_X, player.pos_Y, player.width, player.height };

    SDL_Event event;
    bool quit = false;
    float imageCounter=0;
    while (!quit) {
        if(player.moving){
            actionType=1;
        }
        else{
            actionType=0;
        }
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    player.handleInput(event.key.keysym.sym, event.type == SDL_KEYDOWN);
                     // Pass window size here
                    break;
            }
        }
        player.updatePosition();
        if (player.isEntityOnFloor()) {
            player.onFloor = true;
            player.inAir = false;
            player.airSpeed = 0;
        } else {
            player.onFloor = false;
        }
        SDL_Surface* surface = loadAnimations(player, (int) imageCounter, actionType);
        if (!surface) {
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Nie można utworzyć tekstury: " << SDL_GetError() << std::endl;
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        rect.x = player.pos_X;
        rect.y = player.pos_Y;

        SDL_SetRenderDrawColor(renderer, 242, 5, 191, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
        if(imageCounter<4){
            imageCounter+=0.25;
        }else{
            imageCounter=0;
        }
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
