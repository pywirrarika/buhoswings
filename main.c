/*
 *
 * Buho's Wings; 
 * Author:  Jesus Manuel Mager Hois
 *          in  2006-2007
 *          and heavy modified in 2015
 *
 * Licence: GPL v.3 or greater
 *  
 * Description: This is a simple 2d game.
 *
 * And now in spanish:
 *
 * Instructuvo para compilar:
 *
 * gcc main.c -o main 
 *
 * Para compilar en Visual C++:
 * Antes de compilar se debe indicar los directorios donde se
 * encuentran las librerias y archivos de cabecera de SDL.
 * En el CD de se localizan en src\SDL-1.2.11\INCLUDE Y 
 * src\SDL-1.2.11\LIB.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



// Librerias de SDL
// Comprobando la ruta de la cabecera
// segun compilador

#if defined(_MSC_VER)
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif



/*******************************************************************
 ******************  D E F I N I C I O N E S  **********************
 *******************************************************************/

#define ALTO 600
#define ANCHO 800

// Los flops
#define PF 100


// Otras cursilerias

#define MAXBALAS 6
#define MAXENEMIGOS 5
#define MAXENEMIGOS_1 3
#define MAXENEMIGOS_2 2
// El timer!

int ticUlt;

// Definiendo variables globales!
SDL_Rect naveR; 
SDL_Rect fon;
	


// Teclas del control
int TecIz;
int TecDe;
int TecAr;
int TecAb;

int Vidas;
int Puntos;

//Pantalla en memoria
SDL_Surface *screen;

//El fondo
SDL_Surface *Fondo;

//Sprites
SDL_Surface *osoImg;
SDL_Surface *balaImg;
SDL_Surface *balaEImg;
SDL_Surface *Perder;
SDL_Surface *Enemigo;
SDL_Surface *Yo;


// Definiendo estructuras de datos!!!!!!!!!!

	
typedef struct{
	int vivo;// Si esta activa
	SDL_Rect balasR;
	int tipo;
	int vx, vy;// Velocidad
}Bala;


typedef struct{
	int vivo;// Si esta activa
	SDL_Rect eneR;
	int tipo;
	int vx, vy;// Velocidad
}Ene;


Bala bala[MAXBALAS];	
Ene enemigo[MAXENEMIGOS];



// Definiendo funciones!

int inicia(void);
int pinta(void);
void perD(void);
int balaL(void);
void balaN(int x, int y, int vx, int vy);
int eneL(void);
void eneN(int x, int y, int vx, int tipo);
int cuadCol(SDL_Rect A, SDL_Rect B);
void tray();

// Cuerpo Principal del Programa
int main(int argc, char *argv[]){
	printf("Iniciando...\n");
	
	// Iniciando SDL
		
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER);
    printf("Cargado SDL con exito!\n");
	
	atexit(SDL_Quit);

	// Iniciando el Modo de video!
	screen = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE | SDL_DOUBLEBUF);

	if ( screen == NULL ) {
        fprintf(stderr, "No se pudo entrar en modo grafico 800x600 video mode: %s\n",
                        SDL_GetError());
		exit(-1);
    }
	
	

	/*TODO: Aqui ira el menu!
	
inicio:
	
	switch(menu()){
		case 1:
			// Nuevo Juego
			goto ini;
			break;
		case 2:
			// Cargar un juego cuardado!
			break;
		case 3: 
			// Ver la lista de Records
			//
			break;
		case 0;
			// Salir!
			return 0;
	
	*/
	
	Vidas = 3;
	
ini:
	inicia();  
	
	
SDL_ShowCursor(SDL_ENABLE);

	// Iniciamos el ciclo de eventos

	SDL_Event event;
    
	// Aqui vamos...

 while (1)
  {

	 //Nuevo oso malo: posteriormente águila (tambien malo)! 
	 eneN(670,rand()%600+40,3,1);

	 // Comprueba si se murio la nave, y cuantas vidas le quedan!
	 
	 if(pinta()!=0){
		 Vidas--;
		 if(Vidas==0){
			perD();
			return 0;
		 }
		 else
			 goto ini;
	 }
	 
		
	 
	 if(fon.x>=1300) fon.x=0;
	 fon.x++;
    
	 // Manejo de eventos
    while (SDL_PollEvent(&event)) 
    {
      switch (event.type) 
      {
      	case SDL_KEYDOWN:
                    
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          // Si se presiona ESC finaliza.
          perD();
		  return 0;
        
		   case SDLK_SPACE:
			   balaN(naveR.x+30,naveR.y+15,3,1);
				break;
		// Modifica la posicion del oso con el teclado
			case SDLK_LEFT:
				TecIz=1;
                break;
            case SDLK_RIGHT:
				TecDe=1;
                break;
            case SDLK_UP:
                TecAr=1;
                break;
            case SDLK_DOWN:
                TecAb=1;
                break;
			
            default:
                break;
            }	  
		  break;
	  case SDL_KEYUP:
                    
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          // Si se presiona ESC finaliza.
          
			perD();
			return 0;
        
		// Modifica la posicion del oso con el teclado
			case SDLK_LEFT:
				TecIz=0;
                break;
            case SDLK_RIGHT:
				TecDe=0;
                break;
            case SDLK_UP:
                TecAr=0;
                break;
            case SDLK_DOWN:
                TecAb=0;
                break;
				
            default:
                break;
            }
        break;
      case SDL_QUIT:
        return 0;
      }
    }
  }

		
	
	
	return 0;
}

/* Funcion encargada de cargar a memoria todas las imagenes, definir variables
y otro material, necesarias durante el juego */

int inicia(){
	
	//Vidas=3;
	int i;
	ticUlt = SDL_GetTicks(); 	
	naveR.x=ANCHO/2;
	naveR.y=ALTO/2;

	TecDe=0;
	TecIz=0;
	TecAr=0;
	TecAb=0;

    Puntos = 0;

	// Cargando imagenes

	balaImg = SDL_LoadBMP( "bala.bmp" );
	Yo      = SDL_LoadBMP( "Yo.bmp" );
	Fondo   = SDL_LoadBMP( "gb.bmp" );
	Perder  = SDL_LoadBMP( "perder.bmp" );
	osoImg  = SDL_LoadBMP( "oso.bmp" );
	Enemigo = SDL_LoadBMP( "ene.bmp" );
	

	// Calculado cuadrados
	naveR.w=osoImg->w;
	naveR.h=osoImg->h;

	fon.x=0;
	fon.y=0;
	fon.w=ANCHO;
	fon.h=ALTO;

	
	for(i=0;i<MAXBALAS;i++){
		bala[i].balasR.w=balaImg->w;
		bala[i].balasR.h=balaImg->h;
	}

	for(i=0;i<MAXENEMIGOS;i++){
		enemigo[i].eneR.w=Enemigo->w;
		enemigo[i].eneR.h=Enemigo->h;
	}

	// Definiendo colores transaparentes

	SDL_SetColorKey(osoImg, SDL_SRCCOLORKEY, 0x000000);
	SDL_SetColorKey(Enemigo, SDL_SRCCOLORKEY, 0x000000);
	SDL_SetColorKey(Perder, SDL_SRCCOLORKEY, 0xffffff);
	
	return 0;

};



/* Funcion que graficara todo el entorno del juego
en el ciclo principal */

int pinta(void){
	

	int i,i2;

	/*Activamos el timer*/

	int tic = SDL_GetTicks();

	if (tic <= ticUlt) 
	{
		SDL_Delay(1);
		return 0;
	}

    while (ticUlt < tic)
	{


	/*************************************************************
	 *Se definen todos los movimientos que se realizan por los
	 *objetos en la pantalla!
	 *************************************************************/
	

	/*Se aplican los eventos que ocurren en el teclado*/

	for(i=0;i<3;i++){
        if(TecDe) naveR.x += 1;
        if(TecIz) naveR.x -= 1;
        if(TecAr) naveR.y -= 1;
        if(TecAb) naveR.y += 1;
	}

	/*Se imita el movimiento de caida*/

	naveR.y+=1;
	naveR.x-=1;

	
	/*Se calcula el movimiento de las balas*/
	
	for(i=0;i<MAXBALAS;i++)
    {
		if(bala[i].vivo)
        {
			bala[i].balasR.x+=bala[i].vx;
        }
		if(bala[i].balasR.x>780)
        {
			bala[i].vivo=0;
        }
	}

	/*Se calcula el movimiento de los enemigos*/
		
	for(i=0;i<MAXENEMIGOS;i++)
    {
		if(enemigo[i].vivo)
        {
			enemigo[i].eneR.x-=enemigo[i].vx;
        }
		if(enemigo[i].eneR.x<4)
        {
			enemigo[i].vivo=0;
        }
	}

	/*Detectando altura minima y evitando que se salga el buho de la pantalla!*/
	
	if(naveR.x<=0)   naveR.x=1;
	if(naveR.x>=760) naveR.x=759;
	if(naveR.y<=0)   naveR.y= 1;

	if(naveR.y>540)
		return -1;		
	
	/*Detectando coliciones!*/

	for(i=0;i<MAXBALAS;i++)
    {
		for(i2=0;i2<MAXENEMIGOS;i2++)
        {
			if(cuadCol(enemigo[i2].eneR, bala[i].balasR)) enemigo[i2].vivo=0;
            {
                Puntos += 2;
                if(enemigo[i2].tipo==10)
                    Puntos +=10;
            }
		}
    }
	
	for(i=0;i<MAXENEMIGOS;i++)	
		if(cuadCol(naveR, enemigo[i].eneR)) return -1;
	
	
	ticUlt += 1000 / PF;
	}

  /* Cerrar la Surface si es necesario!*/
  if (SDL_MUSTLOCK(screen))
    if (SDL_LockSurface(screen) < 0) 
      return 0;



	/********************* Fin del Calculo ************************/
	
	
	/*Comienza el Scrolling...*/
	
	
	SDL_BlitSurface(Fondo,&fon,screen, NULL); 

	/*Se dibuja el Oso, posteriormente BUHO!!!*/

	SDL_BlitSurface(osoImg, NULL, screen, &naveR);
	
	/*Rendering de las balas*/
	for(i=0;i<MAXBALAS;i++)
		if(bala[i].vivo){
			SDL_BlitSurface(balaImg,NULL,screen,&bala[i].balasR);
		}
		for(i=0;i<MAXENEMIGOS;i++)
		if(enemigo[i].vivo){
			SDL_BlitSurface(Enemigo,NULL,screen,&enemigo[i].eneR);
		}

	/*Actualizamos la pantalla con los cambios hechos!*/
    SDL_BlitSurface(Yo,NULL,screen,0); 
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);	

	return 0;
};



/* Funcion para dibujar una imagen */


void perD()
{
	SDL_Rect imgP;
	imgP.x=250;
	imgP.y=250;
	
	printf("El puntaje final fue de: %d\n", Puntos); 
	SDL_BlitSurface(Perder, NULL, screen, &imgP);
    
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);	
	SDL_Delay(2000);	
};


// Funcion que regresa el numero de la bala libre!
int balaL(){
	int i;
	for(i=0;i<MAXBALAS;i++)
		if((bala[i].vivo)==0)
			return i;
	return -1;
};

void balaN(int x, int y, int vx, int vy){
	int a;
	a=balaL();
	if(a!=-1){
		bala[a].vivo=1;
		bala[a].balasR.x=x;
		bala[a].balasR.y=y;
		bala[a].vx=vx;
		bala[a].vy=vy;
	}
};

int eneL()
{
	int i;
	for(i=0;i<MAXENEMIGOS;i++)
		if((enemigo[i].vivo)==0)
			return i;
	return -1;
};

void eneN(int x, int y, int vx, int tipo)
{
	int a;
	a=eneL();
	if(a!=-1){
		enemigo[a].vivo=1;
		enemigo[a].eneR.x=x;
		enemigo[a].eneR.y=y;
		enemigo[a].vx=vx;
		enemigo[a].vy=1;
		enemigo[a].tipo=tipo;
	}
	
};


int cuadCol(SDL_Rect A, SDL_Rect B)
{	
	/*Comprobando bordes...*/
	
	if((A.y + A.h) <= B.y) return 0; 
	if(A.y >= (B.y + B.h)) return 0;  
	if((A.x + A.w) <= B.x) return 0;  
	if(A.x >= (B.x + B.w)) return 0;

	/*De lo contrario....*/
	
	return 1; 
} 
