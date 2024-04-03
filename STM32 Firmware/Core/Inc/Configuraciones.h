/*
 * Configuraciones.h
 *
 *  Created on: Jul 8, 2023
 *      Author: christian
 */

#ifndef INC_CONFIGURACIONES_H_
#define INC_CONFIGURACIONES_H_



/************************************************************************/
/*					CONFIGURACIONES PARA EL HARDWARE                    */
/************************************************************************/
//	Inversion de la logica del Encoder Rotativo
#define INV_LOGICA_ENCODER	1
#define INV_LOGICA_SW		0






/************************************************************************/
/*     VALORES DE LOS SALTOS Y DE LOS MAXIMOS PARA LOS TIEMPOS          */
/************************************************************************/
//Maximos valores para los tiempos
#define MAX_T1_US 20000						//us
#define MAX_T1_MS MAX_T1_US/1000.0
#define MAX_T1_GRADOS MAX_T1_MS*360.0/20.0
#define MAX_T2 5000							//ms
#define MAX_T3 5000							//ms

//Minimos saltos
#define MIN_SALTO_T1 	10		//us
#define MIN_SALTO_T2	50		//ms
#define MIN_SALTO_T3	50		//ms





/************************************************************************/
/*     CONFIGURACIONES DE TIEMPO ESPECIFICAS DE LAS FUNCIONES          	*/
/************************************************************************/
//Minimo us para adicionar 20ms al retardo total.
//Esto por que al tratar de lograr un retardo menor al indicado,
//no es capaz de lograrlo, asi que espero al siguiente cruce por cero.
#define MIN_T1_TO_ADD_20MS 	300		//us


//Tiempos con los que decide si es una pulsacion larga (LT) o corta (ST)
#define MIN_MS_ENCODER_LT	500		//ms
#define MAX_MS_ENCODER_ST	50		//ms






/************************************************************************/
/*     						OFFSETS DISPONIBLES          				*/
/************************************************************************/
//Offset para el retardo del disparo principal
#define TIME_OFFSET 1





/************************************************************************/
/*				LOGICA PARA LOS PINES DE SALIDAS          				*/
/************************************************************************/
#define OUT1_ON(logic)  HAL_GPIO_WritePin(OUT_T1_GPIO_Port, OUT_T1_Pin, logic ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define OUT1_OFF(logic) HAL_GPIO_WritePin(OUT_T1_GPIO_Port, OUT_T1_Pin, logic ? GPIO_PIN_RESET : GPIO_PIN_SET)

#define OUT2_ON(logic)  HAL_GPIO_WritePin(OUT_T2_GPIO_Port, OUT_T2_Pin, logic ? GPIO_PIN_SET : GPIO_PIN_RESET)
#define OUT2_OFF(logic) HAL_GPIO_WritePin(OUT_T2_GPIO_Port, OUT_T2_Pin, logic ? GPIO_PIN_RESET : GPIO_PIN_SET)



#endif /* INC_CONFIGURACIONES_H_ */
