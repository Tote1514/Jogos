#include <SDL2/SDL.h>
#include <stdio.h>

#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500

int AUX_WaitEventTimeoutCount(SDL_Event* evt, int* ms){
    Uint32 antes = SDL_GetTicks();
    int evento = SDL_WaitEventTimeout(evt, *ms);
    if (evento) {
        *ms -= (SDL_GetTicks() - antes);
        if (*ms <  0) {
            *ms = 0;
        }
    }
    return evento;

}
int main (int argc, char* args[])
{
    /*Inicializacao*/
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Drag, Click or Cancel",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    /*EXECUCAO*/
    SDL_Rect r = {100, 50, 50, 50};
    SDL_Event evt;
    int close_request = 0;
    int leftMouseButtonDown = 0;
    SDL_Point mousePos;
    int originalx;//coordenada x original do retangulo
    int originaly;//coordenada y original do retangulo
    int digDrag = 0;//Indica se o rectangulo foi arrastado ou nao
    while(!close_request){
        SDL_SetRenderDrawColor(ren, 0,0,0,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0,255,0,0x00);
        SDL_RenderFillRect(ren, &r);
        SDL_RenderPresent(ren);

        switch(evt.type){
            case SDL_QUIT:
                close_request = 1;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT && SDL_PointInRect(&mousePos, r))
                    {
                        leftMouseButtonDown = true;
                        originalx = r.x;
                        originaly = r.y;
                        clickOffset.x = mousePos.x - r.x;
                        clickOffset.y = mousePos.y - r.y;
                        
                    }
                break;
            case SDL_MOUSEBUTTONUP:
                leftMouseButtonDown = 0;
                if (didDrag) {
                    printf("Dropped!");
                    didDrag = 0;
                } else {
                    printf("Clicked!");
                }
                break;
            case SDL_MOUSEMOTION:
                mousePos = { event.motion.x, event.motion.y };
                if (leftMouseButtonDown)
                {
                    r.x = mousePos.x - clickOffset.x;
                    r.y = mousePos.y - clickOffset.y;
                    didDrag = true;
                }
                break;
            case SDL_KEYDOWN:
                if(evt.key.keysym.sym == SDLK_ESCAPE){
                    if(digDrag){
                        r.x = originalx;
                        r.y = originaly;
                        printf("Canceled...");
                    }
                }
                break;
        }
    }
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}