#include <SDL2/SDL.h>


#define WINDOW_HEIGHT 500
#define WINDOW_WIDTH 500

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
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect q = {200, 0, 100, 100};
    SDL_Rect r = {250,0, 10,10};
    int z = 2;// variação y
    int w = 2;//variação do x
    int close_request = 0;
    int espera = 100;
    SDL_Event evt;

    while (!close_request) {
      SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
      SDL_RenderClear(ren);
      SDL_SetRenderDrawColor(ren, 0, 0, 0, 0x00);
      SDL_RenderFillRect(ren, &q);
      SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
      SDL_RenderFillRect(ren, &r);
      SDL_RenderPresent(ren);


      int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
      if (isevt) {
        if (evt.type == SDL_QUIT) {
          close_request = 1;
        }else if (evt.type == SDL_KEYDOWN) {
          switch (evt.key.keysym.sym) {
            case  SDLK_RIGHT:
              if (r.x > 250 && r.x <= 290 && r.y > 0 && r.y <= 40) {
                w = 2;
                z = 2;
              }else if ( r.y > 40 && r.y <= 90 && r.x <= 290 && r.x > 250) {
                w = 2;
                z = -2;
              }else if (r.x <= 240 && r.x > 200 && r.y <= 90 && r.y > 50) {
                w = 2;
                z = 2;
              }else if (r.y <= 40 && r.y >0 && r.x <=240 && r.x > 200) {
                w = 2;
                z = -2;
              }
              break;
            case SDLK_LEFT:
              if (r.x > 250 && r.x <= 290 && r.y > 0 && r.y <= 40) {
                z = -2;
                w = -2;
              }
              else if ( r.y > 40 && r.y <= 90 && r.x <= 290 && r.x > 250) {
                z = 2;
                w = -2;
              }else if (r.x <= 240 && r.x > 200 && r.y <= 90 && r.y > 50) {
                z = -2;
                w = -2;
              }else if (r.y <= 40 && r.y >0 && r.x <=240 && r.x > 200) {
                w = -2;
                z = 2;
              }
              break;
          }
          r.y += z;
          r.x += w;
        }
      } else {
        if(r.y >= 40 && r.x >= 290){
          w = -2;
        }else if( r.y >= 90 && r.x <= 250){
          z = -2;
        }else if(r.x<=200 && r.y <= 50){
          w = 2;
        }else if(r.x >=240 && r.y <=0){
          z = 2;
        }
        r.y += z;
        r.x += w;
      }
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
