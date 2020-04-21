/*
 * UserStory2.c
 *
 * Created: 4/21/2020 12:39:49 PM
 *  Author: Az
 */ 
 /*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
 #include "../../../MCAL/DIO/DIO.h"
 #include "../../../ECUAL/LEDS/led.h"
 #include "../../../ECUAL/KEYPAD/KeyPad.h"
 #include "../../../ECUAL/LCD/LCD.h"
 #include "../../../ECUAL/PUSHBTN_SOS/PUSHBTN_SOS.h"
 #define F_CPU 16000000UL
 #include <util/delay.h>

 #include "UserStory2.h" 
 #include "../../../RTOS_AVR_PORT/FreeRTOS.h"
 #include "../../../RTOS_AVR_PORT/task.h"
 /*- GLOBALS -----------------------------------------------------------------------------------------------*/
 static uint8_t Key_Hold = FALSE;
 static uint8_t Pass_Match_Arr[6] = {'1', '2', '3', '6', '5', '4'};
 static uint8_t Pass_In_Arr[6] = {ZERO};
 /*- FUNCTION DEFINITIONS ---------------------------------------------------------------------------------------------------*/
 /*--------------------------------------  User Story 2 Related Tasks -------------------------------------------------*/
 /*
 *  Description : User Story 2 Init task
 *
 *  @param void * param
 *
 *  @return void
 */
 static void UserStory2_Init(void * param)
 {     
    /* 1- Config & Init LCD */
    LCD_init();
    LCD_gotoRowColumn(1,1);    
    /* 2- Config Keypad*/
    Keypad_Init();
    //Led_Init(LED_0);
   for(;;)
   {
      vTaskDelete(NULL);
   }
 }
 
 /*
 * Description : Send and output on LCD based on keypad input
 *
 * @param void * param
 *
 * @return void 
 */
 static void LCD_Output(void * param)
 {
   uint8_t * p = (uint8_t *) param;   
   /* define digits counter */
   uint8_t au8_digitCounter = 0;
   
   TickType_t xLastWakeTime;
   const TickType_t xDelay = pdMS_TO_TICKS(10);
   /* Update last wake time */
   xLastWakeTime = xTaskGetTickCount();
   
   for(;;)
   {
      if((au8_digitCounter < 6) && (*p != NO_ACTION) && (!Key_Hold))
      {
		 Key_Hold = TRUE;
         switch(*p)
         {
            case ENTER:
            case ERASE:
               /* Clear LCD  */
               LCD_clear();
               /* Send cursor home */
               LCD_gotoRowColumn(1,1);
               /* reset number of digits */
               au8_digitCounter = 0;
            break;
            default: ; /* digits from (0-6) */
            /* Store into pass_match_arr */
			Pass_In_Arr[au8_digitCounter] = *p;
            /* print_lcd(0-6)*/
            LCD_displayChar(*p);
            /* Increment number of digits */
            au8_digitCounter++;
         }
      }
      else if((au8_digitCounter == 6) && (*p != NO_ACTION) && (!Key_Hold))
      {
		 Key_Hold = TRUE;
         switch(*p)
         {
            case ENTER:
			{
               /* compare with pre-defied password */
			   uint8_t it = 0, pass_state = TRUE;
			   for(; it<6; it++)
			   {
				   if(Pass_In_Arr[it] != Pass_Match_Arr[it])
				   {
					   pass_state = FALSE;
				   }
			   }
               /* print message success or fail for two seconds */     
			   if(pass_state == TRUE)
			   {
				   LCD_gotoRowColumn(2,1);
				   LCD_displayString("Success!");
			   }
			   else
			   {
				   LCD_gotoRowColumn(2,1);
				   LCD_displayString("Wrong Password!");
			   }
			   _delay_ms(2000); // for 2s only LOL
               break;
			}
            default:
			{  
				/* Any case other than enter */
                /* reset number of digits */
                au8_digitCounter = 0;  
			}
         }
         /* lcd_send_command(clear) */
         LCD_clear();
         /* lcd_send_command(cursor_home) */
         LCD_gotoRowColumn(1,1);
      }      
      vTaskDelayUntil( &xLastWakeTime, xDelay );  
   }
 } 

 /*
 * Description : Receives input from keypad.
 *
 * @param void * param
 *
 * @return void 
 */
 static void KeyScanning(void * param)
 {
   uint8_t * p = (uint8_t *) param;
   uint8_t key_states[KEYS_NUM] = {ZERO};
   uint8_t pressed_flag = 0;
   /* Define Last wake time */
   TickType_t xLastWakeTime;
   /* Time delay value in ms*/
   const TickType_t xDelay = pdMS_TO_TICKS(2);
   /* Update last wake time */
   xLastWakeTime = xTaskGetTickCount();
   for(;;)
   {
      Keypad_Scan(key_states);
      uint8_t au8_iter = 0;
      for(;au8_iter < KEYS_NUM; au8_iter++)
      {
         /* check which key pressed or not_pressed */
         if(KEY_PRESSED == key_states[au8_iter])
         {
            pressed_flag = 1;
            /* report Number of key pressed */
            *p = au8_iter + 48;        
         }  
      }
      if(pressed_flag == 0)
      {
		  *p = NO_ACTION;
		  Key_Hold = FALSE;
      }
	  pressed_flag = 0;
	  /* block for 2 ms*/
	  vTaskDelayUntil( &xLastWakeTime, xDelay);     
   }
   
 }



 void S1_UserStory2(void)
 {
   /* Define & Initialize Keypad Pressed Btn value*/
   uint8_t au8_keypadVal = ERASE;
   /* Create Tasks */
   xTaskCreate(UserStory2_Init,"INIT",500,NULL,3,NULL);
   xTaskCreate(LCD_Output,"LCD_Output",100,&au8_keypadVal,2,NULL);
   xTaskCreate(KeyScanning,"KeyScanning",200,&au8_keypadVal,1,NULL);
   /* Start Scheduler */
   vTaskStartScheduler();
   while(1)
   {

   }
 }
