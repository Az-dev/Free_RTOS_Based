/*
 * Test.c
 *
 * Created: 2/18/2020 9:30:37 AM
 *  Author: Az
 */ 
/*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "../../../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
#include "../../../ECUAL/LCD/LCD.h"
#include "../../../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
#include "../../../ECUAL/LEDS/led.h"
#include "UserStory2.h"
#include "../../../RTOS_AVR_PORT/FreeRTOS.h"
#include "../../../RTOS_AVR_PORT/task.h"
#include "../../../RTOS_AVR_PORT/semphr.h"
/*- GLOBALS -------------------------------------------------------------------------------------------------------------*/
static uint8_t gu8_displayFlag = CLEAR_MESSAGE;
static uint8_t gu8_messageHolder[50]={0};
static uint8_t gu8_btn0State = RELEASED;
static uint8_t gu8_btn1State = RELEASED;
static uint8_t gu8_ledState = LED_STATE_OFF;
static uint8_t gu8_btnInAction = NO_BTN_IN_ACTION; 
SemaphoreHandle_t xMutex = NULL;
/*- FUNCTION DEFINITIONS ------------------------------------------------------------------------------------------------*/
/*---- Static xTasks ----*/
/*
*  Description : Represents UserStory1 Init.
*
*  @param void * param
*
*  @return void
*/
void S2_Init(void * param)
{
   /* 1 - Config & Init LCD */
   LCD_init();
   //LCD_clear();
   LCD_gotoRowColumn(1,1);
   /* 2 - Config Push BTN */
   PUSHBTN_Config();
   /* 3- Config led */
   Led_Init(LED_0);
   for(;;)
   {
      vTaskDelete(NULL);
   }
}

/*
*  Description :responsible for Reading btn0 state
*
*  @param void * param
*
*  @param void
*/
static void TaskA(void * param)
{
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(70);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();   
   while(1)
   {
      xSemaphoreTake(xMutex,portMAX_DELAY);
      {
         /* Get btn0 state */
         PUSHBTN_Get_BTN0_Status(&gu8_btn0State);
         if(PRESSED == gu8_btn0State) gu8_btnInAction = BTN0_IN_ACTION;
      }
      xSemaphoreGive(xMutex);           
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }
}

/*
*  Description :responsible for Reading btn1 state
*
*  @param void * param
*
*  @param void
*/
static void TaskB(void * param)
{
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(75);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   while(1)
   {
      xSemaphoreTake(xMutex,portMAX_DELAY);
      {
         /* Get btn0 state */
         PUSHBTN_Get_BTN1_Status(&gu8_btn1State);
         if(PRESSED == gu8_btn1State) gu8_btnInAction = BTN1_IN_ACTION;
      }      
      xSemaphoreGive(xMutex);
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }
}

/*
*  Description : responsible for toggling led
*  
*  @param void
*
*  @return void
*/
static void TaskC(void)
{
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(80);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   while(1)
   {
      if((PRESSED == gu8_btn1State) || (PRESSED == gu8_btn0State))
      {
         /* update state with : led_on */
         gu8_ledState = LED_STATE_ON;
         /* turn led on */
         Led_On(LED_0);    
      }
      else
      {
         /* update state with : led_off*/
         gu8_ledState = LED_STATE_OFF;
         /* turn led off */
         Led_Off(LED_0);
    
      }      
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }
}

/*
*  Description : Monitor LED ,BTN states and print them on LCD
*
*  @param void
*
*  @return void
*/
static void TaskD(void)
{
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(100);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   while(1)
   {
      
      /* check led state */
      if(LED_STATE_ON == gu8_ledState)
      {
         /* Send cursor at the second row */
         LCD_gotoRowColumn(1,1);
         /* print message */
         LCD_displayString("LED :  ON");   
      }
      else
      {
         /* Send cursor at the second row */
         LCD_gotoRowColumn(1,1);
         /* print message */
         LCD_displayString("LED : OFF");
      }
      /* check which btn */
      if(BTN0_IN_ACTION == gu8_btnInAction)
      {
         if(PRESSED == gu8_btn0State)
         {
            /* Send cursor at the second row */
            LCD_gotoRowColumn(2,1);
            /* print message */
            LCD_displayString("BTN0 : PRESSED");
         }
         else
         {
            /* Send cursor at the second row */
            LCD_gotoRowColumn(2,1);
            /* print message */
            LCD_displayString("BTN0 : RELEASED");
         }   
      }      
      if(BTN1_IN_ACTION == gu8_btnInAction)
      {
         if(PRESSED == gu8_btn1State)
         {
            /* Send cursor at the second row */
            LCD_gotoRowColumn(2,1);
            /* print message */
            LCD_displayString("BTN1 : PRESSED");
         }
         else
         {
            /* Send cursor at the second row */
            LCD_gotoRowColumn(2,1);
            /* print message */
            LCD_displayString("BTN1 : RELEASED");
         }    
      }
      /* block for 200ms */
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }
}


/*------- User Stories --------*/

void S2_UserStory2(void)
{
   /*create mutex */
   xMutex = xSemaphoreCreateMutex();
   if(NULL != xMutex)
   {
      /* create Taska*/
      xTaskCreate(S2_Init,"S2_Init",100,NULL,4,NULL);
      xTaskCreate(TaskA,"TaskA",100,NULL,1,NULL);
      xTaskCreate(TaskB,"TaskB",100,NULL,1,NULL);
      xTaskCreate(TaskC,"TaskC",100,NULL,2,NULL);
      xTaskCreate(TaskD,"TaskD",100,NULL,3,NULL);
      /* Start Scheduler */
      vTaskStartScheduler();      
   }
   while(1)
   {

   }
}






