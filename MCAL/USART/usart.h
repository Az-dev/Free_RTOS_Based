#ifndef __USART_H__
#define __USART_H__
/*- INCLUDES ------------------------------------------------------------------------------------------------------*/
#include "../../registers.h"
#include "../../std_types.h"
/*- DEFINES --------------------------------------------------------------------------------------------------------*/
#define BAUDE_RATE 103 /* this value generates baude rate of 9600 at Fosc = 16MHz */
/*- TYPEDEFS -------------------------------------------------------------------------------------------------------*/
/*---- 1 - (UCSRC) CONFIG ENUMS ----*/
/* Select mode enum : whether Asynchronous or Synchronous */
typedef enum sel_mode
{
   ASYNCH_MODE = 0X00,
   SYNCH_MODE  = 0X40
}Sel_mode_t;

/* Parity mode selection enum */
typedef enum sel_parity
{
   PARITY_DISABLED = 0X00,
   PARITY_RESERVED = 0X10,
   PARITY_EVEN     = 0X20,
   PARITY_ODD      = 0X30
}Sel_parity_t;

/* Stop bit select mode enum : select whether stop bit will be 1 or 2 bits */
typedef enum sel_stop_bit
{
   STOP_1_BIT = 0X00,
   STOP_2_BIT = 0X08
}Sel_stop_bit_t;

/* Register select enum : select whether (UCSRC) or (UBRRH) */
typedef enum reg_sel
{
   UBRRH_SEL = 0X00,
   UCSRC_SEL = 0X80
}Reg_select_t;

/*---- 2 - (UCSRB) CONFIG ENUMS ----*/
/* Choose interrupt mode enum */
typedef enum interrupt_mode
{
   RECIEVE_COMPLETE_EN = 0X80,
   TRANSMIT_COMPLETE_EN = 0X40,
   DATA_REGISTER_EMPTY_EN = 0X20,
   INTERRUPT_DIS = 0x10
}Interrupt_mode_t;

/* Transmit complete interrupt enable enum */
typedef enum usart_dir
{
   RECIEVE_EN = 0X10,
   TRANSMIT_EN = 0X08,
}Usart_dir_t;

/*---- 3 - (UCSRA) CONFIG ENUMS ----*/
/* double usart speed mode */
typedef enum u2x
{
   U2X_EN = 0X02,
   U2X_DIS = 0X00,
}U2x;

/*---- 4 - Usart_init  STRUCT ----*/
typedef struct usart_init
{
   /*-------------- UCSRC config ----------*/
   Sel_mode_t usart_mode_sel;
   Sel_parity_t parity_mode_sel;
   Sel_stop_bit_t stop_bit_sel;
   Reg_select_t reg_sel_mode;
   /*------------- UCSRB config ----------*/
   Interrupt_mode_t interrupt_mode_sel;
   Usart_dir_t usart_dir_sel;
   /*------------- UCSRA config ----------*/
   U2x double_speed_select;
}gstr_usart_init_t;

/*---- USART Error Enum ----*/
typedef enum EnumUSARTError
{
   INIT_USART_CONFIG_OK = 0,
   INVALID_USART_INPUT_PARAMS = 1,
   USART_BYTE_WRITE_SUCCESS = 2,
   USART_BYTE_WRITE_FAIL = 3,
   USART_BYTE_READ_SUCCESS = 4,
   USART_BYTE_READ_FAIL = 5,
   UDR_RESET_SUCCESS = 6,
   UDR_RESET_FAIL = 7,
   USART_BYTE_TRANSMIT_SUCCESS = 8,
   USART_BYTE_TRANSMIT_FAIL = 9,
   USART_CALL_BACK_SET_SUCCESS = 10,
     
}EnumUSARTError_t;


/*- FUNCTION PROTOTYPES --------------------------------------------------------------------------------------------*/
/*
*  Description : Initializes USART.
*
*  @param const gstr_usart_init_t USART_InitCfg
*  @return EnumUSARTError_t
*/
extern EnumUSARTError_t Usart_Init(const gstr_usart_init_t * USART_InitCfg);

/*
*  Description : Read a character from RXB.
*
*  @param uint8_t * data_byte
*  @return void
*/
extern void UsartReadRx(uint8_t * data_byte);

/*
*  Description : Write a character to TXB
*
*  @param uint8_t * data_byte  (input param)
*  @return void
*/
extern void UsartWriteTx(uint8_t * data_byte);

/*
*  Description : Resets UDR .
*
*  @param void
*
*  @return EnumUSARTError_t
*/
extern EnumUSARTError_t ResetUDR(void);

/*
*  --- !!! this function is deprecated !!!---
*  Description : Get the state of the Reception complete software flag.
*
*  @param void
*  @return EnumUSARTError_t
*/
//extern EnumUSARTError_t getReceptionState(void);

/*
*  Description : Gets Rx notification.
*
*  @param uint8_t * state (output param)
*  @return void
*/
void UsartGetRxState(uint8_t * state);

/*
*  Description : Gets Tx notification.
*
*  @param uint8_t * state (output param)
*  @return void
*/
void UsartGetTxState(uint8_t * state);

/*
*  Description : Set Tx Call back
*
*  @param void (*call_back_ptr)(void)
*
*  @return EnumUSARTError_t
*/
extern EnumUSARTError_t USART_SetTxCallBack(void (*call_back_ptr)(void));

/*
*  Description : Set Rx call back.
*
*  @param void (*call_back_ptr)(void)
*
*  @return EnumUSARTError_t
*
*/
extern EnumUSARTError_t USART_SetRxCallBack(void (*call_back_ptr)(void));

#endif /* END OF __USART_H__ */