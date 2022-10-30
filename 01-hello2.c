#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Hello World!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren,255,255,0,0x00);
    SDL_RenderClear(ren);

    circleRGBA(ren, 100, 100, 50, 0xFF, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    filledEllipseRGBA(ren, 200, 200, 50, 75, 0xFF, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    roundedRectangleRGBA(ren, 300, 300, 450, 475, 10, 0x00, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    arcRGBA(ren, 375, 300, 75,180,0, 0xFF, 0x00, 0xFF, SDL_ALPHA_OPAQUE);
    thickLineRGBA(ren, 50, 50, 150, 150, 10, 0x00, 0xFF, 0x00, SDL_ALPHA_OPAQUE);


    SDL_RenderPresent(ren);
    SDL_Delay(5000);

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
