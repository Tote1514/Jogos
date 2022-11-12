#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 200
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
        *ms += aux;
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
    SDL_Rect r1 = { 40,20, 10,10 };
    SDL_Rect r2 = { 100, 100, 10, 10};
    SDL_Rect r3 = { 100, 50, 10, 10};
    SDL_Event evt;
    int close_request = 0;
    int espera = 500;
    int dx =2; //deslocamento do x
    int dy =2; //deslocamento do y

    while (!close_request) {
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r3);
        SDL_RenderPresent(ren);

        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            if (evt.type == SDL_KEYDOWN) {
                switch (evt.key.keysym.sym) {
                    case SDLK_UP:
                        if (r1.y > 0) {
                            r1.y -= 5;
                        }
                        break;
                    case SDLK_DOWN:
                        if (r1.y < WINDOW_HEIGHT - r1.h) {
                            r1.y += 5;
                        }
                        break;
                    case SDLK_LEFT:
                        if (r1.x > 0) {
                            r1.x -= 5;
                        }
                        break;
                    case SDLK_RIGHT:
                        if (r1.x < WINDOW_WIDTH - r1.w) {
                            r1.x += 5;
                        }
                        break;
                }
            }else if (evt.type == SDL_QUIT) {
                close_request = 1;

            }else if(evt.type == SDL_MOUSEMOTION){
                if(evt.button.x < WINDOW_WIDTH -r3.w){
                   r3.x = evt.button.x;
                }
                if( evt.button.y < WINDOW_HEIGHT - r3.h){
                   r3.y = evt.button.y;
                }
            }
        }else {
           if( r2.x >= WINDOW_WIDTH - r2.w){
               dx = -2;
           }else if(r2.x <= 0){
               dx = 2;
           }
           if( r2.y >= WINDOW_HEIGHT - r2.h){
               dy = -2;
           }else if(r2.y <= 0){
               dy = 2;
           }
            r2.x += dx;
            r2.y += dy;
        }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
