/*
 * LCD.c
 *
 *  Created on: Jul 8, 2023
 *      Author: christian
 */


#include "LCD.h"

extern void delay_us (uint32_t us);

/************************************************************************/
/*            DEFINICIONES, VARIABLES Y FUNCIONES PRIVADAS              */
/************************************************************************/
/* Macros que permiten cambiar los estados de los pines */
#define LCD_RS_HIGH HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define LCD_RS_LOW	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)
#define LCD_E_HIGH	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define LCD_E_LOW	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)
#define LCD_DB4(data) if(data) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET)
#define LCD_DB5(data) if(data) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define LCD_DB6(data) if(data) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_DB7(data) if(data) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); else HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)

/* Mascaras para las diversos modos de funcionamientos */
#define LCD_CMD_CLEAR		(1 << 0)
#define LCD_CMD_HOME		(1 << 1)
#define LCD_CMD_MODE_SET	(1 << 2)
#define LCD_CMD_DISPLAY		(1 << 3)
#define LCD_CMD_CURSOR		(1 << 4)
#define LCD_CMD_FUNCTION	(1 << 5)
#define LCD_CMD_CGRAM_ADD	(1 << 6)
#define LCD_CMD_DDRAM_ADD	(1 << 7)


/* Funciones Privadas */
static void LCD_Send(uint8_t cmd, type_data_send data);
static void LCD_sendNibble(uint8_t data);

/* Variables Privadas */
static char LCD_buff[10];





/************************************************************************/
/*                         FUNCIONES PUBLICAS                           */
/************************************************************************/

void LCD_Init(){

	HAL_Delay(500);

	LCD_sendNibble(0x03);
	HAL_Delay(6);
	LCD_sendNibble(0x03);
	HAL_Delay(1);
	LCD_sendNibble(0x03);
	HAL_Delay(1);


	//4 Bits
	LCD_sendNibble(0x02);

	//4 Bits + 2 Lineas
	LCD_Send(LCD_CMD_FUNCTION | 0b00001100, LCD_COMMAND);

	//Encender Pantalla + Cursor Apagado
	LCD_Send(LCD_CMD_DISPLAY | 0b00000100, LCD_COMMAND);

	//Cursor Incremental
	LCD_Send(LCD_CMD_MODE_SET | 0b00000010, LCD_COMMAND);

	//Limpiar la pantalla
	LCD_Clear();





}


void LCD_Clear(){
	LCD_Send(LCD_CMD_CLEAR, LCD_COMMAND);
	HAL_Delay(5);
}


void LCD_setCursor (uint8_t col, uint8_t row){
	uint8_t offset[] = {0x00, 0x40, 0x14, 0x54};
	if(row > 3) row=3;
	LCD_Send(LCD_CMD_DDRAM_ADD | ((col) + offset[row]), LCD_COMMAND);
}



void LCD_printChar (uint8_t data)	{
	LCD_Send(data, LCD_DATA);
}



void LCD_printString (char *str) {
	while(*str != 0x00){
		LCD_printChar(*str);
		str++;
	}
}


void LCD_printInt(uint16_t val){
	itoa(val, LCD_buff, 10);
	LCD_printString(LCD_buff);
}

void LCD_printFloat(float val, char * data){
	//dtostrf(val, min_width, dig, LCD_buff);
	sprintf(LCD_buff, data, val);
	LCD_printString(LCD_buff);
}




/************************************************************************/
/*                         FUNCIONES PRIVADAS                           */
/************************************************************************/

static void LCD_Send(uint8_t cmd, type_data_send data){
	if(data == LCD_COMMAND)
		LCD_RS_LOW;
	else if(data == LCD_DATA)
		LCD_RS_HIGH;

	LCD_sendNibble(cmd >> 4);
	LCD_sendNibble(cmd);

}




static void LCD_sendNibble(uint8_t data){
	LCD_DB4(data & 0x01);
	LCD_DB5((data >> 1) & 0x01);
	LCD_DB6((data >> 2) & 0x01);
	LCD_DB7((data >> 3) & 0x01);

	LCD_E_HIGH;
	delay_us(100);
	LCD_E_LOW;
}



