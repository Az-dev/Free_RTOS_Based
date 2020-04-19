/*
 * LCD.h
 *
 * Created: 3/11/2020 11:20:07 AM
 *  Author: Khaled Magdy
 */ 


#ifndef LCD_H_
#define LCD_H_

#include "../../std_types.h"
//#include "../util/error_codes.h"



/*
 * Input: 
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Initializes The Alphanumeric LCD 16x2 Module
 * 							
 */
ERROR_STATUS LCD_init (void);
/*
 * Input: 
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Clears The LCD Display Screen
 * 							
 */
ERROR_STATUS LCD_clear(void);
/*
 * Input:  An 8-Bit Command To The LCD
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Sends A Command To The LCD Module
 * 							
 */
ERROR_STATUS LCD_sendCommand(uint8_t a_CMD);
/*
 * Input:  Character To Be Displayed On The LCD
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Sends A Character To The LCD Display @ The Current Cursor Position
 * 							
 */
ERROR_STATUS LCD_displayChar(uint8_t a_CH);
/*
 * Input:  String OF Characters
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Sends The Characters One By One To The LCD Display
 * 							
 */
ERROR_STATUS LCD_displayString(uint8_t* a_STR);
/*
 * Input: Row Number, Colomn Number
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Moves The LCD Cursor To The Specified Position
 * 							
 */
ERROR_STATUS LCD_gotoRowColumn(uint8_t a_ROW, uint8_t a_COL);
/*
 * Input: Row, column Numbers, and a String OF Characters
 * Output:
 * In/Out:	
 * Return: The error status of the function.			
 * Description: Writes A String OF Characters To The LCD @ The Specified Position 
 * 							
 */
ERROR_STATUS LCD_displayStringRowColumn(uint8_t a_ROW, uint8_t a_COL, uint8_t* a_STR);

#endif /* LCD_H_ */