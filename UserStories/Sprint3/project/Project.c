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
 static uint8_t gau8_MessageToSend[16] = "hellow there";
 static uint8_t gau8_RecievedMessage[16]={0};
 static EventGroupHandle_t xEventGroupHandle = NULL;
 SemaphoreHandle_t xSyncTransmission = NULL;
 /*- FUNCTION DEFINITIONS ---------------------------------------------------------------------------------------------------*/
 /*---------- Call backs definitions ----------*/
 static void TxCallBack(void)
 {   
   /* on successful transmission : raise byte_transmission_complete event bit && byte to be transmitted is not null */
   EventBits_t x = xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)BYTE_SENT);  
 }

 static void UDER_CallBack(void)
 {
    /* UDR is ready */
    EventBits_t x = xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)UDR_READY);
 }
  
 static void RxCallBack(void)
 {  
   /* on successful reception : raise byte_reception_complete event bit && byte to be received is not null */
   EventBits_t x = xEventGroupSetBits(xEventGroupHandle,(const EventBits_t)BYTE_RECIEVED);   
 }
 /*--------- End call back definitions --------*/

 

 static void S3_projectInit()
 {
   /* Init USART */
   Usart_Init(&free_rtos_usart_config);
   USART_SetTxCallBack(TxCallBack);
   //USART_Set_UDRE_CallBack(UDER_CallBack);
   USART_SetRxCallBack(RxCallBack);
   /* Init pushbtn*/
   PUSHBTN_Config();   
   for(;;)
   {
      vTaskDelete(NULL);
   }
 }


 
  static void PrintMessage(void * param)
  {
      /* switch transmission state (transmit or receive )*/
      //switch(/*transmission type*/)
      //{
      //   case /*Transmit*/:            
            /* move to the upper row */
            /* print out the string on LCD*/
      //   break;
      //   case /*Received*/:            
            /* move to the lower low */
            /* print out the string on LCD*/
            /* xDelay = 2s */
      //   break;
      //   case /* clear display_after_xDelay*/:
      //   case /* clear display_btn_pressed */:
            /* clear display_after_xDelay or display_btn_pressed flags */
            /* clear LCD */
      //   break;
      //}
  }

  static void GetBtnState(void * param)
  {      
      uint8_t au8_btn1State = RELEASED;
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      const TickType_t xDelay = pdMS_TO_TICKS(20);
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
      uint8_t index = 0;
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
         PORTC_DATA = au8_EventGroupVal;
         switch(au8_EventGroupVal)
         {
            case BYTE_RECIEVED:
               /*waits until byte_reception_complete event bit is raised 
               if true write the next byte(from MessageRecieved )from UDR + increment index */               
               /* Read UDR */               
               UsartReadRx(&gau8_RecievedMessage[index]); 
               //ResetUDR();              
            break;
            case BYTE_SENT:   
            case SEND_MSG:
               /* write byte to UDR */
               if('\0' != gau8_MessageToSend[index])
               {
                  //_delay_ms(5);   /* a delay between the consequent write operations */
                  index++;                  
                  UsartWriteTx(&gau8_MessageToSend[index]);
               }
               else
               {
                  /* wait for next btn action */
                  //xEventGroupWaitBits(xEventGroupHandle,(const EventBits_t)SEND_MSG,pdTRUE,pdFALSE,portMAX_DELAY);
                  /*reset index */
                  index = 0;
               }
            break;            
         }         
         /* force moving task to block state after every wait iteration to switch to reception */
         vTaskDelayUntil( &xLastWakeTime, xDelay );
      }         
  }
  
  static void MessageReciever(void * param)
  {
      /* Define Last wake time */
      TickType_t xLastWakeTime;
      /* Time delay value in ms*/
      const TickType_t xDelay = pdMS_TO_TICKS(10);
      /* Initialize last wake time */
      xLastWakeTime = xTaskGetTickCount();

      while(1)
      {
         
         
         
            
         
                       
         
         /* force moving task to block state after every wait iteration to switch to sending */
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
      xTaskCreate( MessageTransiever, "Message Transiever", 100, NULL, 4, NULL );
      //xTaskCreate( MessageReciever, "Message Reciever", 100, NULL, 4, NULL );
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

  
