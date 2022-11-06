#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect q = {200, 0, 100, 100};
    SDL_Rect r = {250,0, 10,10};
    int z = 2;// variação y
    int w = 2;//variação do x
    int acabou = 0;
    while (acabou <= 3) {
      SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
      SDL_RenderClear(ren);
      SDL_SetRenderDrawColor(ren, 0, 0, 0, 0x00);
      SDL_RenderFillRect(ren, &q);
      SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
      SDL_RenderFillRect(ren, &r);
      SDL_RenderPresent(ren);
      SDL_Delay(100);
      if(r.y >= 40 && r.x >= 290){
        w = -2;
      }else if( r.y >= 90 && r.x <= 250){
         z = -2;
      }else if(r.x<=200 && r.y <= 50){
        w = 2;
      }else if(r.x >=240 && r.y <=0){
        z = 2;
        acabou += 1;
      }
      r.y += z;
      r.x += w;
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
