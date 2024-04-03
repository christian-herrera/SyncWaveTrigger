/*
 * LCD.h
 *
 *  Created on: Jul 8, 2023
 *      Author: christian
 */

#ifndef INC_LCD_LCD_H_
#define INC_LCD_LCD_H_

#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include <stdio.h>

typedef enum {LCD_COMMAND, LCD_DATA} type_data_send;

typedef enum {T1_GRADOS, T1_US, T1_MS} type_data_t1;


/************************************************************************/
/*                         FUNCIONES PUBLICAS                           */
/************************************************************************/

//	Inicializa el LCD y los valores por defecto
void LCD_Init();


//	Limpia completamente el LCD
void LCD_Clear();


//	Posiciona el cursor en una coordenada (col, row)
//	Parametros:
//	--> [uint8_t] col:  Indice de la columna
//	--> [uint8_t] row:  Indice de la fila
void LCD_setCursor (uint8_t col, uint8_t row);


//	Imprime un caracter en la posicion actual
//	Parametros:
//	--> [uint8_t] data:  Byte que se imprime
void LCD_printChar (uint8_t data);


//	Imprime una cadena de caracteres a partir de la posicion actual
//	Parametros:
//	--> [char *] str:  Inicio de la cadena
void LCD_printString (char *str);


//	Imprime un entero sin signo
//	Parametros:
//	--> [uint16_t] val:  Entero sin signo
void LCD_printInt(uint16_t val);


//	Imprime un Float
//	Parametros:
//	--> [float]		val			: Valor de Punto Flotante
//	--> [int8_t]	min_width	: Tama�o minimo, completa con espacios
//	--> [uint8_t]	dig			: Cantidad de digitos despues del punto decimal
void LCD_printFloat(float val, char * data);





/************************************************************************/
/*                      IMPRESION DE PANTALLAS                          */
/************************************************************************/
void printHome(uint8_t clear);

void printViewT1(uint8_t clear, uint16_t T, type_data_t1 mode);
void printViewT2(uint8_t clear, uint16_t T);
void printViewT3(uint8_t clear, uint16_t T);
void printEditT1(uint8_t clear, uint16_t T, type_data_t1 mode);
void printEditT2(uint8_t clear, uint16_t T);
void printEditT3(uint8_t clear, uint16_t T);
void printEditT1Unit(uint8_t clear, uint16_t T, type_data_t1 mode);

void printPreRun(uint8_t clear, uint16_t T1, uint16_t T2, uint16_t T3);
void printRunning(uint16_t T1, uint16_t T2, uint16_t T3);
void printTriggered();

void printConfigEntrar(uint8_t clear);

void printViewConfigT1(uint8_t clear, type_data_t1 mode);
void printEditConfigT1(uint8_t clear, type_data_t1 mode);

void printViewConfigLogT1(uint8_t clear, uint8_t logicaT);
void printEditConfigLogT1(uint8_t clear, uint8_t logicaT);
void printViewConfigLogT2(uint8_t clear, uint8_t logicaT);
void printEditConfigLogT2(uint8_t clear, uint8_t logicaT);


void printConfigVolver(uint8_t clear);

#endif /* INC_LCD_LCD_H_ */
