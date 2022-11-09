#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 100
#define WINDOW_WIDTH 200

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms){
    static int aux;
    Uint32 antes = SDL_GetTicks();
    int evento = SDL_WaitEventTimeout(evt, *ms);
    if (evento) {
        *ms -= (SDL_GetTicks() - antes);
        aux += (SDL_GetTicks() - antes);
        if (*ms <  0) {
            *ms = 0;
        }
    }else{
        *ms  += aux;
        aux = 0;
    }
    return evento;

}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Contagem do tempo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Event evt;
    int close_request = 0;
    int espera = 500;

    while (!close_request) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
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
            }else if (evt.type == SDL_QUIT) {
                close_request = 1;
            }
        } else {
            r.x += 2;
            r.y += 2;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
