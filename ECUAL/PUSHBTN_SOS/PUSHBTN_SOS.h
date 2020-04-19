/*
 * PUSHBTN_SOS.h
 *
 * Created: 3/16/2020 3:54:04 PM
 *  Author: Az
 */
#ifndef PUSHBTN_SOS_H_
#define PUSHBTN_SOS_H_
/*- INCLUDES --------------------------------------------------------------------------------------------------------*/
#include "../../registers.h"
#include "../../common_macros.h"
#include "../../std_types.h"
/*- DEFINES ---------------------------------------------------------------------------------------------------------*/
#define RELEASED 0
#define PRESSED 1
/*- FUNCTION DECLARATIONS --------------------------------------------------------------------------------------------*/
/*
 * Description : Configures Push button.
 *
 * @param void 
 *
 * @return void
 */
void PUSHBTN_Config(void);

/*
 * Description : Gets button 0 state.
 *
 * @param uint8_t * state 
 *
 * @return void
 */
void PUSHBTN_Get_BTN0_Status(uint8_t * state);

 /*
 * Description : Gets button 1 state.
 *
 * @param uint8_t * state 
 *
 * @return void
 */
void PUSHBTN_Get_BTN1_Status(uint8_t * state);

#endif /* PUSHBTN_SOS_H_ */