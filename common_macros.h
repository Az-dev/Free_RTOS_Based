/*
 * common_macros.h
 *
 * Created: 2/17/2020 8:50:36 PM
 *  Author: Az
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

/*- FUNCTION-LIKE MACROS ---------------------------------------------------------------------------------------------------*/
/* Sets a given bit for a given port */
#define SET_BIT(PORT,BIT)           (PORT|=(1<<BIT))
/* Clears a given bit for a given port */
#define CLEAR_BIT(PORT,BIT)         (PORT&=(~(1<<BIT)))
/* Toggles a given bit for a given port */
#define TOGGLE_BIT(PORT,BIT)        (PORT^=(1<<BIT))
/* Checks if a given bit for a given port is set */
#define BIT_IS_SET(PORT,BIT)        (PORT&(1<<BIT))
/* Checks if a given bit for a given port is not set */
#define BIT_IS_CLEARED(PORT,BIT)    (!(PORT&(1<<BIT)))

#endif /* COMMON_MACROS_H_ */