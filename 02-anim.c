#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* img = IMG_LoadTexture(ren, "mario.png");
    assert(img != NULL);

    /* EXECU��O */
    SDL_Rect r = {250,0, 10,10};
    int z = 5;// varia��o y
    int w = 5;//varia��o do x
    int close_request = 0;
    int espera = 100;
    SDL_Event evt;

    while (!close_request) {
      SDL_SetRenderDrawColor(ren, 0, 0, 0, 0x00);
      SDL_RenderClear(ren);
      SDL_Rect c;
      if (w < 0) {
            c = (SDL_Rect) { 0,0, 500,438};
        } else {
            c = (SDL_Rect) { 500,0, 500,438};
        }
      SDL_RenderCopy(ren, img, &c, &r);
      SDL_RenderPresent(ren);


      int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
      if (isevt) {
        if (evt.type == SDL_QUIT) {
          close_request = 1;
        }else if (evt.type == SDL_KEYDOWN) {
          switch (evt.key.keysym.sym) {
            case  SDLK_RIGHT:
              if (r.x >= 250 && r.x <= (WINDOW_WIDTH - r.w) && r.y >= 0 && r.y <= (250 - r.h)) {
                w = 5;
                z = 5;
              }else if ( r.y >= 250 && r.y <= (WINDOW_HEIGHT - r.h) && r.x <= (WINDOW_WIDTH - r.w) && r.x >= 250) {
                w = 5;
                z = -5;
              }else if (r.x <= 250 && r.x > 0 && r.y >= 250 && r.y <= (WINDOW_HEIGHT - r.h)) {
                w = 5;
                z = 5;
              }else if ( r.y >= 0 && r.y <= (250 - r.h) && r.x <= 250 && r.x > 0) {
                w = 5;
                z = -5;
              }
              break;
            case SDLK_LEFT:
              if (r.x >= 250 && r.x <= (WINDOW_WIDTH - r.w) && r.y >= 0 && r.y <= (WINDOW_HEIGHT - r.h)) {
                z = -5;
                w = -5;
              }
              else if ( r.y >= 250 && r.y <= (WINDOW_HEIGHT - r.h) && r.x <= (WINDOW_WIDTH - r.w) && r.x >= 250) {
                z = 5;
                w = -5;
              }else if (r.x <= 250 && r.x > 0 && r.y >= 250 && r.y <= (WINDOW_HEIGHT - r.h)) {
                z = -5;
                w = -5;
              }else if ( r.y >= 0 && r.y <= (250 - r.h) && r.x <= 250 && r.x > 0) {
                w = -5;
                z = 5;
              }
              break;
          }
          r.y += z;
          r.x += w;
        }
      } else {
        if(r.x >= WINDOW_WIDTH - r.w){
          w = -5;
        }else if( r.y >= WINDOW_HEIGHT - r.y){
          z = -5;
        }else if(r.x<=0){
          w = 5;
        }else if(r.y <=0){
          z = 5;
        }
        espera = 100;
        r.y += z;
        r.x += w;
      }
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(img);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}