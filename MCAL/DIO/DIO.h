/*
 * DIO.h
 *
 * Created: 2/17/2020 8:37:03 PM
 *  Author: Az
 */ 

#ifndef DIO_H_
#define DIO_H_

/*- INCLUDES --------------------------------------------------------------------------------------------------------------*/
#include "../../common_macros.h"
#include "../../std_types.h"
#include "../../registers.h"
/*- DEFINES --------------------------------------------------------------------------------------------------------------*/
#define PIN0	      0x01
#define PIN1	      0x02
#define PIN2	      0x04
#define PIN3	      0x08
#define PIN4	      0x10
#define PIN5	      0x20
#define PIN6	      0x40
#define PIN7	      0x80

#define BIT0	      0x01
#define BIT1	      0x02
#define BIT2	      0x04
#define BIT3	      0x08
#define BIT4	      0x10
#define BIT5	      0x20
#define BIT6	      0x40
#define BIT7	      0x80

#define UPPER_NIBBLE	0xF0
#define LOWER_NIBBLE	0x0F
#define FULL_PORT		0xFF

#define GPIOA			0
#define GPIOB			1
#define GPIOC			2
#define GPIOD			3

#define OUTPUT			0xFF
#define INPUT			0x00
/*- TYPEDEFS --------------------------------------------------------------------------------------------------------------*/
typedef struct strDioCfg_t
{
   uint8_t GPIO;
   uint8_t pins;
   uint8_t dir;
}strDioCfg_t;

/*- FUNCTIONS PROTOTYPES -------------------------------------------------------------------------------------------------*/

/*
*Input: strDioCfg_t -> to get PORT name, pins to be initiated and the required direction
*Output: No output
*In/Out:
*Description: This function can set the direction of a full port, a nibble
* 			  or even one pin.
*/
extern ERROR_STATUS DIO_init (strDioCfg_t *DIO_info);


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
extern ERROR_STATUS DIO_Write (uint8_t GPIO, uint8_t pins, uint8_t value);

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
extern ERROR_STATUS DIO_Read (uint8_t GPIO,uint8_t pins, uint8_t *data);

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
extern ERROR_STATUS DIO_Toggle (uint8_t GPIO, uint8_t pins);

#endif /* DIO_H_ */