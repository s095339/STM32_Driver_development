/*
 * task_handle.c
 *
 *  Created on: Aug 7, 2025
 *      Author: s0953
 */


#include "main.h"
static int extract_command(command_t* cmd);
const char *msg_inv = "///Invalid option///\n";
uint8_t Show_TH_UART_flag;
void cmd_task(void * parameters){
    BaseType_t ret;
	command_t cmd;
	while(1)
	{
		// wait for notification from uart isr
		ret = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		if(ret == pdTRUE){
			// process the user data(command) stored in input data queue;
			SEGGER_SYSVIEW_PrintfTarget("UART command\n");
			extract_command(&cmd);
            xTaskNotify(handle_menu_task, (uint32_t)&cmd, eSetValueWithOverwrite);
		}
	}
}
void menu_task(void * parameters){
    uint32_t cmd_addr;
	command_t *cmd;
    const char * s;
	int option;
	const char* msg_menu = "MENU:=====================\n"
							"Temperature & HR ----->10\n"
							"Turn on OLED     ----->20\n"
							"Turn off OLED    ----->30\n"
							"Enter your choice here:  \n";
	while(1)
	{
		xQueueSend(q_uarttx, &msg_menu, portMAX_DELAY); //系統啟動的時候最最一開始會先print一個menu出來
		//wait for menu commands
		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
		cmd = (command_t *)cmd_addr;

        portENTER_CRITICAL();
        Show_TH_UART_flag = 0;
        portEXIT_CRITICAL();

		if(cmd->len == 1)
		{
			
			option = cmd->payload[0]-48;
			switch(option)
			{
			case 1:
                s = "Request for Temperature and Humid Rate\n";
				xQueueSend(q_uarttx, &s, portMAX_DELAY);
                if(oled_state == oIdle)
                {
                    xTaskNotify(handle_i2c_task,0,eNoAction);
                }
                portENTER_CRITICAL();
                Show_TH_UART_flag = 1;
                portEXIT_CRITICAL();
				break;
			case 2:
                s = "Turn on the OLED\n";
				
                if(oled_state == oIdle)
                {   	
                    xTaskNotify(handle_i2c_task,0,eNoAction);
                }

				portENTER_CRITICAL();
				oled_state = oRunning;
				portEXIT_CRITICAL();

				xQueueSend(q_uarttx, &s, portMAX_DELAY);
				break;
			case 3:
                s = "Turn off the OLED\n";
                xQueueSend(q_uarttx, &s, portMAX_DELAY);
				if(oled_state == oRunning)
				{
					portENTER_CRITICAL();
                	oled_state = closing;
					portEXIT_CRITICAL();
				}
				break;
			default:
				xQueueSend(q_uarttx, &msg_inv, portMAX_DELAY);
				continue;

			}
		}else
		{//invalid entry(在這邊command的長度只會是1)
			xQueueSend(q_uarttx, &msg_inv, portMAX_DELAY);
		}
		vTaskDelay(pdMS_TO_TICKS(100));
		//wait for menu commands
		//xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);


	}//end of loop
}
void uart_print_task(void * parameters){
    uint32_t *msg;
	while(1){
		xQueueReceive(q_uarttx, &msg, portMAX_DELAY);
		//HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen((char*)msg),HAL_MAX_DELAY);
		uint32_t aa = strlen((char*)msg);

        while(UART_SendDataIT(&usart6, (uint8_t*)msg, aa)!=UART_READY);
       

	}
}
void i2c_task(void * parameters){
    //SHT2x_SetResolution(RES_14_12);

    BaseType_t ret;
    BaseType_t status;
	
    //運算用
    
    uint16_t received_data;
    float temp;
    float cel;
    float rh;
    
    char buffer[20];
    char *pBuffer;
	while(1){
        // Temperature
		if(oled_state == oIdle){
			xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		}
		int a = 1;

		//Measurement====================================================
        SHT2x_GetTemperatureIT(0);
        xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
        status = xQueueReceive(q_i2crx, &received_data, portMAX_DELAY);
        if(status == pdTRUE){
            //temperature 資料//
            
            
            temp = (received_data/65536.0);
            cel =  -46.85 + 175.72 * temp;
        }

        
        
		

        // Relative Humid
        SHT2x_GetRelativeHumidityIT(0);
        xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		//============================================================

        status = xQueueReceive(q_i2crx, &received_data, portMAX_DELAY);


        if(status == pdTRUE){
            //Relative Humidity 資料//
            rh = -6 + 125.00 * (received_data / 65536.0);
        }
		
        
		if(oled_state == oRunning)
		{  
		   
		   sprintf(buffer,"T = %d.%dC \n RH: %d.%d", 
				SHT2x_GetInteger(cel), 
				SHT2x_GetDecimal(cel, 1),
				SHT2x_GetInteger(rh), 
				SHT2x_GetDecimal(rh, 1)
			);
		   //SSD1306_GotoXY(0, 0);
	   	   //SSD1306_Puts(buffer, &Font_11x18, 1);
		   //delay_ms(500);
		   pBuffer = buffer;
		   xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);

		   //sprintf(buffer,"RH: %d.%d", SHT2x_GetInteger(rh), SHT2x_GetDecimal(rh, 1));
		   //SSD1306_GotoXY(0, 30);
		   //SSD1306_Puts(buffer, &Font_11x18, 1);
		   //SSD1306_UpdateScreen();
		   //pBuffer = buffer;
		   //xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);
		}else if(oled_state == closing){
			
			//SSD1306_DrawBitmap(0, 0, img, 128, 64, 1);
			//SSD1306_Clear();
			buffer[0] = '\0';
			pBuffer = buffer;
		    xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);

			portENTER_CRITICAL();
			oled_state = oIdle;
			portEXIT_CRITICAL();
		}
        
      
        if(Show_TH_UART_flag ){
			SEGGER_SYSVIEW_PrintfTarget("UART shows TH\n");
			portENTER_CRITICAL();
			Show_TH_UART_flag = 0;
			portEXIT_CRITICAL();
			pBuffer = buffer;
			sprintf(buffer,
				"T = %d.%dC\nRH: %d.%d\n", 
				SHT2x_GetInteger(cel), SHT2x_GetDecimal(cel, 1),
				SHT2x_GetInteger(rh), SHT2x_GetDecimal(rh, 1));
			xQueueSend(q_uarttx, &pBuffer, portMAX_DELAY);
			
		}
        //SSD1306_GotoXY(0, 0);
        //SSD1306_Puts(buffer, &Font_11x18, 1);
	}
}
void oled_task(void * parameters){
    //SHT2x_SetResolution(RES_14_12);
    uint32_t *msg;
    BaseType_t status;
	while(1){
		status = xQueueReceive(q_i2ctx, &msg, portMAX_DELAY);
		//HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen((char*)msg),HAL_MAX_DELAY);
		if(status == pdTRUE)
		{
			char * msg_oled = (char*)msg;
			char * pNL = NULL;
			uint8_t i;
			uint32_t Len = strlen((char*)msg);

			if(Len == 0){
				SSD1306_Clear();
				continue;
			}

			for(i=0; msg_oled[i]!='\n' && i<Len; i++);
			if(msg_oled[i] == '\n') msg_oled[i] = '\0';
			SSD1306_GotoXY(0, 0);
			SSD1306_Puts(msg_oled, &Font_11x18, 1);
			if(i<Len)pNL = msg_oled+i+1;
			SSD1306_GotoXY(0, 30);
			SSD1306_Puts(pNL, &Font_11x18, 1);
			SSD1306_UpdateScreen();
		}
	}
}

void bt_task(void * parameters)
{
	const char * uart1_msg1 = "BT_CONNECTED";
	const char * uart1_msg2 = "BT_DISCONNECTED";
	char buffer[20];
	char * pBuffer;
	BaseType_t status;
	while(1)
	{
		status = xTaskNotifyWait(0,0,NULL,portMAX_DELAY);
		if(status != pdTRUE) continue;
		if(BT_ISCONNECTED())
		{
			//UART_SendDataIT(&usart1, (uint8_t *)uart1_msg1, strlen(uart1_msg1));
			//xTaskNotifyFromISR(bt_task,(uint32_t)&bt_cmd, eSetValueWithOverwrite,NULL);
			sprintf(buffer,"BlueTooth\nConnected");
		   //SSD1306_GotoXY(0, 0);
	   	   //SSD1306_Puts(buffer, &Font_11x18, 1);
		   //delay_ms(500);
		   pBuffer = buffer;
		   xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);
		   delay_ms(1000);
		}
		else
		{
			//UART_SendDataIT(&usart1, (uint8_t *)uart1_msg2, strlen(uart1_msg2));
			sprintf(buffer,"BlueTooth\nDisconnected");
		   //SSD1306_GotoXY(0, 0);
	   	   //SSD1306_Puts(buffer, &Font_11x18, 1);
		   //delay_ms(500);
		   pBuffer = buffer;
			
			
		   xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);
		   delay_ms(1000);
		   
		   if(oled_state == oRunning){
		   		portENTER_CRITICAL();
				oled_state = closing;
				portEXIT_CRITICAL();
		   }
		   
		}
		memset(buffer, 0, sizeof(buffer));
		xQueueSend(q_i2ctx, &pBuffer, portMAX_DELAY);
	}
}

//private funciton

static int extract_command(command_t* cmd)//把資料從queue裡面拿出來
{
	uint8_t item;
	BaseType_t status;
	status = uxQueueMessagesWaiting(q_uartrx);//回傳queue裡面還有多少data
	if(!status) return -1;
	uint8_t i=0;

	do
	{
		status = xQueueReceive(q_uartrx, &item, 0);
		if(status == pdTRUE) cmd->payload[i++] = item;
	}while(item!='0');
	cmd->payload[i-1] = '\0'; //replace the '0' with '\0'
	cmd->len = i-1;
	return  0;
}
