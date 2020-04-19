/*
 * KeyPad.c
 *
 * Created: 3/11/2020 11:20:37 AM
 *  Author: Khaled Magdy
 */ 
#include "KeyPad.h"
#include "KeyPad_cfg.h"
#include "../../MCAL/DIO/DIO.h"


static strDioCfg_t Ex_Keypad_COL[COLS];
static strDioCfg_t Ex_Keypad_ROW[ROWS];
static uint8_t Keypad_Init_State = ZERO;

/*
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the keypad GPIO pins
 * 							
 */
ERROR_STATUS Keypad_Init(void)
{
	strDioCfg_t Keypad_COL[COLS], Keypad_ROW[ROWS];
	ERROR_STATUS errorStatus = E_NOK;
	
	if(Keypad_Init_State == ZERO)
	{
		/* Initialize The Columns IO Pins */
		
		Keypad_COL[0].GPIO = KEYPAD_GPIO;
		Keypad_COL[0].dir = OUTPUT;
		Keypad_COL[0].pins = COL1;
		Ex_Keypad_COL[0] = Keypad_COL[0];
		Keypad_COL[1].GPIO = KEYPAD_GPIO;
		Keypad_COL[1].dir = OUTPUT;
		Keypad_COL[1].pins = COL2;
		Ex_Keypad_COL[1] = Keypad_COL[1];
		Keypad_COL[2].GPIO = KEYPAD_GPIO;
		Keypad_COL[2].dir = OUTPUT;
		Keypad_COL[2].pins = COL3;
		Ex_Keypad_COL[2] = Keypad_COL[2];
		DIO_init(&Keypad_COL[0]);
		DIO_init(&Keypad_COL[1]);
		DIO_init(&Keypad_COL[2]);
		
		/* Initialize The Columns IO Pins */
		
		Keypad_ROW[0].GPIO = KEYPAD_GPIO;
		Keypad_ROW[0].dir = INPUT;
		Keypad_ROW[0].pins = ROW1;
		Ex_Keypad_ROW[0] = Keypad_ROW[0];
		Keypad_ROW[1].GPIO = KEYPAD_GPIO;
		Keypad_ROW[1].dir = INPUT;
		Keypad_ROW[1].pins = ROW2;
		Ex_Keypad_ROW[1] = Keypad_ROW[1];
		Keypad_ROW[2].GPIO = KEYPAD_GPIO;
		Keypad_ROW[2].dir = INPUT;
		Keypad_ROW[2].pins = ROW3;
		Ex_Keypad_ROW[2] = Keypad_ROW[2];
		DIO_init(&Keypad_ROW[0]);
		DIO_init(&Keypad_ROW[1]);
		DIO_init(&Keypad_ROW[2]);
		
		/* Activate The Internal Pull Up Resistors*/
		
		DIO_Write(Keypad_ROW[0].GPIO, Keypad_ROW[0].pins, HIGH);
		DIO_Write(Keypad_ROW[1].GPIO, Keypad_ROW[1].pins, HIGH);
		DIO_Write(Keypad_ROW[2].GPIO, Keypad_ROW[2].pins, HIGH);
	}	
	else
	{
		//errorStatus = KEYPAD_ERROR + ALREADY_INIT;
		return errorStatus;
	}
	
	Keypad_Init_State = TRUE;
	errorStatus = E_OK;
	
	return errorStatus;
}
/*
 * Input: 
 * Output:
 * In/Out:	Keys_Status Array Representing The Status OF Each Key in the Keypad
 * Return: The error status of the function.			
 * Description: Scans All The Keys in The Keypad, Applies An FIR Digital Filter To The Key Status. 
 * 							
 */
ERROR_STATUS Keypad_Scan(uint8_t* a_Keys_Status)
{
	ERROR_STATUS errorStatus = E_NOK;
	static uint8_t index = ZERO;
	static uint8_t States_Buffer[FILTER_ORDER][KEYS_NUM] = {{1}};
	uint8_t a_ret_states[ROWS] = {1};
	uint8_t a_ret=0, j=0, k=0, sum=0;
	/*--[ Null Pointer Check ]--*/
	if(a_Keys_Status == NULL)
	{
		//errorStatus = KEYPAD_ERROR + NULL_PTR;
		return errorStatus;
	}
	/*--[ Scan Without Init Check ]--*/
	if(Keypad_Init_State == TRUE)
	{
		/* Steady State (Output Pattern 0-0-0) */
		DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, LOW);
		DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, LOW);
		DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, LOW);
		
		/* Read The Rows*/
		DIO_Read(Ex_Keypad_ROW[0].GPIO, Ex_Keypad_ROW[0].pins, &a_ret_states[0]);
		DIO_Read(Ex_Keypad_ROW[1].GPIO, Ex_Keypad_ROW[1].pins, &a_ret_states[1]);
		DIO_Read(Ex_Keypad_ROW[2].GPIO, Ex_Keypad_ROW[2].pins, &a_ret_states[2]);
		
		/* Check If Any Key Is Pressed */
		if(!a_ret_states[0] | !a_ret_states[1] | !a_ret_states[2])
		{
			/*--[ 1st Row Check ]--*/
			if(!a_ret_states[0])
			{
				/* (Output Pattern 1-0-0) */
				DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, LOW);
				DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, HIGH);
				DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
				/* Read ROW0 */
				DIO_Read(Ex_Keypad_ROW[0].GPIO, Ex_Keypad_ROW[0].pins, &a_ret);
				if(!a_ret)
				{
					States_Buffer[index][0] = 0;
				}
				else
				{
					States_Buffer[index][0] = 1;
					
					/* (Output Pattern 1-0-1) */
					DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, HIGH);
					DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, LOW);
					DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
					
					/* Read ROW0 */
					DIO_Read(Ex_Keypad_ROW[0].GPIO, Ex_Keypad_ROW[0].pins, &a_ret);
					if(!a_ret)
					{
						States_Buffer[index][1] = 0;
					}
					else
					{
						States_Buffer[index][1] = 1;
						States_Buffer[index][2] = 0;
					}
				}
			}
			else
			{
				States_Buffer[index][0] = 1;
				States_Buffer[index][1] = 1;
				States_Buffer[index][2] = 1;
			}
			/*--[ 2nd Row Check ]--*/
			if(!a_ret_states[1])
			{
				/* (Output Pattern 1-0-0) */
				DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, LOW);
				DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, HIGH);
				DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
				/* Read ROW1 */
				DIO_Read(Ex_Keypad_ROW[1].GPIO, Ex_Keypad_ROW[1].pins, &a_ret);
				if(!a_ret)
				{
					States_Buffer[index][3] = 0;
				}
				else
				{
					States_Buffer[index][3] = 1;
					
					/* (Output Pattern 0-1-0) */
					DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, HIGH);
					DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, LOW);
					DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
					
					/* Read ROW1 */
					DIO_Read(Ex_Keypad_ROW[1].GPIO, Ex_Keypad_ROW[1].pins, &a_ret);
					if(!a_ret)
					{
						States_Buffer[index][4] = 0;
					}
					else
					{
						States_Buffer[index][4] = 1;
						States_Buffer[index][5] = 0;
					}
				}
			}
			else
			{
				States_Buffer[index][3] = 1;
				States_Buffer[index][4] = 1;
				States_Buffer[index][5] = 1;
			}
			/*--[ 3rd Row Check ]--*/
			if(!a_ret_states[2])
			{
				/* (Output Pattern 0-1-1) */
				DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, LOW);
				DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, HIGH);
				DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
				/* Read ROW2 */
				DIO_Read(Ex_Keypad_ROW[2].GPIO, Ex_Keypad_ROW[2].pins, &a_ret);
				if(!a_ret)
				{
					States_Buffer[index][6] = 0;
				}
				else
				{
					States_Buffer[index][6] = 1;
					
					/* (Output Pattern 1-0-1) */
					DIO_Write(Ex_Keypad_COL[0].GPIO, Ex_Keypad_COL[0].pins, HIGH);
					DIO_Write(Ex_Keypad_COL[1].GPIO, Ex_Keypad_COL[1].pins, LOW);
					DIO_Write(Ex_Keypad_COL[2].GPIO, Ex_Keypad_COL[2].pins, HIGH);
					
					/* Read ROW2 */
					DIO_Read(Ex_Keypad_ROW[2].GPIO, Ex_Keypad_ROW[2].pins, &a_ret);
					if(!a_ret)
					{
						States_Buffer[index][7] = 0;
					}
					else
					{
						States_Buffer[index][7] = 1;
						States_Buffer[index][8] = 0;
					}
				}
			}
			else
			{
				States_Buffer[index][6] = 1;
				States_Buffer[index][7] = 1;
				States_Buffer[index][8] = 1;
			}
		}
		/* Else IF No Key IS Pressed */
		else
		{
			for(k=0; k<KEYS_NUM; k++)
			{
				States_Buffer[index][k] = 1;
			}
		}
		/* Apply The FIR Digital Filter On Key States Array */
		/* Set The Return Key States Array Based On Filter's Output */
		for(k=0; k<KEYS_NUM; k++)
		{
			sum = ZERO;
			for(j=0; j<FILTER_ORDER; j++)
			{
				sum += States_Buffer[j][k];
			}
			if(sum == 8)
			{
				a_Keys_Status[k] = KEY_RELEASED;
			}
			else if(sum == 0)
			{
				a_Keys_Status[k] = KEY_PRESSED;
			}
			else
			{
				a_Keys_Status[k] = KEY_FLOATING;
			}
		}
		/* Increment The Filter Buffer Pointer And Bound-Check It */
		index++;
		if(index == FILTER_ORDER)
		index = ZERO;
	}
	/* Else if The Keypad Module Is Not Initialized Yet */
	else
	{
		//errorStatus = KEYPAD_ERROR + NOT_INIT;
		return errorStatus;
	}
	
	errorStatus = E_OK;
	
	return errorStatus;
}