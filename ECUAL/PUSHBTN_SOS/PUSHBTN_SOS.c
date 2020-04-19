/*
 * PUSHBTN_SOS.c
 *
 * Created: 3/16/2020 3:54:28 PM
 *  Author: Az
 */

 /*- INCLUDES -------------------------------------------------------------------------------------------------------*/
 #include "PUSHBTN_SOS.h"
 #include "../../registers.h"
 #include "../../common_macros.h"
 #include "../../std_types.h"
 #define F_CPU 16000000UL
 #include <util/delay.h>
 /*- FUNCTION DEFINITIONS -------------------------------------------------------------------------------------------*/
 /*
 * Description : Configures Push button.
 *
 * @param void 
 *
 * @return void
 */
 void PUSHBTN_Config(void)
 {
   PORTC_DIR &= ~(0x10); /* BTN0 */
   //PORTB_DIR &= ~(0x04); /* BTN1 */   
 }

 /*
 * Description : Gets button 0 state.
 *
 * @param uint8_t * state 
 *
 * @return void
 */
 void PUSHBTN_Get_BTN0_Status(uint8_t * state)
 {
   /* denouncing delay*/
   _delay_ms(50);
   /* Check if is Set or not */
   if(BIT_IS_SET(PORTC_PIN,4))
   {
      
      *state = PRESSED;
   }
   else
   {
      *state = RELEASED;
   }
 }

 /*
 * Description : Gets button 1 state.
 *
 * @param uint8_t * state 
 *
 * @return void
 */
 void PUSHBTN_Get_BTN1_Status(uint8_t * state)
 {
    /* Check if is Set or not */
    if(BIT_IS_SET(PORTB_PIN,2))
    {
       *state = PRESSED;
    }
    else
    {
       *state = RELEASED;
    }
 }


 
 
  
