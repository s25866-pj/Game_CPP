#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_image.h>
#include <vector>

int main(int argc, char* args[] ) {
    int speed = 5;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool quit=false;
    std::vector<SDL_Texture*> fragments;
    const int WindowWidth = 640, WindowHeight = 480;
    int x=0,y=0;
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* surface = IMG_Load("kiwi_sprite2.png");
    if(!surface){
        std::cout<<"nie mozna wczytac obrazka"<<std::endl;
        return 1;
    }
    auto window = SDL_CreateWindow( "Moja Gra w CPP",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    WindowWidth, WindowHeight, // Poprawiono WindowHeigth na WindowHeight
                                    SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect rect={x,y,100,100};

    while(!quit){
        while(SDL_PollEvent(&event)){
            switch (event.type) { // Zmieniono warunek na event.type
                case SDL_QUIT: // Zmieniono na SDL_QUIT
                    quit=true;
                    break;
                case SDL_KEYDOWN: // Zmieniono na SDL_KEYDOWN
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit=true;
                            break;
                        case SDLK_w :
                            if(y>0){
                                y-=speed;
                                rect.y=y;
                            }
                            break;
                        case SDLK_a :
                            if(x>0){
                                x-=speed;
                                rect.x=x;
                            }
                            break;
                        case SDLK_s :
                            if(y<WindowHeight-100){
                                y+=speed;
                                rect.y=y;
                            }
                            break;
                        case SDLK_d :
                            if(x<WindowWidth-100){
                                x+=speed;
                                rect.x=x;
                            }
                            break;
                    }
                    break;
            }
        }

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
