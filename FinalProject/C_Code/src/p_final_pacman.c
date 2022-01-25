
/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "mb_interface.h"
#include <unistd.h>
#include "stdbool.h"

//TODO:
//Preguntar como generar aleatorios leyendo un clock del sistema
//Como leer de los botones para que se mueva el pacman
//Hacer los puntos, el fondo del camino gris


//DIRECCIONES DEFINIDAS EN EL HARDWARE GENERADO POR EL EDK
//DIRECCIONES DEFINIDAS EN EL HARDWARE GENERADO POR EL EDK
#define VGA_DIR 0xC6A00000
#define BOTONES 0x81400000
#define LEDSBASE 0x81420000
#define RDISPLAYBASE0 0xCEE00000
#define RDISPLAYBASE1 0xCEE00004
#define RDISPLAYBASE2 0xCEE00008
#define RDISPLAYBASE3 0xCEE0000C
#define XPAR_INT_CTRL_BASEADDR 0x81800000
#define XPAR_TIMER_BASEADDR 0x83C00000
#define XPAR_TIMER_INTERRUPT_MASK 0X1

#define TIMER_INT	XPAR_TIMER_INTERRUPT_MASK	//Si el timer tiene una interrupcion pendiente
#define INTC_IPR (*((volatile unsigned long *)(XPAR_INT_CTRL_BASEADDR + 0x04)))
#define INTC_IER (*((volatile unsigned long *)(XPAR_INT_CTRL_BASEADDR + 0x08)))
#define INTC_IAR (*((volatile unsigned long *)(XPAR_INT_CTRL_BASEADDR + 0x0C)))
#define INTC_MER (*((volatile unsigned long *)(XPAR_INT_CTRL_BASEADDR + 0x1C)))

#define TCSR0 (*((volatile unsigned long *)(XPAR_TIMER_BASEADDR + 0x00)))
#define TLR0 (*((volatile unsigned long *)(XPAR_TIMER_BASEADDR + 0x04)))

//Definiciones Mapa
#define DIMENSIONX 80   // se diide en 26 28 26
#define DIMENSIONY 60   // se divide en 14 31 15

#define XBASE 26
#define XFIN  XBASE+27
#define YBASE 14
#define YFIN  YBASE+30
#define MOVPODERES 30

#define PARED 0x07	//Azul bonito
#define BALDOSA 0x7F
#define VACIO 0x00 //Negro
#define PACMAN 0xFE
#define FANTASMA 0x62
#define FRUTA 0xC




struct obstaculos
{
	char x;
	char y;
	char ladox;
	char ladoy;
};

typedef enum Direccion {arriba,abajo,izquierda,derecha} Direccion;

typedef struct Pacman
{
	int x;
	int y;
	bool vivo;
	Direccion dir;
	bool poderes;
	int movPoderes;
	int puntos;
	//TODO contador de movimientos con poderes

}Pacman;

typedef struct Fantasma
{
	int x;
	int y;
	bool vivo;
	bool OnFruta;
	bool OnBaldosa;
	Direccion previa;
	//Direccion dir;

}Fantasma;

//Variables Mapa
char map[XFIN-XBASE+1][YFIN-YBASE+1];
struct obstaculos barriers[] = {
						  {2,2,4,3},{7,2,5,3},{13,1,2,4},{16,2,5,3}, {22,2,4,3},
						  {2,6,4,2},{7,6,2,8},{10,6,8,2}, {19,6,2,8},{22,6,4,2},
											  {13,8,2,3},
						{1,9,5,11}, {9,9,3,2}, 			 {16,9,3,2}, {22,9,5,11},

											  {10,12,8,5},

									{7,15,2,5},			  {19,15,2,5},
											  {10,18,8,2},
											  {13,20,2,3},
						  {2,21,4,2},{7,21,5,2},		  {16,21,5,2},{22,21,4,2},
						   {4,23,2,3},								 {22,23,2,3},
						{1,24,2,2},{7,24,2,3},{10,24,8,2},{19,24,2,3}, {25,24,2,2},
											  {13,26,2,3},
						  {2,27,10,2},					 {16,27,10,2}
};


struct obstaculos game_over[] = {
					//L
					{1,1,2,13}, {3,12,6,2},
					//0
					{11,1,2,13}, {13,1,7,2}, {18,3,2,11}, {13,12,5,2},
					//S
					{22,1,9,2}, {22,3,2,5}, {24,6,7,2}, {29,8,2,6}, {22,12,7,2},
					//E
					{33,1,9,2}, {33,3,2,11}, {35,7,5,2}, {35,12,7,2}

};

struct obstaculos win[] = {
					//W
					{1,1,2,5}, {2,5,2,6}, {4,8,2,5}, {5,7,1,3}, {6,8,1,5}, {7,5,2,6}, {8,1,2,5},
					//I
					{15,1,3,12},
					//N
					{23,1,2,12}, {25,1,1,4}, {26,2,1,7}, {27,5,1,7}, {28,6,1,7}, {29,10,1,3}, {30,1,2,12}
};

Pacman pacman;
Fantasma fantasma[4];
int fantasmasVivos = 4;

volatile int*vga_dir=(int*)VGA_DIR;
volatile unsigned int*botones=(unsigned int*)BOTONES;
volatile int*leds=(int*)LEDSBASE;
volatile unsigned int*displayr0=(unsigned int*)RDISPLAYBASE0;
volatile unsigned int*displayr1=(unsigned int*)RDISPLAYBASE1;
volatile unsigned int*displayr2=(unsigned int*)RDISPLAYBASE2;
volatile unsigned int*displayr3=(unsigned int*)RDISPLAYBASE3;
bool flag;
int cont;

void myISR(void)__attribute__((interrupt_handler));
void init_timer(); //Inicialización del timer
void pintaOculto(int x, int y, char color);
void pinta(int x, int y, char color);
void creaObstaculos();
void moverPacman(Direccion dir);
void moverFantasma(int id);
void inicializaPersonajes();
void mostrarleds();
void displayPuntos();
//void limpia();

void dibujaRectangulo (int numelem, int desplazamientox, int desplazamientoy, char color, struct obstaculos *array, bool oculto);

/**
 * Si cualquier funcion de mover retorna 1 (pacman muere)   -> JUGADOR PIERDE
 * 								 retorna 2 (fantasma muere) -> JUGADOR GANA PUNTO
 * */

int main()
{
	int valor_btn;

    //limpia();
	creaObstaculos();
	inicializaPersonajes();

	//srand((int)NULL);
	init_timer(); 							//Inicializamos el timer
	microblaze_enable_interrupts();			//Activamos interrupciones en el procesado



	for(;;) {
		if(flag){
			valor_btn =(*botones)>>4;
			//xil_printf("%X\n",(*botones)>>28);
			if(pacman.vivo) {
				if((valor_btn)==2){ //B1
					moverPacman(abajo);
				}else if((valor_btn)==1){
					moverPacman(arriba);
				}else if((valor_btn)==8){
					moverPacman(derecha);
				}else if((valor_btn)==4){
					moverPacman(izquierda);
				}
			}

			moverFantasma(0);
			moverFantasma(1);
			moverFantasma(2);
			moverFantasma(3);

			displayPuntos();
			mostrarleds();

			flag=FALSE;
		}

		if(pacman.vivo==false) {
			dibujaRectangulo(15, 19,20,0xF8, game_over, true);
            for(;;){}
		}

        if(pacman.puntos==70){
            dibujaRectangulo(17, 24,21,0xF8, win, true);
            for(;;){}
        }

	}


    return 0;
}

void pinta(int x, int y, char color) {

	unsigned char *vga = (unsigned char*)VGA_DIR;
	*(vga+ x + (y*128))=color;
	map[x-XBASE][y-YBASE]=color;
	//128 es la potencia de 2 superior a 80

}


void pintaOculto(int x, int y, char color) {

	unsigned char *vga = (unsigned char*)VGA_DIR;
	*(vga+ x + (y*128))=color;
	//128 es la potencia de 2 superior a 80

}

void displayPuntos(){

    int u,d,c,m, puntos;
    puntos=pacman.puntos;

    m=puntos/1000;
    c=puntos/100;
    d=puntos/10;
    u=(puntos-(1000*m+100*c+10*d));

    *displayr0=u;
    *displayr1=d;
    *displayr2=c;
    *displayr3=m;
}

void mostrarleds(){

    //PRINTEAR EN LOS LEDS
    char digitos[5]={0x0, 0x1, 0x3, 0x7, 0xF};
    *leds=(digitos[fantasmasVivos]);

}

/*void limpia(){

    int i,j;
    for(i=0; i<DIMENSIONX; i++){
        for(j=0; j<DIMENSIONY; j++){
            if((i>=XBASE && i<XFIN) && (j>=YBASE && j< YFIN)){
                pinta(i,j,VACIO);
            }else{
                pintaOculto(i,j,VACIO);
            }
        }
    }

}*/

void inicializaPersonajes(){

	int i;
	//inicializa pacman
	pacman.vivo=true;
	pacman.x = 14;
	pacman.y = 23;
	pacman.puntos =0;
	pinta(pacman.x+XBASE, pacman.y+YBASE, PACMAN);

	//inicializa Fantasmas

	fantasma[0].x = 1;
	fantasma[0].y = 1;
	/*fantasma[0].vivo = TRUE;
	fantasma[0].OnBaldosa = TRUE;
	fantasma[0].OnFruta = FALSE;*/
	fantasma[1].x = 26;
	fantasma[1].y = 1;
	/*fantasma[1].vivo = TRUE;
	fantasma[1].OnBaldosa = TRUE;
	fantasma[1].OnFruta = FALSE;*/
	fantasma[2].x = 6;
	fantasma[2].y = 14;
	/*fantasma[2].vivo = TRUE;
	fantasma[2].OnBaldosa = TRUE;
	fantasma[2].OnFruta = FALSE;*/
	fantasma[3].x = 21;
	fantasma[3].y = 14;
	/*fantasma[3].vivo = TRUE;
	fantasma[3].OnBaldosa = TRUE;
	fantasma[3].OnFruta = FALSE;*/

	for(i=0; i<4; i++){
		fantasma[i].vivo = TRUE;
		fantasma[i].OnBaldosa = TRUE;
		fantasma[i].OnFruta = FALSE;
		pinta(fantasma[i].x+XBASE, fantasma[i].y+YBASE, FANTASMA);
	}

	//inicializa Fruta
	pinta(1+XBASE, 3+YBASE, FRUTA);
	pinta(26+XBASE, 3+YBASE, FRUTA);
	pinta(26+XBASE, 23+YBASE, FRUTA);
	pinta(1+XBASE, 23+YBASE, FRUTA);

	//enciende leds
	fantasmasVivos=4;


}


void creaObstaculos(){

	int j,i;

	for (i=XBASE; i<XFIN ; i++) {
		for(j=YBASE; j<YFIN;j++) {
			pinta(i,j,BALDOSA);
		}
	}

	//pared exterior
	for (j= YBASE ; j< YFIN; j++){ //pintamos columnas exteriores
		pinta(XBASE,j,PARED);
		pinta(XFIN,j,PARED);
	}
	for (i=XBASE; i<XFIN; i++){ //pintamos filas exteriores
		pinta(i,YBASE,PARED);
		pinta(i,YFIN,PARED);
	}

	dibujaRectangulo(34, XBASE, YBASE, PARED, barriers, false);
}

void dibujaRectangulo (int numelem, int desplazamientox, int desplazamientoy, char color, struct obstaculos *array, bool oculto){
	int i, j, k, coordX, coordY;

	for (i=0; i<numelem; i++){
		for(k=0; k<(int)array[i].ladoy; k++){
			for(j=0; j<(int)array[i].ladox; j++){
				coordX = (int)array[i].x+desplazamientox+j;
				coordY = (int)array[i].y+desplazamientoy+k;
				if(oculto){
					pintaOculto(coordX,coordY,color);
				}else {
					pinta(coordX,coordY,color);
				}
			}
		}
	}

}

void mataFantasma(int x, int y){

	int i;
	//identificar fantasma
	for(i=0; i<4; i++){
		if(fantasma[i].x==x && fantasma[i].y==y){
			fantasma[i].vivo=false;
			fantasmasVivos--;
		}
	}
}

/**
 * Return 0 -> Todo Correcto
 * 		  1 -> Choque Fantasma
 * 		  2 -> Choque Fantasma con fruta
 * 		  3 -> Choque fruta
 * */
void moverPacman(Direccion dir){

	int newCoordX;
	int newCoordY;

	//calcular nueva coordenada
	switch (dir)
	{
	case arriba:
		/* code */
		newCoordX=pacman.x;
		newCoordY=pacman.y-1;
		break;
	case abajo:
		/* code */
		newCoordX=pacman.x;
		newCoordY=pacman.y+1;
		break;
	case izquierda:
		/* code */
		newCoordX=pacman.x-1;
		newCoordY=pacman.y;
		break;
	case derecha:
		/* code */
		newCoordX=pacman.x+1;
		newCoordY=pacman.y;
		break;
	default:
		break;
	}

	//Comprobar si es posible
	if(map[newCoordX][newCoordY] != PARED){
		if(map[newCoordX][newCoordY] == FANTASMA){
			//Tiene superpoderes?
			if(pacman.poderes){
				mataFantasma(newCoordX,newCoordY);
			}else{
				pacman.vivo=false;
				return;
			}

		} else if(map[newCoordX][newCoordY] == FRUTA){
			//Dar superpoderes
			pacman.poderes=true;
			pacman.movPoderes=MOVPODERES;
		}else if(map[newCoordX][newCoordY] == BALDOSA){
					//Dar superpoderes
					pacman.puntos++;
		}
		if(pacman.poderes){
			if(pacman.movPoderes%2==0){
				//pinta(pacman.x+XBASE,pacman.y+YBASE,VACIO);
				pinta(newCoordX+XBASE,newCoordY+YBASE,PACMAN);
				/*pacman.x=newCoordX;
				pacman.y=newCoordY;*/
			}else{
				//pinta(pacman.x+XBASE,pacman.y+YBASE,VACIO);
				pintaOculto(newCoordX+XBASE,newCoordY+YBASE,0xF8);
				/*pacman.x=newCoordX;
				pacman.y=newCoordY;*/
			}



		}else {
			//pinta(pacman.x+XBASE,pacman.y+YBASE,VACIO);
			pinta(newCoordX+XBASE,newCoordY+YBASE,PACMAN);
			/*pacman.x=newCoordX;
			pacman.y=newCoordY;*/
		}
		pinta(pacman.x+XBASE,pacman.y+YBASE,VACIO);
		pacman.x=newCoordX;
		pacman.y=newCoordY;


		if(pacman.poderes){
			pacman.movPoderes--;
			if(pacman.movPoderes==0){
				pacman.poderes=false;
			}
		}
		/*if(pacman.movPoderes==0 && pacman.poderes) {
			pacman.poderes=false;
		}*/

	}
	return;
}
/**
 * Return 0 -> Todo Correcto
 * 		  1 -> Choque Pacman
 * 		  2 -> Choque Pacman con poderes
 * */
void moverFantasma(int id){

	int newCoordX = fantasma[id].x;
	int newCoordY = fantasma[id].y;
	Direccion direcciones[4];
	int direccionesPosibles=0;

	if(fantasma[id].vivo){
		//IA de los fantasmas
		char check =map[newCoordX][newCoordY+1];
		if((check != PARED) && (check != FANTASMA)){ //POSIBLES DIRECCIONES
			//if(fantasma[id].previa != arriba) {
				direcciones[direccionesPosibles]=abajo;
				direccionesPosibles++;
			//}
		}

		check =map[newCoordX][newCoordY-1];
		if ((check != PARED) && (check != FANTASMA)){
			//if(fantasma[id].previa != abajo) {
				direcciones[direccionesPosibles]=arriba;
				direccionesPosibles++;
			//}
		}
		check =map[newCoordX+1][newCoordY];
		if ((check != PARED) && (check != FANTASMA)){
			//if(fantasma[id].previa != izquierda) {
				direcciones[direccionesPosibles]=derecha;
				direccionesPosibles++;
			//}
		}

		check =map[newCoordX-1][newCoordY];
		if ((check != PARED) && (check != FANTASMA)){
			//if(fantasma[id].previa != derecha) {
				direcciones[direccionesPosibles]=izquierda;
				direccionesPosibles++;
			//}
		}

		Direccion dir = direcciones[rand() % direccionesPosibles];
		fantasma[id].previa = dir;
		//calcular nueva coordenada
		switch (dir)
		{
		case arriba:
			/* code */
			newCoordX=fantasma[id].x;
			newCoordY=fantasma[id].y-1;
			break;
		case abajo:
			/* code */
			newCoordX=fantasma[id].x;
			newCoordY=fantasma[id].y+1;
			break;
		case izquierda:
			/* code */
			newCoordX=fantasma[id].x-1;
			newCoordY=fantasma[id].y;
			break;
		case derecha:
			/* code */
			newCoordX=fantasma[id].x+1;
			newCoordY=fantasma[id].y;
			break;
		default:
			break;
		}

		//Comprobar si es posible
		if((map[newCoordX][newCoordY]) != PARED){
			if((map[newCoordX][newCoordY]&0xFF) == PACMAN){
				//Tiene superpoderes?
				if(pacman.poderes){
					fantasma[id].vivo=false; //Fantasma muere
					pinta(fantasma[id].x+XBASE,fantasma[id].y+YBASE,VACIO); // limpio pixel
					fantasmasVivos--;
					return; //notifico
				}else{
					pacman.vivo=false;
				}

			}
			if(fantasma[id].OnFruta==true){ //Si el fantasma esta sobre fruta, hay que pintar la fruta
				if (map[newCoordX][newCoordY] == BALDOSA){ //Si el fantasma se mueve encima de una fruta, es un estado nuevo
					fantasma[id].OnBaldosa=true;
				}
				pinta(fantasma[id].x+XBASE,fantasma[id].y+YBASE,FRUTA);
				/*pinta(newCoordX+XBASE,newCoordY+YBASE,FANTASMA);
				fantasma[id].x=newCoordX;
				fantasma[id].y=newCoordY;*/
				fantasma[id].OnFruta=false;

			}else if(fantasma[id].OnBaldosa==true){ //Si el fantasma esta sobre Baldosa, hay que pintar la fruta
				fantasma[id].OnBaldosa=false;
				if (map[newCoordX][newCoordY] == BALDOSA){ //Si el fantasma se mueve encima de una fruta, es un estado nuevo
					fantasma[id].OnBaldosa=true;
				}else if (map[newCoordX][newCoordY] == FRUTA){ //Si el fantasma se mueve encima de una fruta, es un estado nuevo
					fantasma[id].OnFruta=true;
				}
				pinta(fantasma[id].x+XBASE,fantasma[id].y+YBASE,BALDOSA);
				/*pinta(newCoordX+XBASE,newCoordY+YBASE,FANTASMA);
				fantasma[id].x=newCoordX;
				fantasma[id].y=newCoordY;*/

			}else {
				if (map[newCoordX][newCoordY] == BALDOSA){ //Si el fantasma se mueve encima de una fruta, es un estado nuevo
					fantasma[id].OnBaldosa=true;
				}else if (map[newCoordX][newCoordY] == FRUTA){ //Si el fantasma se mueve encima de una fruta, es un estado nuevo
					fantasma[id].OnFruta=true;
				}
				pinta(fantasma[id].x+XBASE,fantasma[id].y+YBASE,VACIO);
				/*pinta(newCoordX+XBASE,newCoordY+YBASE,FANTASMA);
				fantasma[id].x=newCoordX;
				fantasma[id].y=newCoordY;*/
			}
			pinta(newCoordX+XBASE,newCoordY+YBASE,FANTASMA);
			fantasma[id].x=newCoordX;
			fantasma[id].y=newCoordY;
		}
	}
	return;
}

/**
*Función que maneja la interrupción
**/
void myISR(void){ 				//Handler interrupcion

	if(INTC_IPR & TIMER_INT) {	// Timer Interrupt Is Pending
		cont++;
		if(cont == 1000){
			flag = TRUE;
			cont=0;
		}
		TCSR0 = TCSR0; 			//Limpia la interrupcion
	}
	INTC_IAR = INTC_IPR;
}

/**
*Función que incializa el timer al inicio de la ejecución del programa
*
**/
void init_timer() {

	flag=FALSE;					//Change boolean flag to false
	cont=0;						//Inicio el contador de ticks de reloj

	TLR0=50000;					//1 microsegundo de interrupcion son 50000 tics de clock

	TCSR0 = 0x000007F6; 		//Timer Load and Clear any Pending Ints
	TCSR0 = 0x000007D6; 		//Timer Clear Load Bit

	INTC_IER = TIMER_INT ;		//Enable Timer and Button Interrupt in IntC
	INTC_MER = 3; 				//Int
}

