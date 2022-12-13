#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_HEIGHT 200
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

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {0xFF,0x00,0x00,0xFF};

    surface = TTF_RenderText_Blended(font, text, textColor);
    assert(surface != NULL);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    assert(texture != NULL);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}
int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_Window* win = SDL_CreateWindow("Contagem do tempo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    TTF_Font* fnt = TTF_OpenFont("tiny.ttf", 20);
    assert(fnt != NULL);
    SDL_Texture* txt;
    assert(txt != NULL);
   
    /* EXECUÇÃO */

    SDL_Rect r1 = { 100,50, 10,10 };
    SDL_Rect r2 = { 100, 100, 10, 10};
    SDL_Rect r3 = { 100, 150, 10, 10};
    SDL_Rect texto = { 50,50, 200,80 };
    SDL_Rect chegada = {450,0, 5, 500};
    SDL_Event evt;
    int close_request = 0;
    int espera = 100;
    int posicao = 1;
    int chegou1 = 0;
    int chegou2 = 0;
    int chegou3 = 0;
    int ganhou = 0;
    int dx =10; //deslocamento do x
    int reiniciar  = 0; // indica que é possivel reiniciar o corrida
    get_text_and_rect(ren, 0, 0, "START!!!", fnt, &txt, &texto);

    while (!close_request) {
        SDL_SetRenderDrawColor(ren, 0,0,0,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 0,255,0,0x00);
        SDL_RenderFillRect(ren, &r1);
        SDL_SetRenderDrawColor(ren, 255,0,0,0x00);
        SDL_RenderFillRect(ren, &r2);
        SDL_SetRenderDrawColor(ren, 0,0,255,0x00);
        SDL_RenderFillRect(ren, &r3);
	    SDL_SetRenderDrawColor(ren, 255,255,255,0x00);
	    SDL_RenderFillRect(ren, &chegada);
        SDL_RenderCopy(ren, txt, NULL, &texto);
        SDL_RenderPresent(ren);

        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
            if (evt.type == SDL_KEYDOWN) {
                if(!chegou1){  
                    switch (evt.key.keysym.sym) {
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
		        }
                if (SDL_HasIntersection(&chegada, &r1) && !chegou1) {
                    r1.x = chegada.x - r1.w;
                    posicao += 1;
                    chegou1 = 1;
                    if(!chegou2 && !chegou3){
                        ganhou = 1;
                    }
                }
                switch (evt.key.keysym.sym)
                {
                case SDL_SPACE:
                    if (reiniciar){
                        chegou1 = 0;
                        chegou2 = 0;
                        chegou3 = 0;
                        r1.x = 100;
                        r2.x = 100;
                        r3.x = 100;
                        posicao = 1;
                        reiniciar = 0;
                        get_text_and_rect(ren, 0, 0, "START!!!", fnt, &txt, &texto);

                    }
                    break;
                }
	
            }else if (evt.type == SDL_QUIT) {
                close_request = 1;

            }else if(evt.type == SDL_MOUSEMOTION){
                if (SDL_HasIntersection(&chegada, &r3) && !chegou3) {
                    r3.x = chegada.x - r3.w;
                    posicao += 1;
                    chegou3 = 1;
                    if(!chegou2 && !chegou1){
                        ganhou = 3;
                    }
                }
                if(evt.button.x < chegada.x && !chegou3){
                    r3.x = evt.button.x;
                }
            }
        }else {
            
            if(!chegou2){
                r2.x += dx;
                espera = 100;
            }
            if (SDL_HasIntersection(&chegada, &r2) && !chegou2) {
                    r2.x = chegada.x-r2.w;
                    posicao += 1;
                    chegou2 = 1;
                    if(!chegou1 && !chegou3){
                        ganhou = 2;
                    }
            }
	    
        }
        if(chegou1 && chegou2 && chegou3){
            switch(ganhou)
            {
            case 1:
                get_text_and_rect(ren, 0, 0, "O verde chegou primeiro!!", fnt, &txt, &texto);
                break;
            case 2:
                get_text_and_rect(ren, 0, 0, "O vermelho chegou primeiro!!", fnt, &txt, &texto);
                break;
            case 3:
                get_text_and_rect(ren, 0, 0, "O azul chegou primeiro!!", fnt, &txt, &texto);
                break;
            default:
                get_text_and_rect(ren, 0, 0, "Alguem ganhou", fnt, &txt, &texto);
            }
            //SDL_RenderCopy(ren, txt, NULL, &texto);
            reiniciar = 1;
            
        }
	
    }

    /* FINALIZACAO */
    /* Deinit TTF. */
    SDL_DestroyTexture(txt);
    TTF_CloseFont(fnt);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}