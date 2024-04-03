/*
 * LCD_menus.c
 *
 *  Created on: Jul 8, 2023
 *      Author: christian
 */

#include "LCD.h"

#define DELAY_POST_PRINT HAL_Delay(150)

void printHome(uint8_t clear){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(6, 2);
	LCD_printString("En Espera...");

	DELAY_POST_PRINT;
}




void printViewT1(uint8_t clear, uint16_t T, type_data_t1 mode){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);


	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T1:");


	if(T <= 9)
		LCD_setCursor(11, 2);
	else if(T <= 99)
		LCD_setCursor(10, 2);
	else if(T <= 999)
		LCD_setCursor(9, 2);
	else if(T <= 9999)
		LCD_setCursor(8, 2);
	else
		LCD_setCursor(7, 2);

	LCD_printInt(T);
	LCD_printString("   us");

	DELAY_POST_PRINT;

}


void printViewT2(uint8_t clear, uint16_t T){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);


	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T2:");


	if(T <= 9)
		LCD_setCursor(11, 2);
	else if(T <= 99)
		LCD_setCursor(10, 2);
	else if(T <= 999)
		LCD_setCursor(9, 2);
	else
		LCD_setCursor(8, 2);


	LCD_printInt(T);
	LCD_printString(" ms");

	DELAY_POST_PRINT;

}



void printViewT3(uint8_t clear, uint16_t T){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);


	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T3:");


	if(T <= 9)
		LCD_setCursor(11, 2);
	else if(T <= 99)
		LCD_setCursor(10, 2);
	else if(T <= 999)
		LCD_setCursor(9, 2);
	else
		LCD_setCursor(8, 2);


	LCD_printInt(T);
	LCD_printString(" ms");

	DELAY_POST_PRINT;

}






void printEditT1(uint8_t clear, uint16_t T, type_data_t1 mode){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T1: > ");

	if(T <= 9)
		LCD_printString("    ");
	else if(T <= 99)
		LCD_printString("   ");
	else if(T <= 999)
		LCD_printString("  ");
	else if(T <= 9999)
		LCD_printString(" ");


	LCD_printInt(T);

	switch(mode){
		case T1_GRADOS:
			LCD_printString(" <  ");
			LCD_printChar(223);
			break;
		case T1_US:
			LCD_printString(" < us");
			break;
		case T1_MS:
			LCD_printString(" < ms");
			break;
	}

	DELAY_POST_PRINT;
}



void printEditT2(uint8_t clear, uint16_t T){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T2: >  ");

	if(T <= 9)
		LCD_printString("   ");
	else if(T <= 99)
		LCD_printString("  ");
	else
		LCD_printString(" ");


	LCD_printInt(T);
	LCD_printString(" < ms");

	DELAY_POST_PRINT;
}



void printEditT3(uint8_t clear, uint16_t T){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T3: >  ");

	if(T <= 9)
		LCD_printString("   ");
	else if(T <= 99)
		LCD_printString("  ");
	else
		LCD_printString(" ");

	LCD_printInt(T);
	LCD_printString(" < ms");


	DELAY_POST_PRINT;
}




void printEditT1Unit(uint8_t clear, uint16_t T, type_data_t1 mode){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);


	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);
	LCD_printString("T1:");


	if(T <= 9)
		LCD_setCursor(11, 2);
	else if(T <= 99)
		LCD_setCursor(10, 2);
	else if(T <= 999)
		LCD_setCursor(9, 2);
	else if(T <= 9999)
		LCD_setCursor(8, 2);
	else
		LCD_setCursor(7, 2);

	LCD_printInt(T);

	switch(mode){
		case T1_GRADOS:
			LCD_printString(" >  ");
			LCD_printChar(223);
			LCD_printString(" <");
			break;
		case T1_US:
			LCD_printString(" > us <");
			break;
		case T1_MS:
			LCD_printString(" > ms <");
			break;
	}

	DELAY_POST_PRINT;

}





void printPreRun(uint8_t clear, uint16_t T1, uint16_t T2, uint16_t T3){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");
	LCD_setCursor(1, 2);

	LCD_printString("Iniciar?");

	//Imprimo T1
	LCD_setCursor(10, 1);
	if(T1 < 10)
		LCD_printString("T1:    ");
	else if(T1 < 100)
		LCD_printString("T1:   ");
	else if(T1 < 1000)
		LCD_printString("T1:  ");
	else if(T1 < 10000)
		LCD_printString("T1: ");
	else
		LCD_printString("T1:");

	LCD_printInt(T1);
	LCD_printString("us");

	//Imprimo T2
	LCD_setCursor(10, 2);
	if(T2 < 10)
		LCD_printString("T2:    ");
	else if(T2 < 100)
		LCD_printString("T2:   ");
	else if(T2 < 1000)
		LCD_printString("T2:  ");
	else
		LCD_printString("T2: ");

	LCD_printInt(T2);
	LCD_printString("ms");

	//Imprimo T3
	LCD_setCursor(10, 3);
	if(T3 < 10)
		LCD_printString("T3:    ");
	else if(T3 < 100)
		LCD_printString("T3:   ");
	else if(T3 < 1000)
		LCD_printString("T3:  ");
	else
		LCD_printString("T3: ");

	LCD_printInt(T3);
	LCD_printString("ms");

	DELAY_POST_PRINT;
}



void printRunning(uint16_t T1, uint16_t T2, uint16_t T3){
	LCD_Clear();
	LCD_printString("ARMANDO EN   ...");
	LCD_setCursor(1, 2);

	//Imprimo T1
	LCD_setCursor(2, 1);
	if(T1 < 10)
		LCD_printString("T1:        ");
	else if(T1 < 100)
		LCD_printString("T1:       ");
	else if(T1 < 1000)
		LCD_printString("T1:      ");
	else if(T1 < 10000)
		LCD_printString("T1:     ");
	else
		LCD_printString("T1:    ");

	LCD_printInt(T1);
	LCD_printString("  us");

	//Imprimo T2
	LCD_setCursor(2, 2);
	if(T2 < 10)
		LCD_printString("T2:        ");
	else if(T2 < 100)
		LCD_printString("T2:       ");
	else if(T2 < 1000)
		LCD_printString("T2:      ");
	else
		LCD_printString("T2:     ");

	LCD_printInt(T2);
	LCD_printString("  ms");

	//Imprimo T3
	LCD_setCursor(2, 3);
	if(T3 < 10)
		LCD_printString("T3:        ");
	else if(T3 < 100)
		LCD_printString("T3:       ");
	else if(T3 < 1000)
		LCD_printString("T3:      ");
	else
		LCD_printString("T3:     ");

	LCD_printInt(T3);
	LCD_printString("  ms");

	for(uint8_t t = 5; t > 0; t--){
		LCD_setCursor(12, 0);
		LCD_printInt(t);
		HAL_Delay(1000);
	}

	LCD_setCursor(0, 0);
	LCD_printString(">ESPERANDO FLANCO...");


	DELAY_POST_PRINT;

}



void printTriggered(){
	LCD_Clear();

	LCD_setCursor(1, 1);
	LCD_printString("DISPARO EFECTUADO!");

}







void printViewConfigT1(uint8_t clear, uint8_t mode){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("T1:       ");
	if(mode) {
		LCD_printString("  us");
	} else {
		LCD_printString("grados");
	}

	DELAY_POST_PRINT;
}




void printEditConfigT1(uint8_t clear, type_data_t1 mode){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("T1:     > ");
	if(mode == T1_US) {
		LCD_printString("  us  ");
	} else if(mode == T1_GRADOS) {
		LCD_printString("grados");
	}
	LCD_printString(" <");

	DELAY_POST_PRINT;
}





void printViewConfigLogT1(uint8_t clear, uint8_t logicaT){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("Q1_on:   ");
	if(logicaT) {
		LCD_printString("HIGH");
	} else {
		LCD_printString(" LOW");
	}

	DELAY_POST_PRINT;
}


void printViewConfigLogT2(uint8_t clear, uint8_t logicaT){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("Q2_on:   ");
	if(logicaT) {
		LCD_printString("HIGH");
	} else {
		LCD_printString(" LOW");
	}

	DELAY_POST_PRINT;
}





void printEditConfigLogT1(uint8_t clear, uint8_t logicaT){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("Q1_on: > ");
	if(logicaT) {
		LCD_printString("HIGH");
	} else {
		LCD_printString(" LOW");
	}

	LCD_printString(" <");

	DELAY_POST_PRINT;
}




void printEditConfigLogT2(uint8_t clear, uint8_t logicaT){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("Q2_on: > ");
	if(logicaT) {
		LCD_printString("HIGH");
	} else {
		LCD_printString(" LOW");
	}

	LCD_printString(" <");

	DELAY_POST_PRINT;
}



void printConfigVolver(uint8_t clear){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">CONFIGURACIONES");

	LCD_setCursor(1, 2);
	LCD_printString("Volver?");

	DELAY_POST_PRINT;
}





void printConfigEntrar(uint8_t clear){
	if(clear)
		LCD_Clear();
	else
		LCD_setCursor(0,0);

	LCD_printString(">MENU PRINCIPAL");

	LCD_setCursor(1, 2);
	LCD_printString("Configuraciones");

	DELAY_POST_PRINT;
}






