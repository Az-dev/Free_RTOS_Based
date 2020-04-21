/*
 * UserStory1.c
 *
 * Created: 4/21/2020 12:39:12 PM
 *  Author: Az
 */ 
 /*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
 #include "../../../MCAL/DIO/DIO.h"
 #include "../../../ECUAL/LEDS/led.h" 
 #define F_CPU 16000000UL
 #include <util/delay.h>

 #include "UserStory1.h" 
 #include "../../../RTOS_AVR_PORT/FreeRTOS.h"
 #include "../../../RTOS_AVR_PORT/task.h"
 /*- FUNCTION DEFINITIONS ------------------------------------------------------------------------------------------------*/
 /*--------------------------------------  User Story 1 Related Tasks -------------------------------------------------*/   
/*
*  Description : User Story 1 Init task 
*
*  @param void 
*
*  @return void 
*/
 static void UserStory1_Init(void * params)
 {
   Led_Init(LED_0);
   for(;;)
   {
      vTaskDelete(NULL);
   }   
 }

 /*
 * Description : a Task that is responsible for toggling Leds each (1000) ms.
 *
 * @param void 
 *
 * @return void
 */
 static void LedToggle(void * params)
 {
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS_CUSTOM( 1000 );
   /* Update last wake time */
   xLastWakeTime = xTaskGetTickCount();
   for(;;)
   {
	   Led_Toggle(LED_0);
       /* block for 1000 ms*/       
       vTaskDelayUntil( &xLastWakeTime, xDelay ); 
   }
   
 }



 void S1_UserStory1(void)
 {
   /* Create Tasks */
   xTaskCreate(UserStory1_Init,"INIT",100,NULL,1,NULL);
   xTaskCreate(LedToggle,"Led Toggle",100,NULL,1,NULL);
   /* Start Scheduler */
   vTaskStartScheduler();
   while(1)
   {

   }
 }
