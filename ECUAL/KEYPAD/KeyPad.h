/*
 * KeyPad.h
 *
 * Created: 3/11/2020 11:20:48 AM
 *  Author: Khaled Magdy
 */ 


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "../../std_types.h"
//#include "../util/error_codes.h"

#define KEY_PRESSED	 1
#define KEY_RELEASED 0
#define KEY_FLOATING 2

#define KEYS_NUM     9
#define ROWS	  	 3
#define COLS		 3

#define FILTER_ORDER 8

/**=========================================================================*
 *							Keypad Functions Prototypes						*
 *==========================================================================*/


/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the keypad GPIO pins
 * 							
 */
ERROR_STATUS Keypad_Init(void);
/**
 * Input: 
 * Output:
 * In/Out:	Keys_Status Array Representing The Status OF Each Key in the Keypad
 * Return: The error status of the function.			
 * Description: Scans All The Keys in The Keypad, Applies An FIR Digital Filter To The Key Status. 
 * 							
 */
ERROR_STATUS Keypad_Scan(uint8_t* a_Keys_Status);


#endif /* KEYPAD_H_ */