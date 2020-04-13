#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define SQR(a)      (a)*(a)
#define MAX(a,b)    (((a)>(b))?(a):(b))
#define M_PI 3.14159265
#define STRING_SIZE 100       // max size for some strings
#define TABLE_SIZE 550        // main game space size
#define LEFT_BAR_SIZE 150     // left white bar size
#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define SQUARE_SEPARATOR 8    // square separator in px
#define BOARD_SIZE_PER 0.7f   // board size in % wrt to table size
#define MAX_BOARD_POS 15      // maximum size of the board
#define MAX_COLORS 5
#define MARGIN 5
#define MAX_MOVES 225

// declaration of the functions related to graphical issues
void InitEverything(int , int , TTF_Font **, SDL_Surface **, SDL_Window ** , SDL_Renderer ** );
void InitSDL();
void InitFont();
SDL_Window* CreateWindow(int , int );
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);
int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);
int RenderTable(int, int, int [], TTF_Font *, SDL_Surface **, SDL_Renderer *);
void ProcessMouseEvent(int , int , int [], int , int *, int * );
void RenderPoints(int [][MAX_BOARD_POS], int, int, int [], int, SDL_Renderer *);
void RenderStats( SDL_Renderer *, TTF_Font *, int [], int , int );
void filledCircleRGBA(SDL_Renderer * , int , int , int , int , int , int );
void inicio(char[], int* , int* , int* , int [],int*, int*, int []);
void Gerartabuleiro (int, int, int, int [][MAX_BOARD_POS]);
void removercor(int [MAX_BOARD_POS][MAX_BOARD_POS], int [MAX_BOARD_POS][MAX_BOARD_POS], int , int, int, int, int, int [MAX_COLORS]);
void validcor(int [MAX_BOARD_POS][MAX_BOARD_POS],int [MAX_MOVES],int [MAX_MOVES], int*, int);
void detectquadrado(int [MAX_BOARD_POS][MAX_BOARD_POS], int, int, int, int[MAX_MOVES], int[MAX_MOVES], int*, int*);
void derrota( SDL_Renderer *, TTF_Font *);
void vitoria( SDL_Renderer *, TTF_Font *);
void escritaresultados(char [8], int, int);




// definition of some strings: they cannot be changed when the program is executed !
const char myName[] = "Diogo Ribeiro";
const char myNumber[] = "IST190051";
const int colors[3][MAX_COLORS] = {{242, 35, 14, 0, 255},{19, 41, 186, 20, 255},{19, 248, 32, 0, 255}};

/**
 * main function: entry point of the program
 * only to invoke other functions !
 */
int main( void )
{
    char jog[8];
    int cor=0;
    int pont[5]={0};
    int pontundo[5]={0};
    int pontinicialnovo[5]={0};
    int jogadas=0;
    int i=0, j=0;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *imgs[2];
    SDL_Event event;
    int delay = 300;
    int guardarx[MAX_MOVES]={0};
    int guardary[MAX_MOVES]={0};
    int quit = 0;
    int width = (TABLE_SIZE + LEFT_BAR_SIZE);
    int height = TABLE_SIZE;
    int square_size_px = 0, board_size_px[2] = {0};
    int board_pos_x = 0, board_pos_y = 0;
    int moves[MAX_BOARD_POS][MAX_BOARD_POS]={{0}};
    int board[MAX_BOARD_POS][MAX_BOARD_POS] = {{0}};
    int boardundo[MAX_BOARD_POS][MAX_BOARD_POS]={{0}};
    int pt_x = 0, pt_y = 0, making_move=0, primeiroy=0, primeirox=0, passagem=0, valido=0, cortrue=0, begin_color=0, passagem2=0, jogadasinicialnovo=0;
    int lose=0, jogadasundo=0, y=0, r=0, t=0, k=0, x=0, q=0, jogos=1;
    srand(1234);


    inicio (jog, &board_pos_y, &board_pos_x, &cor, pont, &jogadas, &jogadasinicialnovo, pontinicialnovo);

    Gerartabuleiro(board_pos_y, board_pos_x, cor, board);

    // initialize graphics
    InitEverything(width, height, &serif, imgs, &window, &renderer);


    while( quit == 0 )
    {
        // while there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                quit=1;
            }
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case SDLK_n:
                            //caso gere um tabuleiro novo sem ter acabado o anterior será contabilizado como um derrota
                             if((jogadas!=0 && pont[0]!=0) || (jogadas!=0 && pont[1]!=0) || (jogadas!=0 && pont[2] != 0) || (jogadas!=0 && pont[3]!=0) || (jogadas!=0 && pont[4]!=0))
                             {
                                lose=lose+1;
                             }
                             //para aumentar o número de jogos
                             jogos=jogos+1;
                             //criar um tabuleiro novo
                             Gerartabuleiro(board_pos_y, board_pos_x, cor, board);
                             //para manter os parametros
                             for(int m=0; m<cor; m++)
                             {
                                pont[m]=pontinicialnovo[m];
                                jogadas=jogadasinicialnovo;
                             }
                        break;
                    case SDLK_q:
                        //caso desista a meio de um jogo será contabilizado como derrota
                        if((jogadas!=0 && pont[0]!=0) || (jogadas!=0 && pont[1]!=0) || (jogadas!=0 && pont[2] != 0) || (jogadas!=0 && pont[3]!=0) || (jogadas!=0 && pont[4]!=0))
                             {
                                lose=lose+1;
                             }
                        escritaresultados(jog, jogos, lose);
                        quit=1;
                        break;

                    case SDLK_u:
                        for(k=0; k<board_pos_y; k++)
                        {
                            for(t=0; t<board_pos_x; t++)
                                board[k][t]=boardundo[k][t];
                        }
                        jogadas=jogadasundo;
                         for(x=0; x<cor; x++)
                         {
                              pont[x]=pontundo[x];
                         }
                        break;
                    default:
                        break;
                }
            }
            else if ( event.type == SDL_MOUSEBUTTONDOWN )
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                //Por o tabuleiro (moves) a zero e dar o valor 1 na célula onde começa.
                jogadasundo=jogadas;
                for(q=0; q<cor; q++)
                {
                    pontundo[q]=pont[q];
                }
                for(r=0; r<board_pos_y; r++)
                    {
                        for(y=0; y<board_pos_x; y++)
                            boardundo[r][y]=board[r][y];
                    }
                for(j=0; j<board_pos_y; j++)
                    for(i=0; i<board_pos_x; i++)
                    {
                        moves[j][i]=0;
                    }

                passagem=1;


                if(moves[pt_y][pt_x]==0)
                {
                    moves[pt_y][pt_x]=passagem;
                }
                primeirox=pt_x;                  //guardar a primeira posição
                primeiroy=pt_y;                  //guardar a primeira posição
                guardary[passagem]=pt_y;         //guardar a posição
                guardarx[passagem]=pt_x;         //guardar a posição
                begin_color=board[guardarx[1]][guardary[1]];
                valido=1;                        //iniciar o valido para comparar as cores
                making_move=1;





                }
            else if ( event.type == SDL_MOUSEBUTTONUP )
            {

                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);
                validcor(board, guardary, guardarx, &valido, passagem); // fuñção que compara as cores
                if(passagem>=2 && valido==passagem) // caso as cores durante o percurso forem iguais e selecionar pelo menos 2 bolas
                {
                    --jogadas;  // decrementar as jogadas

                    if (pt_y==guardary[1] && pt_x == guardarx[1] && passagem >= 4)// sabendo que a primeira posição é igual à ultima conseguimos saber que é um poligno fechado
                    {
                        detectquadrado(board, board_pos_y, board_pos_x, cor, guardary, guardarx, &cortrue, &passagem2);// função que elimana as bolas da mesma cor
                        pont[begin_color]=pont[begin_color]-passagem2;
                        if(pont[begin_color]<=0)
                            pont[begin_color]=0;
                    }
                    else if (jogadas<0)
                    {
                        quit=1;
                    }
                    else if((jogadas==0 && pont[0]!=0) || (jogadas==0 && pont[1]!=0) || (jogadas==0 && pont[2] != 0) || (jogadas==0 && pont[3]!=0) || (jogadas==0 && pont[4]!=0))
                    {
                        lose=lose+1;
                    }
                    else
                    {
                        removercor(board, moves, board_pos_y, board_pos_x, cor, passagem, begin_color, pont); //função para remover pontos, movimentar pontos e gerar novos pontos
                        pont[begin_color]=pont[begin_color]-passagem;

                        if(pont[begin_color]<=0)
                            pont[begin_color]=0;
                    }

                }

                guardarx[1]=0;
                guardary[1]=0;
                making_move=0;
                passagem=0;
                begin_color=0;
                passagem2=0;


            }
            else if ( event.type == SDL_MOUSEMOTION )
            {
                ProcessMouseEvent(event.button.x, event.button.y, board_size_px, square_size_px, &pt_x, &pt_y);

                if(making_move==1)
                    {
                    if(moves[pt_y][pt_x]==0)
                    {
                        if((pt_y==primeiroy+1 && pt_x==primeirox)||(pt_y==primeiroy-1 && pt_x==primeirox)||(pt_y==primeiroy && pt_x==primeirox+1)||(pt_y==primeiroy && pt_x==primeirox-1))
                        {

                              //conta o numero de bolas por onde passa
                              moves[pt_y][pt_x]=++passagem;
                              //guarda a posição
                              guardary[passagem]=pt_y;
                              guardarx[passagem]=pt_x;
                              // serve para detetar se está em cima, à direita, à esquerda ou em baixo
                              primeirox=pt_x;
                              primeiroy=pt_y;


                        }
                    }
                    else if (pt_y==guardary[passagem-1] && pt_x==guardarx[passagem-1])
                    {
                        moves[guardary[passagem]][guardarx[passagem]]=0;
                        --passagem;
                        guardary[passagem]=pt_y;
                        guardarx[passagem]=pt_x;
                        primeirox=pt_x;
                        primeiroy=pt_y;

                    }

               }

           }
        }
        // render game table
        square_size_px = RenderTable( board_pos_x, board_pos_y, board_size_px, serif, imgs, renderer);
        // render board
        RenderPoints(board, board_pos_x, board_pos_y, board_size_px, square_size_px, renderer);
        RenderStats(renderer, serif, pont, cor, jogadas);
        if((jogadas==0 && pont[0]!=0) || (jogadas==0 && pont[1]!=0) || (jogadas==0 && pont[2] != 0) || (jogadas==0 && pont[3]!=0) || (jogadas==0 && pont[4]!=0))
        {
            derrota (renderer, serif);
        }
        else if(pont[0]==0 && pont[1]==0 && pont[2]==0 && pont[3]==0 && pont[4]==0 && jogadas>=0)
        {
            vitoria(renderer, serif);
        }

        SDL_RenderPresent(renderer);
        // add a delay
        SDL_Delay( delay );



    }

    // free memory allocated for images and textures and closes everything including fonts
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
//função dos parametros

void inicio(char _jog[], int* board_pos_y, int* board_pos_x, int* _cor, int _pont[],int*_jogadas, int*_jogadasinicialnovo, int _pontinicialnovo[])
{
    int p=0, i=0;
    printf("Bem-vindos ao ISTDOTS\n\nSempre que desejar começar um jogo clique na tecla (irá ter os mesmo parametros)\nCaso inicie sem ter acabado o jogo anterior será contabilizado como derrota\n \nSempre que desejar sair clique na tecla q mas será contabilizado como derrota \n\n");
    //obter o nome do utilizador
    printf("Qual é o nome do utilizador:\n");
    fgets(_jog, STRING_SIZE, stdin);


    while (strlen(_jog)>9)//obriga a ter mesmo de 8 carateres
    {
        printf("Qual é o nome do utilizador:\n");
        fgets(_jog, STRING_SIZE, stdin);
    }

    printf("Quantas linhas deseja que o tabeleiro tenha (de 5 a 15):\n");
    scanf("%d",board_pos_y);
    while(*board_pos_y<5 || *board_pos_y>15)//obriga a ter de 5 a 15 linhas
    {
        printf("Quantas linhas deseja que o tabeleiro tenha (de 5 a 15):\n");
        scanf("%d",board_pos_y);
    }

    printf("Quantas colunas deseja que o tabeleiro tenha (de 5 a 15):\n");
    scanf("%d",board_pos_x);
    while(*board_pos_x<5 || *board_pos_x>15)//obriga ter de 5 a 15 colunas
    {
        printf("Quantas colunas deseja que o tabeleiro tenha (de 5 a 15):\n");
        scanf("%d",board_pos_x);
    }
    printf("Quantas cores desejas que o tabeleiro tenha (de 2 a 5):\n");
    scanf("%d",_cor);
    while(*_cor<2 || *_cor>5)//obriga ter entre 2 e 5 cores
    {
        printf("Quantas cores desejas que o tabeleiro tenha (de 2 a 5):\n");
        scanf("%d",_cor);
    }

    for (i=0; i< *_cor; i++)
    {
        printf("Qual a pontuação da cor %d:\n", i);
        scanf("%d", &p);
        _pont[i]=p;
        _pontinicialnovo[i]=_pont[i];
        while (p<1 || p>99)// obriga a ter a pontuação no máximo 99 e no minimo 1
        {
            printf("Qual a pontuação da cor %d:\n", i);
            scanf("%d", &p);
            _pont[i]=p;
            _pontinicialnovo[i]=_pont[i];

        }
    }

    printf("Quantas jogadas desejas que o tabeleiro tenha (até 99):\n");
    scanf("%d",_jogadas);
    *_jogadasinicialnovo=*_jogadas;
    while(*_jogadas<1 || *_jogadas>99)// obriga a ter no minimo 1 jogada e no maximo 99
    {
        printf("Quantas jogadas desejas que o tabeleiro tenha (até 99):\n");
        scanf("%d",_jogadas);
        *_jogadasinicialnovo=*_jogadas;

    }
}


//função que gera o tabuleiro inicial
void Gerartabuleiro(int board_pos_y, int board_pos_x, int cor, int board[][MAX_BOARD_POS])
{

    int i=0, a=0;
//    srand(1234);
    for (i=0; i<board_pos_y; i++)
    {
        for(a=0; a<board_pos_x; a++)
        {
            board[a][i]= ((int)rand()%(cor));
        }
    }

}

void validcor (int board[MAX_BOARD_POS][MAX_BOARD_POS], int guardarx[MAX_MOVES], int guardary[MAX_MOVES], int*_valido, int passagem)
{
    int i=0;

    for(i=2; i<=passagem; i++)
    {
        if(board [guardary[i]][guardarx[i]] == board[guardary[1]][guardarx[1]]) // se a cor for igual a da primeira
            *_valido=*_valido+1;


    }
}

/**
 * ProcessMouseEvent: gets the square pos based on the click positions !
 * \param _mouse_pos_x position of the click on pixel coordinates
 * \param _mouse_pos_y position of the click on pixel coordinates
 * \param _board_size_px size of the board !
 * \param _square_size_px size of each square
 * \param _pt_x square nr
 * \param _pt_y square nr
 */
void ProcessMouseEvent(int _mouse_pos_x, int _mouse_pos_y, int _board_size_px[], int _square_size_px,
        int *_pt_x, int *_pt_y )
{
    // corner of the board
    int x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    int y_corner = (TABLE_SIZE - _board_size_px[1] - 15);
    int sq_x=0, sq_y=0, dist=0;
    int circleX=0, circleY=0, circleR=0;

    // verify if valid cordinates
    if (_mouse_pos_x < x_corner || _mouse_pos_y < y_corner || _mouse_pos_x > (x_corner + _board_size_px[0])
        || _mouse_pos_y > (y_corner + _board_size_px[1]) )
    {
        *_pt_x = -1;
        *_pt_y = -1;
        return;
    }

    sq_x = (_mouse_pos_x - x_corner) / (_square_size_px + SQUARE_SEPARATOR);
    sq_y = (_mouse_pos_y- y_corner) / (_square_size_px + SQUARE_SEPARATOR);

    circleX = x_corner + (sq_x+1)*SQUARE_SEPARATOR + sq_x*(_square_size_px)+(_square_size_px>>1);
    circleY = y_corner + (sq_y+1)*SQUARE_SEPARATOR + sq_y*(_square_size_px)+(_square_size_px>>1);
    circleR = (int)(_square_size_px*0.4f);

    dist= (int)floor(sqrt (SQR (_mouse_pos_x-circleX)+ SQR(_mouse_pos_y-circleY)));

    if(dist<circleR)
    {
        *_pt_x=sq_x;
        *_pt_y=sq_y;
    }
    else
    {
        *_pt_x=-1;
        *_pt_y=-1;
    }

}

void removercor(int board[MAX_BOARD_POS][MAX_BOARD_POS], int moves[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos_y, int board_pos_x, int cor, int passagem,int begin_color, int pont[MAX_COLORS])
{
    int i=0, j=0, k=0;
//    srand(1234);

    for(i=0; i<board_pos_y; i++)
    {
        for(j=0; j<board_pos_x; j++)
        {

            if(moves[i][j]>=1)
            {
                for(k=i; k>=0; k--)
                {
                    if(k==0)// obriga a gerar uma cor
                        board[j][k]=((int)rand()%cor);

                    else
                        board[j][k]=board[j][k-1];// vai buscar a cor de cima


                }
            }
        }
    }

}

void detectquadrado(int board[MAX_BOARD_POS][MAX_BOARD_POS], int board_pos_y, int board_pos_x, int cor, int guardary[MAX_MOVES], int guardarx[MAX_MOVES], int*_cortrue, int *_passagem2)
{
    int i=0, a=0, k=0;
    *_cortrue = board [guardarx[1]][guardary[1]];
    for(i=0; i<board_pos_y; i++)
    {
        for(a=0; a<board_pos_x; a++)
        {
            if(board[a][i] == *_cortrue)
            {
                *_passagem2=*_passagem2+1; // saber a quantidade de bolas com a cor do quadrado
                for(k=i; k>=0; k--)
                {   if (k==0)
                    {
                        board[a][k]=((int)rand()%cor);
                        while(board[a][k]==*_cortrue) //para não haver cores iguais aos dos quadrados
                        {
                            board[a][k]=((int)rand()%cor);
                        }
                    }
                    else
                        board[a][k]=board[a][k-1];
                }
            }
        }
    }


}

//void dentrodoquadrado(int moves[MAX_BOARD_POS][MAX_BOARD_POS])
//{
//    for(a)
//}
/**
 * RenderPoints: renders the board
 * \param _board 2D array with integers representing board colors
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _square_size_px size of each square
 * \param _board_size_px size of the board
 * \param _renderer renderer to handle all rendering in a window
 */
 //função para imprimir o resultado da vitória
void vitoria( SDL_Renderer *_renderer, TTF_Font *_font)
{
    char str[STRING_SIZE]={0};


    SDL_Color light={225, 225, 23};
    SDL_Color dark={0, 0, 0};               //string com o texto

    SDL_Rect vitoriaa;                     //rectangulo com o texto
    vitoriaa.x=200;                        //local onde o texto aparece
    vitoriaa.y=325;                        //local onde o texto aparece

    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo


    sprintf(str, "             Venceste!!                ");                     //converte o inteiro moves para string
    TTF_SizeText(_font, str, &vitoriaa.w, &vitoriaa.h);                    //para saber o tamanho do texto
    SDL_RenderFillRect(_renderer, &vitoriaa);                               //desenhar o retâgulo
    RenderText(vitoriaa.x, vitoriaa.y, str, _font, &dark, _renderer);

    SDL_Rect vitoriaa2;                     //rectangulo com o texto
    vitoriaa2.x=200;                        //local onde o texto aparece
    vitoriaa2.y=350;                        //local onde o texto aparece

    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo


    sprintf(str, "Digite n para um jogo novo!!");                     //converte o inteiro moves para string
    TTF_SizeText(_font, str, &vitoriaa2.w, &vitoriaa2.h);                    //para saber o tamanho do texto
    SDL_RenderFillRect(_renderer, &vitoriaa2);                               //desenhar o retâgulo
    RenderText(vitoriaa2.x, vitoriaa2.y, str, _font, &dark, _renderer);
}
// função para imprimir o resultado da derrota
void derrota( SDL_Renderer *_renderer, TTF_Font *_font)
{
    char str[STRING_SIZE]={0};


    SDL_Color light={225, 225, 23};
    SDL_Color dark={0, 0, 0};               //string com o texto

    SDL_Rect derrotaa;                     //rectangulo com o texto
    derrotaa.x=200;                        //local onde o texto aparece
    derrotaa.y=325;                        //local onde o texto aparece

    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo


    sprintf(str, "             Perdeste!!                ");                     //converte o inteiro moves para string
    TTF_SizeText(_font, str, &derrotaa.w, &derrotaa.h);                    //para saber o tamanho do texto
    SDL_RenderFillRect(_renderer, &derrotaa);                               //desenhar o retâgulo
    RenderText(derrotaa.x, derrotaa.y, str, _font, &dark, _renderer);

    SDL_Rect derrotaa2;                     //rectangulo com o texto
    derrotaa2.x=200;                        //local onde o texto aparece
    derrotaa2.y=350;                        //local onde o texto aparece

    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo


    sprintf(str, "Digite n para um jogo novo!");                     //converte o inteiro moves para string
    TTF_SizeText(_font, str, &derrotaa2.w, &derrotaa2.h);                    //para saber o tamanho do texto
    SDL_RenderFillRect(_renderer, &derrotaa2);                               //desenhar o retâgulo
    RenderText(derrotaa2.x, derrotaa2.y, str, _font, &dark, _renderer);
}

void escritaresultados(char _jog[8], int _jogos, int _lose)
{

    int _winner=0;
    FILE *doc = fopen("resultados.txt", "w");
    _winner=_jogos-_lose;
    fprintf(doc,"Nome do jogador: %s\n Realizou %d jogos\n Teve %d vitoria!\n Teve %d derrotas!\n Obrigado por jogar!!", _jog, _jogos, _winner, _lose);
}

void RenderPoints(int _board[][MAX_BOARD_POS], int _board_pos_x, int _board_pos_y,
        int _board_size_px[], int _square_size_px, SDL_Renderer *_renderer )
{
    int clr, x_corner, y_corner, circleX, circleY, circleR;

    // corner of the board
    x_corner = (TABLE_SIZE - _board_size_px[0]) >> 1;
    y_corner = (TABLE_SIZE - _board_size_px[1] - 15);

    // renders the squares where the dots will appear
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
                // define the size and copy the image to display
                circleX = x_corner + (i+1)*SQUARE_SEPARATOR + i*(_square_size_px)+(_square_size_px>>1);
                circleY = y_corner + (j+1)*SQUARE_SEPARATOR + j*(_square_size_px)+(_square_size_px>>1);
                circleR = (int)(_square_size_px*0.4f);
                // draw a circle
                clr = _board[i][j];
                filledCircleRGBA(_renderer, circleX, circleY, circleR, colors[0][clr], colors[1][clr], colors[2][clr]);
        }
    }
}

/**
 * filledCircleRGBA: renders a filled circle
 * \param _circleX x pos
 * \param _circleY y pos
 * \param _circleR radius
 * \param _r red
 * \param _g gree
 * \param _b blue
 */

void filledCircleRGBA(SDL_Renderer * _renderer, int _circleX, int _circleY, int _circleR, int _r, int _g, int _b)
{
    int off_x = 0;
    int off_y = 0;
    float degree = 0.0;
    float step = M_PI / (_circleR*8);

    SDL_SetRenderDrawColor(_renderer, _r, _g, _b, 255);

    while (_circleR > 0)
    {
        for (degree = 0.0; degree < M_PI/2; degree+=step)
        {
            off_x = (int)(_circleR * cos(degree));
            off_y = (int)(_circleR * sin(degree));
            SDL_RenderDrawPoint(_renderer, _circleX+off_x, _circleY+off_y);
            SDL_RenderDrawPoint(_renderer, _circleX-off_y, _circleY+off_x);
            SDL_RenderDrawPoint(_renderer, _circleX-off_x, _circleY-off_y);
            SDL_RenderDrawPoint(_renderer, _circleX+off_y, _circleY-off_x);
        }
        _circleR--;
    }
}

/*
 * Shows some information about the game:
 * - Goals of the game
 * - Number of moves
 * \param _renderer renderer to handle all rendering in a window
 * \param _font font to display the scores
 * \param _goals goals of the game
 * \param _ncolors number of colors
 * \param _moves number of moves remaining
 */

void RenderStats( SDL_Renderer *_renderer, TTF_Font *_font, int _pont[], int _cor, int _jogadas)
{
    /* To Be Done */
    char str[STRING_SIZE]={0};
    int i=0;

    SDL_Color light={205, 193, 181};
    SDL_Color dark={125, 110, 102};          //string com o txeto

    SDL_Rect text_area;                     //rectangulo com o texto
    text_area.x=30;                         //local onde o texto aparece
    text_area.y=50;                         //local onde o texto aparece

    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo


    sprintf(str, "  jogadas: %d  ", _jogadas);                                //converte o inteiro moves para string
    TTF_SizeText(_font, str, &text_area.w, &text_area.h);                    //para saber o tamanho do texto
    SDL_RenderFillRect(_renderer, &text_area);                               //desenhar o retâgulo
    RenderText(text_area.x, text_area.y, str, _font, &dark, _renderer);

    for(i=0; i<_cor; i++)
    {
        SDL_Rect cores;                                                       //rectangulo com o texto
        cores.x=150+80*i;                                                    //local onde o texto aparece
        cores.y=50;
        sprintf(str, "         %d  ", _pont[i]);

        SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a);  //define a cor do retângulo




        //converte o inteiro moves para string
        TTF_SizeText(_font, str, &cores.w, &cores.h);
        //para saber o tamanho do texto
        SDL_RenderFillRect(_renderer, &cores);
         //desenhar o retâgulo
        RenderText(cores.x, cores.y, str, _font, &dark, _renderer);
        //criar as bolas com as cores
        filledCircleRGBA(_renderer, 163+80*i, 63, text_area.h/3, colors[0][i], colors[1][i], colors[2][i]);
    }


}

/*
 * RenderTable: Draws the table where the game will be played, namely:
 * -  some texture for the background
 * -  the right part with the IST logo and the student name and number
 * -  the grid for game board with squares and seperator lines
 * \param _board_pos_x number of positions in the board (x axis)
 * \param _board_pos_y number of positions in the board (y axis)
 * \param _board_size_px size of the board
 * \param _font font used to render the text
 * \param _img surfaces with the table background and IST logo (already loaded)
 * \param _renderer renderer to handle all rendering in a window
 */

int RenderTable( int _board_pos_x, int _board_pos_y, int _board_size_px[],
        TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer )
{
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color light = { 205, 193, 181 };
    SDL_Color dark = { 120, 110, 102 };
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, board, board_square;
    int height, board_size, square_size_px, max_pos;

    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;
    tableDest.w = TABLE_SIZE;
    tableDest.h = TABLE_SIZE;

    // draws the table texture
    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(TABLE_SIZE, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(TABLE_SIZE+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(TABLE_SIZE+3*MARGIN, height, myNumber, _font, &black, _renderer);

    // compute and adjust the size of the table and squares
    max_pos = MAX(_board_pos_x, _board_pos_y);
    board_size = (int)(BOARD_SIZE_PER*TABLE_SIZE);
    square_size_px = (board_size - (max_pos+1)*SQUARE_SEPARATOR) / max_pos;
    _board_size_px[0] = _board_pos_x*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;
    _board_size_px[1] = _board_pos_y*(square_size_px+SQUARE_SEPARATOR)+SQUARE_SEPARATOR;

    // renders the entire board background
    SDL_SetRenderDrawColor(_renderer, dark.r, dark.g, dark.b, dark.a );
    board.x = (TABLE_SIZE - _board_size_px[0]) >> 1;
    board.y = (TABLE_SIZE - _board_size_px[1] - 15);
    board.w = _board_size_px[0];
    board.h = _board_size_px[1];
    SDL_RenderFillRect(_renderer, &board);

    // renders the squares where the numbers will appear
    SDL_SetRenderDrawColor(_renderer, light.r, light.g, light.b, light.a );

    // iterate over all squares
    for ( int i = 0; i < _board_pos_x; i++ )
    {
        for ( int j = 0; j < _board_pos_y; j++ )
        {
            board_square.x = board.x + (i+1)*SQUARE_SEPARATOR + i*square_size_px;
            board_square.y = board.y + (j+1)*SQUARE_SEPARATOR + j*square_size_px;
            board_square.w = square_size_px;
            board_square.h = square_size_px;
            SDL_RenderFillRect(_renderer, &board_square);
        }
    }

    // destroy everything
    SDL_DestroyTexture(table_texture);
    // return for later use
    return square_size_px;
}

/**
 * RenderLogo function: Renders the IST logo on the app window
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */

int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}

/**
 * RenderText function: Renders some text on a position inside the app window
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string with the text to be written
 * \param _font TTF font used to render the text
 * \param _color color of the text
 * \param _renderer renderer to handle all rendering in a window
 */

int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;
    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture
    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory
    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _font font that will be used to render the text
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */

void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */

void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */

void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */

SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "ISTDOTS", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */

SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );

    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}

