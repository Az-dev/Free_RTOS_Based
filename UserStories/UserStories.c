/*
 * Test.c
 *
 * Created: 2/18/2020 9:30:37 AM
 *  Author: Az
 */ 
/*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
#include <string.h>
#include "../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
#include "../ECUAL/LCD/LCD.h"
#include "../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
#include "UserStories.h"
#include "../RTOS_AVR_PORT/FreeRTOS.h"
#include "../RTOS_AVR_PORT/task.h"
#include "../RTOS_AVR_PORT/semphr.h"
/*- GLOBALS -------------------------------------------------------------------------------------------------------------*/
static uint8_t gu8_displayFlag = CLEAR_MESSAGE;
static uint8_t gu8_messageHolder[50]={0};
static uint8_t gu8_btn0State = RELEASED; 
/*- FUNCTION DEFINITIONS ------------------------------------------------------------------------------------------------*/
/*-----  Start Idle Task ------*/
void vApplicationIdleHook( void )
{
   /* Do Nothing */
   //taskYIELD();
}
/*----- End Idle Task -------*/
/*---- Static xTasks ----*/
/*
*  Description : Represents UserStory1 Init.
*
*  @param void * param
*
*  @return void
*/
void S1_Init(void * param)
{
   /* 1 - Init PushBTN0 */
   PUSHBTN_Config();
   /* 2 - Config & Init LCD */
   LCD_init();
   //LCD_clear();
   LCD_gotoRowColumn(1,1);
   /* 3 - Config Push BTN */
   PUSHBTN_Config();
   for(;;)
   {
      vTaskDelete(NULL);
   }
}

/*
*  Description :responsible for outputting data on LCD
*
*  @param void
*
*  @param void 
*/
static void TaskA(void * param)
{  
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(200);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   while(1)
   {
      if(DISPLAY_MESSAGE == gu8_displayFlag)
      {
         /* Pull down Display Flag */
         gu8_displayFlag = CLEAR_MESSAGE;
         if(PRESSED == gu8_btn0State)
         {
            /* Send cursor at the second row */
            LCD_gotoRowColumn(2,1);       
         }
         else
         {
            /* Send cursor home */
            LCD_gotoRowColumn(1,1);
         }         
         /* print message */
         LCD_displayString((uint8_t *)(&gu8_messageHolder));         
      }
      else
      {
         /* Clear LCD  */
         LCD_clear();
      }
      /* Block for 200ms*/
      vTaskDelayUntil( &xLastWakeTime, xDelay );           
   }   
}

/*
*  Description :responsible for Reading btn state
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
   const TickType_t xDelay = pdMS_TO_TICKS(70);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();   
   while(1)
   {
      /* Get btn0 state */
      PUSHBTN_Get_BTN0_Status(&gu8_btn0State);     
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }
}

/*
*  Description :Sending data to lCD
*
*  @param void * param
*
*  @param void
*/
static void TaskC(void * param)
{
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(400);
   /* Initialize last wake time */
   xLastWakeTime = xTaskGetTickCount();
   while(1)
   {
      /* Pull_Up Display flag */
      gu8_displayFlag = DISPLAY_MESSAGE;
      /* write message */
      if(PRESSED == gu8_btn0State)
      {
         strcpy((char *)(&gu8_messageHolder),"I have been over-written !!!!!!!!!");         
      }
      else
      {
         strcpy((char *)(&gu8_messageHolder),"Hello LCD !!");
      }           
      /* Block for 400ms*/
      vTaskDelayUntil( &xLastWakeTime, xDelay );
   }   
}
/*------- User Stories --------*/

void UserStory1(void)
{
   /* create Task A*/
   xTaskCreate(S1_Init,"S1_Init",100,NULL,4,NULL);
   xTaskCreate(TaskA,"TaskA",100,NULL,2,NULL);
   xTaskCreate(TaskB,"TaskB",100,NULL,1,NULL);
   xTaskCreate(TaskC,"TaskC",100,NULL,3,NULL);
   /* Start Scheduler */
   vTaskStartScheduler();
   while(1)
   {

   }
}






