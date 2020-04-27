/*
 * Sprint3.h
 *
 * Created: 4/21/2020 2:19:11 PM
 *  Author: Az
 */ 


#ifndef PROJECT_H_
#define PROJECT_H_
/*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
#include "../../../std_types.h"
#include "../../../common_macros.h"
/*- DEFINES ------------------------------------------------------------------------------------------------------------*/
/* Led State */
#define LED_ON  1
#define LED_OFF 2

/* Keypad BTN values */
#define DIGIT_1 0
#define DIGIT_2 1
#define DIGIT_3 2
#define DIGIT_4 3
#define DIGIT_5 4
#define DIGIT_6 5
#define ENTER   55
#define DIGIT_0 7
#define ERASE   56
#define NO_ACTION 9

/* Event group bits */
#define BYTE_SENT             0X01
#define BYTE_RECIEVED         0X02
#define SEND_MSG              0X04

/* LCD states */
#define NO_LCD_ACTION         0
#define PRINT_RECEIVED_MSG    1
#define CLEAR_LCD             2


/* Message Size */
#define MESSAGE_SIZE    16

/*- FUNCTION PROTOTYPES ---------------------------------------------------------------------------------------------------*/
extern void S3_project(void);




#endif /* PROJECT_H_ */