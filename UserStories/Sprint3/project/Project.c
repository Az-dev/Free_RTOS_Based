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
 #include "../../../RTOS_AVR_PORT/timers.h"
 #include "../../../RTOS_AVR_PORT/portmacro.h"
 /* GLOBALS -----------------------------------------------------------------------------------------------------------------*/
 static EventGroupHandle_t xEventGroupHandle = NULL;
 SemaphoreHandle_t xSyncTransmission = NULL;
 static uint8_t gu8_LCD_Flag = NO_DISPLAY_ACTION;
 /* Define communication queues*/
 QueueHandle_t xQueueTx = NULL;
 QueueHandle_t xQueueRx = NULL;
 QueueHandle_t xQueueKeypadDisplay = NULL;
 /* Define terminating character */
 uint8_t gu8_terminatingChar = '\n';

 /*- FUNCTION DEFINITIONS ---------------------------------------------------------------------------------------------------*/
 /*---------- Call backs definitions ----------*/
 void setRxEvent( void * pvParameter1, uint32_t ulParameter2 )
 {
   BaseType_t xHigherPriorityTaskWoken, xResult;
   /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
   xHigherPriorityTaskWoken = pdFALSE;
   /* on successful reception : raise byte_reception_complete event bit && byte to be received is not null */
   xResult = xEventGroupSetBitsFromISR(xEventGroupHandle,(const EventBits_t)BYTE_RECIEVED,&xHigherPriorityTaskWoken);
   taskYIELD();
 }

 void setTxEvent( void * pvParameter1, uint32_t ulParameter2 )
 {
   BaseType_t xHigherPriorityTaskWoken, xResult;
   /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
   xHigherPriorityTaskWoken = pdFALSE;
   /* on successful transmission : raise byte_transmission_complete event bit && byte to be transmitted is not null */
   xResult = xEventGroupSetBitsFromISR(xEventGroupHandle,(const EventBits_t)BYTE_SENT,&xHigherPriorityTaskWoken);
   taskYIELD();
 }

 static void TxCallBack(void)
 {
   BaseType_t xHigherPriorityTaskWoken;
   xHigherPriorityTaskWoken = pdFALSE;
   xTimerPendFunctionCallFromISR( setTxEvent,NULL,NULL,&xHigherPriorityTaskWoken ); 
   taskYIELD();            
 } 
  
 static void RxCallBack(void)
 {
   BaseType_t xHigherPriorityTaskWoken;
   xHigherPriorityTaskWoken = pdFALSE;
   xTimerPendFunctionCallFromISR( setRxEvent,NULL,NULL,&xHigherPriorityTaskWoken );
   taskYIELD();       
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
   /* Init KeyPad */
   Keypad_Init();   
   for(;;)
   {
      vTaskDelete(NULL);
   }
 }   

  static void MessageTransiever(void * param)
  { 
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      const TickType_t xDelay = pdMS_TO_TICKS(45);
      /* Initialize last wake time */
      xLastWakeTime = xTaskGetTickCount();
      EventBits_t au8_EventGroupVal = 0;
      
      uint8_t au8_temp_Tx_Char = 0;
      uint8_t au8_temp_Rx_Char = 0;         
      while(1)
      {
         /*1 - waits until send_message event bit is raised 
         if true write the next byte(from MessageToSendBuffer ) to UDR until you get to the last element in buffer and while send_byte_success*/
         au8_EventGroupVal = xEventGroupWaitBits(xEventGroupHandle,(const EventBits_t)(BYTE_SENT|SEND_MSG|BYTE_RECIEVED),pdTRUE,pdFALSE,pdMS_TO_TICKS(10));         
         switch(au8_EventGroupVal)
         {
            case BYTE_RECIEVED:               
               /* Read UDR */
               UsartReadRx(&au8_temp_Rx_Char);
               /* Check end of transfer */
               if(gu8_terminatingChar != au8_temp_Rx_Char)
               {
                  /* store character in xQueueRx */
                  xQueueSendToBack(xQueueRx,&au8_temp_Rx_Char,pdMS_TO_TICKS(5));
               }
               else
               {
                  ResetUDR();
                  /* triggers display received msg event */
                  gu8_LCD_Flag = PRINT_RECEIVED_MSG;
               }                                          
            break;
            case SEND_MSG:           
            case BYTE_SENT:                         
               /* write byte to UDR from xQueueTx*/
               if(pdPASS == xQueueReceive(xQueueTx,&au8_temp_Tx_Char,pdMS_TO_TICKS(1)))
               {                                   
                  UsartWriteTx(&au8_temp_Tx_Char);                                    
               }                             
            break;            
         }         
         /* force moving task to block state after every wait iteration to switch to reception */
         vTaskDelayUntil( &xLastWakeTime, xDelay );
      }         
  }

  static void PrintMessage(void * param)
  {     
     /* Define Last wake time */
     TickType_t xLastWakeTime;
     /* Time delay value in ms*/
     TickType_t xDelay = pdMS_TO_TICKS(25);
     /* Initialize last wake time */
     xLastWakeTime = xTaskGetTickCount();
     /* keypad character/ received character to be displayed on LCD */
     uint8_t au8_charToDisplay = 0;
     /* col_index */
     uint8_t au8_colIndex = 1;
     while(1)
     {
        /* switch message type (transmit or receive )*/
        switch(gu8_LCD_Flag)
        {
           case PRINT_RECEIVED_MSG:
              /* move to the lower low */
              LCD_gotoRowColumn(2,1);
              /* print out the string on LCD : by reading each character from xQueueRx*/
              while(pdPASS == xQueueReceive(xQueueRx,&au8_charToDisplay,pdMS_TO_TICKS(1)))
              {
                 LCD_displayChar(au8_charToDisplay);
              }
              /* xDelay = 2s */
              xDelay = pdMS_TO_TICKS_CUSTOM(2000);
              /* Set event to clear */
              gu8_LCD_Flag = CLEAR_LCD;
           break;
           case CLEAR_LCD:
              //   case /* clear display_btn_pressed */:
              /* clear display_after_xDelay or display_btn_pressed flags */
              /* xDelay = 33ms */
              xDelay = pdMS_TO_TICKS(25);
              /* clear LCD */
              LCD_clear();
              /* Set LCD Flag to no action */
              gu8_LCD_Flag = NO_DISPLAY_ACTION;
              /* reset col index */
              au8_colIndex = 1;
           break;
           default:
              /* move to the upper row */
              LCD_gotoRowColumn(1,au8_colIndex);
              if(pdPASS == xQueueReceive(xQueueKeypadDisplay,&au8_charToDisplay,pdMS_TO_TICKS(1)))
              {
                 LCD_displayChar(au8_charToDisplay);
                 au8_colIndex++;
              }
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
     const TickType_t xDelay = pdMS_TO_TICKS(40);
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
              /* append terminating character to the data to be transmitted */
              xQueueSendToBack(xQueueTx,&gu8_terminatingChar,pdMS_TO_TICKS(2));
              /* clear the printed message on display */
              gu8_LCD_Flag = CLEAR_LCD;
           break;
           case RELEASED:
              /* send task to block state */
              //vTaskDelayUntil( &xLastWakeTime, xDelay );
           break;
        }
        /* send task to block state */
        vTaskDelayUntil( &xLastWakeTime, xDelay );
     }
     
  }

  static void KeypadScanner(void * param)
  {
     uint8_t key_states[KEYS_NUM] = {ZERO};
     uint8_t au8_keyVal = 0;
     /* Define Last wake time */
     TickType_t xLastWakeTime;
     /* Time delay value in ms*/
     const TickType_t xDelay = pdMS_TO_TICKS(13);
     /* Initialize last wake time */
     xLastWakeTime = xTaskGetTickCount();
     /* scan times*/
     int au8_scanTimes = 0;
     /* key press flage*/
     uint8_t au8_pressedFlag = 0;
     while(1)
     { 
        /* scanning keypad for 2 times */
        for(au8_scanTimes=0; au8_scanTimes < 2; au8_scanTimes++)
        {
            Keypad_Scan(key_states);
            _delay_us(250);
        }
        /* iterating over scanned states */        
        uint8_t au8_iter = 0;
        for(;au8_iter < KEYS_NUM; au8_iter++)
        {
           /* check which key pressed or not_pressed */
           if(KEY_PRESSED == key_states[au8_iter])
           {
              au8_pressedFlag = 1;
              /* Evaluate key value */
              au8_keyVal = au8_iter + ASCII_OFFSET;              
           }
        }

        if(1 == au8_pressedFlag)
        {           
           /* store it in xQueueTx */
           xQueueSendToBack(xQueueTx,&au8_keyVal,pdMS_TO_TICKS(1));
           /* send it to be displayed whith in display queue*/
           xQueueSendToBack(xQueueKeypadDisplay,&au8_keyVal,pdMS_TO_TICKS(1));
        }
        au8_pressedFlag = 0;        
        /* force moving task to block state after every wait iteration to switch to reception */
        vTaskDelayUntil( &xLastWakeTime, xDelay );
     }
  }

 void S3_project(void)
 {      
   /* Creating an event group */
   xEventGroupHandle = xEventGroupCreate();
   xSyncTransmission =  xSemaphoreCreateMutex();
   /* Create queues */
   xQueueTx = xQueueCreate( 16, sizeof( uint8_t ) );
   xQueueRx = xQueueCreate( 16, sizeof( uint8_t ) );
   xQueueKeypadDisplay = xQueueCreate( 1, sizeof( uint8_t ) );   
   /* Check against successful queue creation */
   if((NULL != xEventGroupHandle) && (NULL != xSyncTransmission) && (NULL != xQueueTx) && (NULL != xQueueRx) && (NULL != xQueueKeypadDisplay))
   {
      xTaskCreate( S3_projectInit, "Project Init", 100, NULL, 5, NULL );
      xTaskCreate( MessageTransiever, "Message Transiever", 100, NULL, 2, NULL );      
      xTaskCreate( PrintMessage, "Print Message", 200, NULL, 3, NULL );
      xTaskCreate( GetBtnState, "Get Btn State", 100, NULL, 1, NULL );      
      xTaskCreate( KeypadScanner, "Keypad Scanner", 100, NULL, 4, NULL );          
      vTaskStartScheduler();   
   }
   else
   {
      /* Failed to create event group */
   }
   while(1){}
 
 } 

  
