/*
 * main.h
 *
 *  Created on: Aug 7, 2025
 *      Author: s0953
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <string.h>
//Peripheral Driver
#include "stm32f746xx.h"
extern I2C_Handle_t hi2c1;
extern UART_Handle_t usart6;


//BSP Driver
#include "sht2x.h"
#include "ssd1306.h"
//#include "liquidcrystal_i2c.h"
//RTOS
#include "FreeRTOS.h"
#include "Task.h"
#include "queue.h"
#include "timers.h"

extern xTaskHandle handle_cmd_task;
extern xTaskHandle handle_menu_task;
extern xTaskHandle handle_uart_print_task;
extern xTaskHandle handle_i2c_task;
extern xTaskHandle handle_th_task;

extern void cmd_task(void * parameters);
extern void menu_task(void * parameters);
extern void uart_print_task(void * parameters);
extern void i2c_task(void * parameters);
extern void th_task(void * parameters);

//queue
extern QueueHandle_t q_uartrx;
extern QueueHandle_t q_uarttx;
extern QueueHandle_t q_i2crx;
extern QueueHandle_t q_i2ctx;

//oled state
typedef enum{
    oIdle = 0,
    oRunning,
    closing
}OLED_state;
extern OLED_state oled_state;

//command
typedef struct
{
	uint8_t payload[10];
	uint32_t len;
}command_t;


extern uint8_t i2c_rx_data[3];//I2C 
extern uint8_t i2c_tx_data;

//OLED
extern uint8_t dt_1[256];
extern uint8_t dt_2[2];
#endif /* MAIN_H_ */
