/*- INCLUDES -----------------------------------------------------------------------------------------------------*/
#include "usart.h"
/*- CONSTANTS ----------------------------------------------------------------------------------------------------*/
/* create USART initialization struct */
const gstr_usart_init_t usart_init_config = 
{
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,
   (RECIEVE_COMPLETE_EN),
   //INTERRUPT_DIS,   
   (RECIEVE_EN|TRANSMIT_EN),
   U2X_DIS,
};

/*------------------------------------- Free RTOS related usart struct -----------------------*/
const gstr_usart_init_t free_rtos_usart_config =
{
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,
   (RECIEVE_COMPLETE_EN|TRANSMIT_COMPLETE_EN),
   //INTERRUPT_DIS,
   (RECIEVE_EN|TRANSMIT_EN),
   U2X_DIS,
};


/*-------------------------------------- BCM Related Configurations -------------------------*/

/* Transmitter configuration */
const gstr_usart_init_t usart_init_config_transmit =
{
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,   
   TRANSMIT_COMPLETE_EN,
   TRANSMIT_EN,
   U2X_DIS,
};

/* Receiver configuration */
const gstr_usart_init_t usart_init_config_receive =
{
   ASYNCH_MODE,
   PARITY_DISABLED,
   STOP_1_BIT,
   UCSRC_SEL,
   RECIEVE_COMPLETE_EN,   
   RECIEVE_EN,
   U2X_DIS,
};


