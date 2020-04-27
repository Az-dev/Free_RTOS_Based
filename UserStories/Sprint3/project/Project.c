/*
 * Sprint3.c
 *
 * Created: 4/21/2020 2:18:52 PM
 *  Author: Az
 */
 /*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
 #include "../../../MCAL/DIO/DIO.h"
 #include "../../../ECUAL/LEDS/led.h"
 #include "../../../ECUAL/KEYPAD/KeyPad.h"
 #include "../../../ECUAL/LCD/LCD.h"
 #include "../../../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
 #include "../../../MCAL/USART/usart.h"
 #include "../../../MCAL/USART/usart_Cfg.h"
 #define F_CPU 16000000UL
 #include <util/delay.h>

 #include "Project.h"
 #include "../../../RTOS_AVR_PORT/FreeRTOS.h"
 #include "../../../RTOS_AVR_PORT/task.h"
 #include "../../../RTOS_AVR_PORT/queue.h"
 #include "../../../RTOS_AVR_PORT/event_groups.h"
 #include "../../../RTOS_AVR_PORT/semphr.h"
 /* GLOBALS -----------------------------------------------------------------------------------------------------------------*/
 static uint8_t gau8_MessageToSend[MESSAGE_SIZE] = "12345679";
 static uint8_t gau8_RecievedMessage[MESSAGE_SIZE] = "4";
 static EventGroupHandle_t xEventGroupHandle = NULL;
 SemaphoreHandle_t xSyncTransmission = NULL;
 static uint8_t gu8_LCD_Flag = NO_LCD_ACTION;
 /*- FUNCTION DEFINITIONS ---------------------------------------------------------------------------------------------------*/
 /*---------- Call backs definitions ----------*/
 static void TxCallBack(void)
 {   
   /* on successful transmission : raise byte_transmission_complete event bit && byte to be transmitted is not null */
   xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)BYTE_SENT);     
 } 
  
 static void RxCallBack(void)
 {  
   /* on successful reception : raise byte_reception_complete event bit && byte to be received is not null */
   xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)BYTE_RECIEVED);
   /* forcing getting out from call back by forcing context switch with taskYIELD */
   taskYIELD();
   //return;   
 }
 /*--------- End call back definitions --------*/

 

 static void S3_projectInit()
 {
   /* Init USART */
   Usart_Init(&free_rtos_usart_config);
   USART_SetTxCallBack(TxCallBack);   
   USART_SetRxCallBack(RxCallBack);
   /* Init pushbtn*/
   PUSHBTN_Config();
   /* Init LCD */
   LCD_init();
   //LCD_clear();
   LCD_gotoRowColumn(1,1);   
   for(;;)
   {
      vTaskDelete(NULL);
   }
 }


 
  static void PrintMessage(void * param)
  {
      
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      TickType_t xDelay = pdMS_TO_TICKS(60);
      /* Initialize last wake time */
      xLastWakeTime = xTaskGetTickCount();      
      while(1)
      {
         /* switch message type (transmit or receive )*/
         switch(gu8_LCD_Flag)
         {
            //case PRINT_SENT_MSG:
               /* move to the upper row */               
               //LCD_gotoRowColumn(1,1);              
               /* print out the string on LCD*/
               //LCD_displayString(gau8_MessageToSend);
            //break;
            case PRINT_RECEIVED_MSG:               
               /* move to the lower low */
               LCD_gotoRowColumn(2,1);
               /* print out the string on LCD*/
               LCD_displayString((uint8_t *)(&gau8_RecievedMessage));
               /* xDelay = 2s */
               xDelay = pdMS_TO_TICKS_CUSTOM(2000);               
               /* Set event to clear */
               gu8_LCD_Flag = CLEAR_LCD;               
            break;
            case CLEAR_LCD:               
               //   case /* clear display_btn_pressed */:
               /* clear display_after_xDelay or display_btn_pressed flags */
               /* xDelay = 33ms */
               xDelay = pdMS_TO_TICKS(60);
               /* clear LCD */
               LCD_clear();
               /* Set LCD Flag to no action */
               gu8_LCD_Flag = NO_LCD_ACTION;
            break;
         }
         /* force moving task to block state after every wait iteration to switch to reception */
         vTaskDelayUntil( &xLastWakeTime, xDelay );
      }             
  }

  static void GetBtnState(void * param)
  {      
      uint8_t au8_btn1State = RELEASED;
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      const TickType_t xDelay = pdMS_TO_TICKS(10);
      /* Initialize last wake time */
      xLastWakeTime = xTaskGetTickCount();

      while(1)
      {
         /* Get PushBtn state */
         PUSHBTN_Get_BTN1_Status(&au8_btn1State);
         switch(au8_btn1State)
         {
            case PRESSED:
               /* raise send_message event bit */
               xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)SEND_MSG);
            break;
            case RELEASED:
               /* send task to block state */
               vTaskDelayUntil( &xLastWakeTime, xDelay );
            break;
         }    
      }
      
  }

  static void MessageTransiever(void * param)
  { 
      uint8_t au8_txBufferIndex = 0;
      uint8_t au8_rxBufferIndex = 0;
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      const TickType_t xDelay = pdMS_TO_TICKS(20);
      /* Initialize last wake time */
      xLastWakeTime = xTaskGetTickCount();
      EventBits_t au8_EventGroupVal = 0;         
      while(1)
      {
         /*1 - waits until send_message event bit is raised 
         if true write the next byte(from MessageToSendBuffer ) to UDR until you get to the last element in buffer and while send_byte_success*/
         au8_EventGroupVal = xEventGroupWaitBits(xEventGroupHandle,(const EventBits_t)(BYTE_SENT|SEND_MSG|BYTE_RECIEVED),pdTRUE,pdFALSE,pdTRUE);
         
         switch(au8_EventGroupVal)
         {
            case BYTE_RECIEVED:                              
               /*waits until byte_reception_complete event bit is raised 
               if true write the next byte(from MessageRecieved )from UDR + increment index */
               if((au8_rxBufferIndex < MESSAGE_SIZE))
               {
                  au8_rxBufferIndex++;
                  /* Read UDR */
                  UsartReadRx(&gau8_RecievedMessage[au8_rxBufferIndex]);
                  PORTC_DATA = gau8_RecievedMessage[au8_rxBufferIndex];
               }               
               else
               {
                  /* triggers display received msg event */
                  gu8_LCD_Flag = PRINT_RECEIVED_MSG;                  
                  /* Reset index */
                  au8_rxBufferIndex = 0;
               }                            
            break;
            case SEND_MSG:
               au8_txBufferIndex++;
               UsartWriteTx(&gau8_MessageToSend[au8_txBufferIndex]);
            break;
            case BYTE_SENT:                         
               /* write byte to UDR */
               if(('\0' != gau8_MessageToSend[au8_txBufferIndex]) || (MESSAGE_SIZE > au8_txBufferIndex))
               {
                  //_delay_ms(5);   /* a delay between the consequent write operations */
                  au8_txBufferIndex++;                  
                  UsartWriteTx(&gau8_MessageToSend[au8_txBufferIndex]);                  
               }
               else
               {
                  /* wait for next btn action */
                  //xEventGroupWaitBits(xEventGroupHandle,(const EventBits_t)SEND_MSG,pdTRUE,pdFALSE,portMAX_DELAY);
                  /*reset index */
                  au8_txBufferIndex = 0;
               }
            break;            
         }         
         /* force moving task to block state after every wait iteration to switch to reception */
         vTaskDelayUntil( &xLastWakeTime, xDelay );
      }         
  }

 void S3_project(void)
 {      
   /* Creating an event group */
   xEventGroupHandle = xEventGroupCreate();
   xSyncTransmission =  xSemaphoreCreateMutex();
   /* Check against successful queue creation */
   if((NULL != xEventGroupHandle) && (NULL != xSyncTransmission))
   {
      xTaskCreate( S3_projectInit, "Project Init", 100, NULL, 5, NULL );
      //xTaskCreate( PrintMessage, "Print Message", 100, NULL, 4, NULL );
      xTaskCreate( PrintMessage, "Print Message", 250, NULL, 4, NULL );
      xTaskCreate( MessageTransiever, "Message Transiever", 100, NULL, 3, NULL );      
      xTaskCreate( GetBtnState, "Get Btn State", 100, NULL, 2, NULL );
            
      //xTaskCreate( vSenderTask, "Sender1", 1000, ( void * ) 100, 1, NULL );
      //xTaskCreate( vSenderTask, "Sender2", 1000, ( void * ) 200, 1, NULL );
      /* Create the task that will read from the queue. The task is created with
      priority 2, so above the priority of the sender tasks. */
      //xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 2, NULL );
      /* Start the scheduler so the created tasks start executing. */
      vTaskStartScheduler();   
   }
   else
   {
      /* Failed to create event group */
   }
   while(1){}
 
 } 

  
