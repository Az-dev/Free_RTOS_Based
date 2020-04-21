/*
 * UserStory3.h
 *
 * Created: 4/21/2020 12:34:30 PM
 *  Author: Az
 */ 


#ifndef USERSTORY3_H_
#define USERSTORY3_H_
/*- INCLUDES -------------------------------------------------------------------------------------------------------------*/
#include "../../../std_types.h"
#include "../../../common_macros.h"
/*- DEFINES ------------------------------------------------------------------------------------------------------------*/
/* Led State */
#define LED_ON  1
#define LED_OFF 2

/* Led action -based on btn press duration- */
#define SET_LED_OFF 0
#define xDELAY_400MS 1
#define xDELAY_100MS 2
/*- FUNCTION PROTOTYPES ---------------------------------------------------------------------------------------------------*/
extern void S1_UserStory3(void);

#endif /* USERSTORY3_H_ */