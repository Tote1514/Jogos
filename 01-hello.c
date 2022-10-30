#include <SDL2/SDL.h>

#define POINTS_COUNT 4

static SDL_Point triangulo[POINTS_COUNT] = {
    {250, 225},
    {225, 275},
    {275, 275},
    {250, 225}
};
int main (int argc, char* argv[])
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


    //Desenhar Linhas dividindo a tela
    SDL_SetRenderDrawColor(ren, 0,255,0,0x00);
    SDL_RenderDrawLine(ren, 0, 0, 500, 500);
    SDL_RenderDrawLine(ren, 500, 0, 0, 500);

    //Desenhar primeiro retângulo
    SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
    SDL_Rect r1 = {200,200,100,100};
    SDL_RenderFillRect(ren, &r1);


    //Desenhar o segundo retângulo
    SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00, 0x00);
    SDL_Rect r2 = { 225, 225, 50, 50};
    SDL_RenderFillRect(ren, &r2);

    //Desenhar o triangulo
    SDL_SetRenderDrawColor(ren,255,69,0,0x00);
    SDL_RenderDrawLines(ren, triangulo, POINTS_COUNT);
    //Desenhar último retângulo
    SDL_SetRenderDrawColor(ren,255, 255, 255, 0x00);
    SDL_Rect ponto = {248, 251, 5, 5};
    SDL_RenderFillRect(ren, &ponto);



    SDL_RenderPresent(ren);
    SDL_Delay(3000);



    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
