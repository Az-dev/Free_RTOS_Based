/*
 * led.c
 *
 * Created: 1/17/2020 5:16:12 PM
 *  Author: Khaled Magdy
 */ 
#include "LED.h"


void Led_Init(En_LedNumber_t en_led_id)
{
	strDioCfg_t LED_cfg;
   switch(en_led_id) 
   {
      case LED_0:
					LED_cfg.GPIO = LED_0_GPIO;
					LED_cfg.pins = LED_0_BIT;
					LED_cfg.dir = OUTPUT;
                    DIO_init(&LED_cfg);
                    break;
      case LED_1:
                    LED_cfg.GPIO = LED_1_GPIO;
                    LED_cfg.pins = LED_1_BIT;
                    LED_cfg.dir = OUTPUT;
                    DIO_init(&LED_cfg);
                    break;
      case LED_2:
                    LED_cfg.GPIO = LED_2_GPIO;
                    LED_cfg.pins = LED_2_BIT;
                    LED_cfg.dir = OUTPUT;
                    DIO_init(&LED_cfg);
                    break;
      case LED_3:
                    LED_cfg.GPIO = LED_2_GPIO;
                    LED_cfg.pins = LED_2_BIT;
                    LED_cfg.dir = OUTPUT;
                    DIO_init(&LED_cfg);
                    break;
   }
}

void Led_On(En_LedNumber_t en_led_id)
{
   switch(en_led_id)
   {
      case LED_0:
                    DIO_Write(LED_0_GPIO, LED_0_BIT, HIGH);
                    break;
      case LED_1:
                    DIO_Write(LED_1_GPIO, LED_1_BIT, HIGH);
                    break;
      case LED_2:
                    DIO_Write(LED_2_GPIO, LED_2_BIT, HIGH);
                    break;
      case LED_3:
                    DIO_Write(LED_3_GPIO, LED_3_BIT, HIGH);
                    break;
      default:
                    break;
   }
}

void Led_Off(En_LedNumber_t en_led_id)
{
   switch(en_led_id)
   {
       case LED_0:
                    DIO_Write(LED_0_GPIO, LED_0_BIT, LOW);
                    break;
       case LED_1:
                    DIO_Write(LED_1_GPIO, LED_1_BIT, LOW);
                    break;
       case LED_2:
                    DIO_Write(LED_2_GPIO, LED_2_BIT, LOW);
                    break;
       case LED_3:
                    DIO_Write(LED_3_GPIO, LED_3_BIT, LOW);
                    break;
       default:
                    break;
   }
}

void Led_Toggle(En_LedNumber_t en_led_id)
{
   switch(en_led_id)
   {
       case LED_0:
                    DIO_Toggle(LED_0_GPIO, LED_0_BIT);
					break;
	   case LED_1:
	                DIO_Toggle(LED_1_GPIO, LED_1_BIT);
	                break;
	   case LED_2:
		            DIO_Toggle(LED_2_GPIO, LED_2_BIT);
		            break;
	   case LED_3:
		            DIO_Toggle(LED_3_GPIO, LED_3_BIT);
	      	        break;
	   default:
	                break;
   }
}