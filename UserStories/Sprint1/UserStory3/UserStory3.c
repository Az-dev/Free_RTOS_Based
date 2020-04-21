/*
 * UserStory3.c
 *
 * Created: 4/21/2020 12:40:03 PM
 *  Author: Az
 */ 
 /*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
 #include "../../../MCAL/DIO/DIO.h"
 #include "../../../ECUAL/LEDS/led.h"
 #include "../../../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
 #define F_CPU 16000000UL
 #include <util/delay.h>

 #include "UserStory3.h" 
 #include "../../../RTOS_AVR_PORT/FreeRTOS.h"
 #include "../../../RTOS_AVR_PORT/task.h"
 /*- GLOBALS ------------------------------------------------------------------------------------------------------------*/
 /* User Story 3 related variables */
 static uint8_t gu8_btn0State = RELEASED;
 static uint8_t gu8_durationCounter = 0;   /* represents duration counter (in this case it counts in second)*/
 static uint8_t gu8_ledAction = SET_LED_OFF;
 /*- FUNCTION DEFINITIONS ------------------------------------------------------------------------------------------------*/
 /*
 *  Description : User Story 3 Init task
 *
 *  @param void * param
 *
 *  @return void
 */
 static void UserStory3_Init(void * param)
 {
     
    /* 1- config PUSH BTN0 */
    PUSHBTN_Config();
    /* 2- config LED */
    Led_Init(LED_0);   
   for(;;)
   {
      vTaskDelete(NULL);
   }
 }


 /*
 * Description : updates push btn state.
 *
 * @param void 
 *
 * @return void 
 */ 
 static void updatePushBtnState(void * param)
 {
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(50);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   
   int iter = 0;   
   for(;;)
   {
      /* get btn0 status */
      PUSHBTN_Get_BTN0_Status(&gu8_btn0State);          
      /* while BTN0 is pressed */
      if(PRESSED == gu8_btn0State)
      {         
         if(iter >= 20)   /* 20 * 50 = 1000 ms*/
         {
            iter = 0;
            gu8_durationCounter++;           
         }         
         iter++;
         /* Set led action decision*/
         if(2 > gu8_durationCounter)
         {
            gu8_ledAction = SET_LED_OFF;
         }
         else if(( 2 <= gu8_durationCounter ) && ( 4 > gu8_durationCounter ))
         {
            gu8_ledAction = xDELAY_400MS;
         }
         else
         {
            gu8_ledAction = xDELAY_100MS;
         }                                 
      }
      else
      {         
         /* Reset iterator */
         iter = 0;
         /* Reset counter */
         gu8_durationCounter = 0;
         /* if btn0 is released : move the task to block state */
         vTaskDelayUntil( &xLastWakeTime, xDelay );                      
      }                  
   }
 }



 /*
 * Description : a Task that is responsible for change toggling rate of Leds.
 *
 * @param void 
 *
 * @return void
 */
 static void controlToggleRate(void * params)
 {
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms : initialize rate with 200 ms*/
   TickType_t xDelay = pdMS_TO_TICKS( 100 );
   /* Update last wake time */
   xLastWakeTime = xTaskGetTickCount();   
   for(;;)
   {
      if(RELEASED == gu8_btn0State)
      {
         switch(gu8_ledAction)
         {
            case SET_LED_OFF:
               /* turn led off */
               Led_Off(LED_0);
            break;
            case xDELAY_400MS:
               /* update xDelay to 400ms*/
               xDelay = pdMS_TO_TICKS(400);
               /* toggle led*/
               Led_Toggle(LED_0);
            break;
            case xDELAY_100MS:
               /* update xDelay to 100ms*/
               xDelay = pdMS_TO_TICKS(100);
               /* toggle led*/
               Led_Toggle(LED_0);
            break;
         }                 
      }
      /* block for xDelay ms*/
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }   
 }

 void S1_UserStory3(void)
 {
   /* Create Tasks */
   xTaskCreate(UserStory3_Init,"INIT",100,NULL,3,NULL);
   xTaskCreate(controlToggleRate,"control toggle",100,NULL,2,NULL);
   xTaskCreate(updatePushBtnState,"get btn0 state",100,NULL,1,NULL);
   
   /* Start Scheduler */
   vTaskStartScheduler();
   while(1)
   {

   }
 }