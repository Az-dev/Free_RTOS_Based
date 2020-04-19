/*
 * DIO.c
 *
 * Created: 2/17/2020 8:36:48 PM
 *  Author: Az
 */ 
/*- INCLUDES --------------------------------------------------------------------------------------------------------------*/
#include "DIO.h"
/*- FUNCTIONS DEFINITIONS -------------------------------------------------------------------------------------------------*/
/*
*Input: strDioCfg_t -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_init (strDioCfg_t * DIO_info)
{
   if(DIO_info != NULL)
   {
      /* Set pins direction */
      /*check the direction whether input or output*/
      if((DIO_info->dir) == INPUT)
      {
         /*direction is input*/         
         switch(DIO_info->GPIO)
         {
            case 0: /* GPIOA */
               PORTA_DIR &= ~(DIO_info->pins);
            break;
            case 1: /* GPIOB */
               PORTB_DIR &= ~(DIO_info->pins);
            break;
            case 2: /* GPIOC */
               PORTC_DIR &= ~(DIO_info->pins);
            break;
            case 3: /* GPIOD */
               PORTD_DIR &= ~(DIO_info->pins);
            break;
         }
      }
      else 
      {
         /* Direction is OUTPUT */
         switch(DIO_info->GPIO)
         {
            case 0: /* GPIOA */
            PORTA_DIR |= DIO_info->pins;
            break;
            case 1: /* GPIOB */
            PORTB_DIR |= DIO_info->pins;
            break;
            case 2: /* GPIOC */
            PORTC_DIR |= DIO_info->pins;
            break;
            case 3: /* GPIOD */
            PORTD_DIR |= DIO_info->pins;
            break;
         }
      }
      return E_OK;      
   }
   else
   {
      return E_NOK;      
   }
}


/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		value 	-> The desired value
*					- HIGH
*					- LOW
*Output: No output
*In/Out: No In/Out
*Description: This function can set the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value)
{
   if(value == LOW)
   {
      switch(GPIO)
      {
         case 0: /* GPIOA */
            PORTA_DATA &= (~pins);		/* clear pins*/
         break;
         case 1: /* GPIOB */
            PORTB_DATA &= (~pins);		/* clear pins*/
         break;
         case 2: /* GPIOC */
            PORTC_DATA &= (~pins);		/* clear pins*/
         break;
         case 3: /* GPIOD */
            PORTD_DATA &= (~pins);		/* clear pins*/
         break;
      }
   }
   else
   {
      switch(GPIO)
      {
         case 0: /* GPIOA */
            PORTA_DATA |= pins;		/* set pins*/
         break;
         case 1: /* GPIOB */
            PORTB_DATA |= pins;		/* set pins*/
         break;
         case 2: /* GPIOC */
            PORTC_DATA |= pins;		/* set pins*/
         break;
         case 3: /* GPIOD */
            PORTD_DATA |= pins;		/* set pins*/
         break;
      }
   }
   return E_OK;
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*		
*Output: data -> the acquired data wether it was PORT data or pins data
*In/Out:
*Description: This function gets the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data)
{
   if(data != NULL)
   {
      switch(GPIO)
      {
         case 0: /* GPIOA */
         (*data) = PORTA_PIN & pins ;		/* set pins*/
         break;
         case 1: /* GPIOB */
         (*data) = PORTB_PIN & pins;		/* set pins*/
         break;
         case 2: /* GPIOC */
         (*data) = PORTC_PIN & pins;		/* set pins*/
         break;
         case 3: /* GPIOD */
         (*data) = PORTD_PIN & pins;		/* set pins*/
         break;
      }      
      return E_OK; 
   }
   else
   {
      return E_NOK;
   }
}

/*
*Input: GPIO -> to get PORT name
*					- GPIOA
*					- GPIOB
*					- GPIOC
*					- GPIOD
*		pins -> pins to be written at.
*					- PIN0
*					- PIN1
*					- PIN2
*					- PIN3
*					- PIN4
*					- PIN5
*					- PIN6
*					- PIN7
*					- UPPER_NIBBLE
*					- LOWER_NIBBLE
*					- FULL_PORT
*Output: data -> No output
*In/Out:
*Description: This function toggles the value of a full port, a nibble
* 			  or even one pin.
*/
ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins)
{
   switch(GPIO)
   {
      case 0: /* GPIOA */
         PORTA_DATA ^= pins;		/* toggle pins values*/
      break;
      case 1: /* GPIOB */
         PORTB_DATA ^= pins;		/* toggle pins values*/
      break;
      case 2: /* GPIOC */
         PORTC_DATA ^= pins;		/* toggle pins values*/
      break;
      case 3: /* GPIOD */
         PORTD_DATA ^= pins;		/* toggle pins values*/
      break;
   }
   return E_OK;
}
