/*
 * LCD.c
 *
 * Created: 3/11/2020 11:19:55 AM
 *  Author: Khaled Magdy
 */ 
#include "LCD.h"
#include "LCD_cfg.h"
#include "../../MCAL/DIO/DIO.h"

# define F_CPU 16000000UL
#include <util/delay.h> // for development only

static uint8_t LCD_Init_State = ZERO;
static ERROR_STATUS LCD_DATA(uint8_t a_Data);

ERROR_STATUS LCD_init (void)
{
	ERROR_STATUS errorStatus = E_NOK;
	
	if(LCD_Init_State == ZERO)
	{
		/* DIO Pins Initialization */
		strDioCfg_t LCD_Pins, LCD_EN;
		LCD_EN.GPIO   = LCD_EN_GPIO;
		LCD_EN.pins   = LCD_EN_BIT;
		LCD_EN.dir    = OUTPUT;
		LCD_Pins.GPIO = LCD_GPIO;
		LCD_Pins.dir  = OUTPUT;
		LCD_Pins.pins = (RS | RW | D4 | D5 | D6 | D7);
		DIO_init(&LCD_EN);
		DIO_init(&LCD_Pins);
		
		/* LCD Initialization Procedure (As Stated In Datasheet) */
		DIO_Write(LCD_GPIO, RS, LOW);
		DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
		LCD_DATA(0x00);
		_delay_ms(150);
		
		LCD_sendCommand(0x03);
		_delay_ms(5);
		LCD_sendCommand(0x03);
		_delay_us(150);
		
		LCD_sendCommand(0x03);
		LCD_sendCommand(0x02);
		
		LCD_sendCommand(0x02);
		LCD_sendCommand(0x08);
		
		LCD_sendCommand(0x00);
		LCD_sendCommand(0x0C);
		
		LCD_sendCommand(0x00);
		LCD_sendCommand(0x06);
		
		LCD_sendCommand(0x00);
		LCD_sendCommand(0x01);
	}
	else
	{
		//errorStatus = KEYPAD_ERROR + ALREADY_INIT;
		//return errorStatus;
	}
	
	LCD_Init_State = TRUE;
	errorStatus = E_OK;

	return errorStatus;
}

static ERROR_STATUS LCD_DATA(uint8_t a_Data)
{
	ERROR_STATUS errorStatus = E_NOK;
	
	if(a_Data & 1)
		DIO_Write(LCD_GPIO, D4, HIGH);
	else
		DIO_Write(LCD_GPIO, D4, LOW);
	if(a_Data & 2)
		DIO_Write(LCD_GPIO, D5, HIGH);
	else
		DIO_Write(LCD_GPIO, D5, LOW);
	if(a_Data & 4)
		DIO_Write(LCD_GPIO, D6, HIGH);
	else
		DIO_Write(LCD_GPIO, D6, LOW);
	if(a_Data & 8)
		DIO_Write(LCD_GPIO, D7, HIGH);
	else
		DIO_Write(LCD_GPIO, D7, LOW);
	
	errorStatus = E_OK;
		
	return errorStatus;
}

ERROR_STATUS LCD_sendCommand(uint8_t a_CMD)
{
	ERROR_STATUS errorStatus = E_NOK;
	
	// Select Command Register
	DIO_Write(LCD_GPIO, RS, LOW);
	// Move The Command Data To LCD
	LCD_DATA(a_CMD);
	// Send The EN Clock Signal
	DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
	_delay_us(15);
	DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
	_delay_us(15);
	
	errorStatus = E_OK;
	
	return errorStatus;
}

ERROR_STATUS LCD_displayChar(uint8_t a_CH)
{
	ERROR_STATUS errorStatus = E_NOK;
	uint8_t Low4,High4;
	  
	if(LCD_Init_State == TRUE)
	{
		Low4 = a_CH & 0x0F;
		High4 = a_CH & 0xF0;
		
		DIO_Write(LCD_GPIO, RS, HIGH);
		
		LCD_DATA(High4>>4);
		DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
		_delay_us(15);
		DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
		_delay_us(15);
		
		LCD_DATA(Low4);
		DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, HIGH);
		_delay_us(15);
		DIO_Write(LCD_EN_GPIO, LCD_EN_BIT, LOW);
		_delay_us(15);
	}
	else
	{
		//errorStatus = LCD_ERROR + NOT_INIT;
		//return errorStatus;
	} 
	
	errorStatus = E_OK;
	
	return errorStatus;
}

ERROR_STATUS LCD_gotoRowColumn(uint8_t a_ROW, uint8_t a_COL)
{
	ERROR_STATUS errorStatus = E_NOK;
	uint8_t Temp,Low4,High4;
	
	if(LCD_Init_State == TRUE)
	{
		if(a_ROW == 1)
		{
			Temp = 0x80 + a_COL - 1;
			High4 = Temp >> 4;
			Low4 = Temp & 0x0F;
			LCD_sendCommand(High4);
			LCD_sendCommand(Low4);
		}
		if(a_ROW == 2)
		{
			Temp = 0xC0 + a_COL - 1;
			High4 = Temp >> 4;
			Low4 = Temp & 0x0F;
			LCD_sendCommand(High4);
			LCD_sendCommand(Low4);
		}
	}
	else
	{
		//errorStatus = LCD_ERROR + NOT_INIT;
		//return errorStatus;
	}
	
	errorStatus = E_OK;
	
	return errorStatus;
}

ERROR_STATUS LCD_displayString(uint8_t* a_STR)
{
	ERROR_STATUS errorStatus = E_NOK;
	uint8_t i;
	
	if(LCD_Init_State == TRUE)
	{
		if(a_STR != NULL)
		{
			for(i=0; a_STR[i] != '\0'; i++)
			{
				LCD_displayChar(a_STR[i]);
			}
		}
		else
		{
			//errorStatus = LCD_ERROR + NULL_PTR;
			//return errorStatus;
		}
	}
	else
	{
		//errorStatus = LCD_ERROR + NOT_INIT;
		//return errorStatus;
	}

	errorStatus = E_OK;
	
	return errorStatus;
}

ERROR_STATUS LCD_displayStringRowColumn(uint8_t a_ROW, uint8_t a_COL, uint8_t* a_STR)
{
	ERROR_STATUS errorStatus = E_NOK;
	
	if(LCD_Init_State == TRUE)
	{
		if(a_STR != NULL)
		{
			LCD_gotoRowColumn(a_ROW, a_COL);
			LCD_displayString(a_STR);
		}
		else
		{
			//errorStatus = LCD_ERROR + NULL_PTR;
			//return errorStatus;
		}
	}
	else
	{
		//errorStatus = LCD_ERROR + NOT_INIT;
		//return errorStatus;
	}
	
	errorStatus = E_OK;
	
	return errorStatus;
}

ERROR_STATUS LCD_clear(void)
{
	ERROR_STATUS errorStatus = E_NOK;
	
	if(LCD_Init_State == TRUE)
	{
		LCD_sendCommand(0x00);
		LCD_sendCommand(0x01);
	}
	else
	{
		//errorStatus = LCD_ERROR + NOT_INIT;
		//return errorStatus;
	}
	
	errorStatus = E_OK;
	
	return errorStatus;
}