#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define WINDOW_HEIGHT 250
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
    SDL_Color textColor = {255,255,255,0xFF};

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

int bateu_parede (SDL_Rect* b){
    return  (b->x >= WINDOW_WIDTH - b->w||  
            b->x <= 0||
            b->y >= WINDOW_HEIGHT - b->h||
            b->y <= 0);

}

void posicao_raquete(SDL_Rect* p){
    srand(time(0));
    int r = (rand() % (WINDOW_HEIGHT - p->h - 10)) + 10;
    p->y = r;
}

//funcao para mudar as coordenadas dos obstaculos
void posicao_obstaculos(SDL_Rect r[], int quantidade){
    int i;
    int metadex[]={250, 225, 275};
    int coordenaday[]={75, 100, 150, 200};
    int esquerdax[]={100, 150,175, 200};
    int direitax[]= {300,325, 350, 400};

    //Posicao do obstaculo da metade
    int x =  rand() % 3;
    int y = rand() % 4;
    r[i].x = metadex[x];
    r[i].y = coordenaday[y];
    r[i].h = 30;
    r[i].w = 30;
    i++;

    //Posicao do obstaculo da esquerda
    x =  rand() % 3;
    y = rand() % 4;
    r[i].x = esquerdax[x];
    r[i].y = coordenaday[y];
    r[i].h = 30;
    r[i].w = 30;
    i++;

    //Posicao do obstaculo da direita
    x =  rand() % 3;
    y = rand() % 4;
    r[i].x = direitax[x];
    r[i].y = coordenaday[y];
    r[i].h = 30;
    r[i].w = 30;

    
    
}

//Escolhe o corte da imagem 
void escolher_corte(SDL_Rect* c){
    int coordenadax[] = {0, 125, 250, 375};
    int coordenaday[] = {0, 115, 230, 345};
    int randomx = rand() % 4;
    int randomy = rand() % 4;
    c->x = coordenadax[randomx];
    c->y = coordenaday[randomy];
    c->h = 115;
    c->w = 125;
}

//funcao que detecta uma colisao com algum dos obstaculos 
int colisao(SDL_Rect* b, SDL_Rect r[]){
    int i;
    SDL_Rect aux;
    for (i = 0; i < 3; i++)
    {
        aux.x = r[i].x;
        aux.y = r[i].y;
        aux.h = r[i].w;
        aux.w = r[i].h;
        if(SDL_HasIntersection(b, &aux)){
            return 1;
        }
    }
    return 0;
    
}

typedef enum{
    PULANDO,
    MORTO, 
    QUICANDO, 
    INICIO
} Estados;

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("JOGO",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    TTF_Font* fnt = TTF_OpenFont("tiny.ttf", 20);
    assert(fnt != NULL);
    SDL_Texture* txt;
    assert(txt != NULL);
    SDL_Texture* txt1;
    assert(txt != NULL);

    SDL_Texture* img = IMG_LoadTexture(ren,"asteroides.png");
    assert(img!=NULL);
   
    /* EXECUÇÃO */

    SDL_Rect raquete1 = { 0,120, 10,70 };
    SDL_Rect raquete2 = { 490, 120, 10, 70};
    SDL_Rect bola = {240, 150, 10, 10};
    SDL_Rect obstaculos[3];
    SDL_Rect corte[3];

    SDL_Rect placar;
    SDL_Rect texto_final;
    SDL_Event evt;

    int espera = 500;
    int tqueda = 500;
    int tdie = 60000;

    //Valores para o temporizador do pulo
    Uint32 startTime = 0;
    Uint32 deltaTime;

    int melhor_resultado = 0;
    int close_request = 0;
    int comecou;
    int evento_enquanto_pula;
    int contador;
    char aux[100];
    int acabou;
    int velocidadex =2;
    int velocidadey = -2;
    int gravidade = 2;

    int i;
    int evento_enquanto_cai;

    Estados estado = INICIO;

    while (!close_request) {
        

        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);
        if (evt.type == SDL_QUIT) {
                close_request = 1;

        }
        switch (estado)
        {
        case INICIO:
            contador = 0; 
            comecou = 0;
            get_text_and_rect(ren, 150, 50, " ", fnt, &txt1, &texto_final);
            get_text_and_rect(ren, 240, 80, "0", fnt, &txt, &placar);
            if(isevt){
                switch (evt.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    estado = PULANDO;
                    comecou = 1;
                    posicao_obstaculos(obstaculos,3);
                    for (i = 0; i < 3; i++) {
                        escolher_corte(&corte[i]);
                    }
                    break;
                
                case SDL_KEYDOWN:
                    if(evt.key.keysym.sym==SDLK_SPACE){
                        estado = PULANDO;
                        comecou = 1;
                        posicao_obstaculos(obstaculos,3);
                        for (i = 0; i < 3; i++) {
                            escolher_corte(&corte[i]);
                        }
                    }
                    break;
                }
            }
            break;
        case PULANDO:
            sprintf(aux, "%d", contador);

            startTime = SDL_GetTicks();
            deltaTime = SDL_GetTicks() - startTime;
            if (deltatime > 20)
            {
                deltatime = 20;
            }
            
            get_text_and_rect(ren, 150, 50, " ", fnt, &txt1, &texto_final);
            get_text_and_rect(ren, 240, 80, aux , fnt, &txt, &placar);
            evento_enquanto_pula = AUX_WaitEventTimeoutCount(&evt, &espera);
            //fazer essa merda pular
            //bola.x += velocidadex;
            
            bola.y += velocidadey*deltatime;
            bola.x += velocidadex*deltatime;

            velocidadey = velocidadey + gravidade*deltatime;
            velocidadex += velocidadex;


            printf("Estou pulando agora\n");


            if (SDL_HasIntersection(&raquete1, &bola) || SDL_HasIntersection(&raquete2, &bola)){
                //Batendo nas raquetes
                estado = QUICANDO;
                    
            }else if(bateu_parede(&bola)|| colisao(&bola, obstaculos)){
                //Batendo nas paredes
                estado = MORTO;

            }else{
                if(evento_enquanto_pula){
                    switch (evt.type)
                    {
                        case SDL_MOUSEBUTTONDOWN:
                            
                            break;
                    
                        case SDL_KEYDOWN:
                            if(evt.key.keysym.sym==SDLK_SPACE){
                                
                            }
                            break;
                    }
                }else{
                    espera = 500;
                }
            }
           
            break;
        case QUICANDO:
            contador += 1;// Aumentar 1 no contador de vezes que a bola quicou nas raquetes
            printf("Estou quicando agora\n");
            velocidadex = -velocidadex;
            if(bola.x < 250){
                bola.x = raquete1.w;
            }else{
                bola.x = raquete2.x - bola.w;
            }
            sprintf(aux, "%d", contador);

            get_text_and_rect(ren, 150, 50, " ", fnt, &txt1, &texto_final);
            get_text_and_rect(ren, 240, 80, aux , fnt, &txt, &placar);
            //Inverter o sentido 


            //mexer raquete
            bola.x < 250 ? posicao_raquete(&raquete1):posicao_raquete(&raquete2);

            //atualizar obstaculos 
            posicao_obstaculos(obstaculos,3);
            for (i = 0; i < 3; i++) {
                escolher_corte(&corte[i]);
            }
            estado = PULANDO;
            break;
        case MORTO:
            printf("Eu estou morto\n");
            comecou = 0;
            bola.x = 240;
            bola.y = 150;
            sprintf(aux, "%d", contador);
            get_text_and_rect(ren, 150, 50, "SEU RESULTADO FOI:", fnt, &txt1, &texto_final);
            get_text_and_rect(ren, 240, 80, aux, fnt, &txt, &placar);


            //Calculo do melhor placar ate agora
            if(contador > melhor_resultado){
                melhor_resultado = contador;
            }

            acabou = AUX_WaitEventTimeoutCount(&evt, &tdie);
            if(acabou){
                // Se apertar space ou clicar o jogo comeca de novo
                switch (evt.type)
                {
                case SDL_MOUSEBUTTONDOWN:
                    estado = INICIO;
                    break;
                
                case SDL_KEYDOWN:
                    if(evt.key.keysym.sym==SDLK_SPACE){
                        estado = INICIO;
                    }
                    break;
                }
            }else{
                close_request = 1;
            }
            break;
        }
        SDL_SetRenderDrawColor(ren, 0,0,0,0x00);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren,255,255, 255, 0x00);
        SDL_RenderFillRect(ren, &raquete1);
        SDL_SetRenderDrawColor(ren, 255,255,255,0x00);
        SDL_RenderFillRect(ren, &raquete2);
        SDL_SetRenderDrawColor(ren, 255,255,255,0x00);
        SDL_RenderFillRect(ren, &bola);
        //obstaculos
        if(comecou){
            for (i = 0; i < 3; i++) {
                SDL_RenderCopy(ren, img, &corte[i], &obstaculos[i]);

            }
        }
        SDL_RenderCopy(ren, txt1, NULL, &texto_final);
        SDL_RenderCopy(ren, txt, NULL, &placar);
        SDL_RenderPresent(ren);
    }

    /* FINALIZACAO */
    /* Deinit TTF. */

    SDL_DestroyTexture(txt);
    SDL_DestroyTexture(txt1);
    SDL_DestroyTexture(img);
    TTF_CloseFont(fnt);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}