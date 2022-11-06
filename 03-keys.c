#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_HEIGHT 100
#define WINDOW_WIDTH 200

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect rect[10];
    int count = 0;//contar o número de rectangulos
    SDL_Event evt;
    int close_request = 0;

    while (!close_request) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        srand(time(0));
        for (int i = 0; i < count; i++) {
            int r = (rand() % 255) + 1;
            int g = (rand() % 255) + 1;
            int b = (rand() % 255) + 1;
            SDL_SetRenderDrawColor(ren, r, g, b, 0x00);
            SDL_RenderFillRect(ren, &rect[i]);

        }
        SDL_RenderPresent(ren);

        SDL_WaitEvent(&evt);
        if (evt.type == SDL_KEYDOWN) {
            switch (evt.key.keysym.sym) {
                case SDLK_UP:
                    if (r.y > 0) {
                        r.y -= 5;
                    }
                    break;
                case SDLK_DOWN:
                    if (r.y < WINDOW_HEIGHT - r.h) {
                        r.y += 5;
                    }
                    break;
                case SDLK_LEFT:
                    if (r.x > 0) {
                        r.x -= 5;
                    }
                    break;
                case SDLK_RIGHT:
                    if (r.x < WINDOW_WIDTH - r.w) {
                        r.x += 5;
                    }
                    break;
            }
        }else if (evt.type == SDL_MOUSEBUTTONDOWN) {
            if (evt.button.button == SDL_BUTTON_LEFT) {
                if (count < 10) {
                    rect[count].x = evt.button.x;
                    rect[count].y = evt.button.y;
                    rect[count].w = 10;
                    rect[count].h = 10;
                    count += 1;
                }
            }
        }else if (evt.type == SDL_QUIT) {
            close_request = 1;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
