#include <SDL2/SDL.h>

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
void ganhador(int rect, int posicao){

    switch(rect)
    {
	case 1:
	    printf("O Rectangulo verde chegou na %dº posicao\n", posicao);
	    break;
	case 2:
	    printf("O Rectangulo vermelho chegou na %dº posicao\n", posicao);
	    break;
	case 3:
  	    printf("O Rectangulo azul chegou na %dº posicao\n", posicao);
	    break;

     }
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

    SDL_Rect r1 = { 100,50, 10,10 };
    SDL_Rect r2 = { 100, 100, 10, 10};
    SDL_Rect r3 = { 100, 150, 10, 10};
    SDL_Rect chegada = {450,0, 5, 500};
    SDL_Event evt;
    int close_request = 0;
    int espera = 100;
    int go1 = 1;
    int go2 = 1; //para o quadrado dois se mexer
    int go3 = 1;
    int posicao = 1;
    int chegou1 = 0;
    int chegou2 = 0;
    int chegou3 = 0;
    int dx =10; //deslocamento do x

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
        SDL_RenderPresent(ren);

        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (isevt) {
	    if (evt.type == SDL_KEYDOWN) {
  	        if(go1){  
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
                     go1 = 0;
		     ganhador(1, posicao);
		     r1.x = chegada.x - r1.w;
	 	     posicao += 1;
		     chegou1 = 1;
                }
	
            }else if (evt.type == SDL_QUIT) {
                close_request = 1;

            }else if(evt.type == SDL_MOUSEMOTION){
		if (SDL_HasIntersection(&chegada, &r3) && !chegou3) {
                    go3 = 0;
                    ganhador(3, posicao);
		    r3.x = chegada.x - r3.w;
	 	    posicao += 1;
		    chegou3 = 1;
                }
                if(evt.button.x < WINDOW_WIDTH -r3.w && go3){
                   r3.x = evt.button.x;
                }
            }
        }else {
            
	    if(go2){
	       r2.x += dx;
	       espera = 100;
	    }
            if (SDL_HasIntersection(&chegada, &r2) && !chegou2) {
                go2 = 0;
		r2.x = chegada.x-r2.w;
                ganhador(2, posicao);
	 	posicao += 1;
		chegou2 = 1;
            }
	    
        }
	
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
