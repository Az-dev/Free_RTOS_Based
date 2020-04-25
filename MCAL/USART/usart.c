 /*-INCLUDES ------------------------------------------------------------------------------------------------------*/
#include "usart.h"
#include "../../interrupt.h"

/*- GLOBAL VARIALBLES ---------------------------------------------------------------------------------------------*/
//volatile uint8_t gu8_RX_CompleteFlag = 0;    /* Receive complete software flag */
//volatile uint8_t gu8_TX_CompleteFlag = 0;    /* Transmit complete software flag */
static void (*tx_call_back)(void) = NULL;
static void (*rx_call_back)(void) = NULL;
static void (*udre_call_back)(void) = NULL;
/* TX/RX notifications */
static uint8_t gu8_txNotification = 0;
static uint8_t gu8_rxNotification = 0;
/*- FUNCTION DEFINITIONS ------------------------------------------------------------------------------------------*/
/*
*  Description : Initializes USART.
*
*  @param const gstr_usart_init_t USART_InitCfg
*  @return EnumUSARTError_t
*/
EnumUSARTError_t Usart_Init(const gstr_usart_init_t * USART_InitCfg)
{
   /* Define error state */
   uint8_t au8_errrorState = 0;
   /* Check Null pointer */
   if(NULL != USART_InitCfg)
   {
      /*----------------- initializing (UCSRC) -----------------*/
      UCSRC |= (0x80|USART_InitCfg->usart_mode_sel|USART_InitCfg->stop_bit_sel|USART_InitCfg->reg_sel_mode|0x06); /*0x06 becuase we want UCSZ0:1 to be set to 1 1 ---> in order to obtain 8-bit width character*/
      //UCSRC |= 0x85; /*works fine*/
      /*----------------- initializing (UCSRB) -----------------*/
      UCSRB |= ((USART_InitCfg->interrupt_mode_sel) | (USART_InitCfg->usart_dir_sel) & (~(0x04)) & (~(0x03)));
      //UCSRB  |= 0x90;
      /*----------------- initializing (UCSRA) -----------------*/
      //UCSRA &= ~(0x1C); /* Check that FE & DOR & PE is set to zero*/
      UCSRA |= (USART_InitCfg->double_speed_select & ~(0x1C));
      /*------------------ initialize baude rate ---------------*/
      UBRRL = BAUDE_RATE;
      /*------------------ Character size select  ---------------*/      
      /*---- setting it to 8-bit -----*/
      //UCSRC |= 0x05;
      //UCSRB &= ~(0x04); /* ----> check this if it will work or not */
      /*----- clear RXB8 and TXB8 ----*/
      //UCSRB &= ~(0x03);
      /*---- set global interrupt -----*/
      sei();
      /* Report success */
      au8_errrorState = INIT_USART_CONFIG_OK; 
   }
   else
   {
      /* Report Fail */
      au8_errrorState = INVALID_USART_INPUT_PARAMS;
   }
   return au8_errrorState;   
}

/*
*  Description : Read a character from RXB.
*
*  @param uint8_t * data_byte (output param)
*  @return void
*/
void UsartReadRx(uint8_t * data_byte)
{
   /* Define error state */
   //uint8_t au8_errorState = 0;
   /* Check Null pointer */
   if(NULL != data_byte)
   {
      /* Read byte in udr*/
      *data_byte = UDR;      
      /* Notify success*/
      //gu8_rxNotification = USART_BYTE_READ_SUCCESS;           
   }
   else
   {
      /* Notify Fail */
      //gu8_rxNotification = INVALID_USART_INPUT_PARAMS;
   }   
}

/*
*  Description : Write a character to TXB
*
*  @param uint8_t * data_byte  (input param)
*  @return void
*/
void UsartWriteTx(uint8_t * data_byte)
{
   /* Define error state */
   //uint8_t au8_errorState = 0;
   /* Check NULL pointer */
   if(NULL != data_byte)
   {
      /* Write value  to UDR */
      UDR = *data_byte;
      /* notify Success */
      //gu8_txNotification = USART_BYTE_WRITE_SUCCESS;

      //check UDR is ready for the next byte : by checking UDRE flag 
      /*
      if(0 == (UCSRA & 0x20))
      {
            
      }
      */           
   }
   else
   {
      /* notify Fail */
      //gu8_txNotification = INVALID_USART_INPUT_PARAMS;     
   }    
}

/*
*  Description : Gets Tx notification.
*  
*  @param uint8_t * state (output param)
*  @return void
*/
void UsartGetTxState(uint8_t * state)
{
   *state = gu8_txNotification;
}

/*
*  Description : Gets Rx notification.
*
*  @param uint8_t * state (output param)
*  @return void
*/
void UsartGetRxState(uint8_t * state)
{
   *state = gu8_rxNotification;
}

/*
*  --- !!! this function is deprecated !!!---
*  Description : Get the state of the Reception complete software flag. 
*
*  @param void
*  @return void
*/
//EnumUSARTError_t getReceptionState(void)
//{
   /* define error state*/
//    uint8_t au8_errorState = 0;
//    if(gu8_RX_CompleteFlag == 1)
//    {
//       au8_errorState = USART_BYTE_TRANSMIT_SUCCESS;
//       /* Reset Reception state flag */
//       gu8_RX_CompleteFlag = 0;      
//    }
//    else
//    {
//       au8_errorState = USART_BYTE_TRANSMIT_FAIL;
//    }
//    return au8_errorState;   
//}


/*
*  Description : Resets UDR .
*
*  @param void
*
*  @return EnumUSARTError_t
*/
EnumUSARTError_t ResetUDR(void)
{
   /* Define error state */
   uint8_t au8_errorState = 0;
   /*Reset UDR*/
   UDR = 0;
   /* Check for failure*/
   if(0 == UDR)
   {
      /* Report Success*/
      au8_errorState = UDR_RESET_SUCCESS;     
   }
   else
   {
      /* Report Fail */
      au8_errorState = UDR_RESET_FAIL; 
   }
   return au8_errorState;
}

/*
*  Description : Set Tx Call back
*  
*  @param void (*call_back_ptr)(void)
*
*  @return EnumUSARTError_t
*/
EnumUSARTError_t USART_SetTxCallBack(void (*call_back_ptr)(void))
{
   uint8_t au8_errorState = 0;
   if(NULL != call_back_ptr)
   {
      tx_call_back = call_back_ptr;
      /* report success */
      au8_errorState = USART_CALL_BACK_SET_SUCCESS;
   }
   else
   {
      /* report fail */
      au8_errorState = INVALID_USART_INPUT_PARAMS;
   }
   return au8_errorState;
}

/*
*  Description : Set UDRE Call back
*
*  @param void (*call_back_ptr)(void)
*
*  @return EnumUSARTError_t
*/
EnumUSARTError_t USART_Set_UDRE_CallBack(void (*call_back_ptr)(void))
{
   uint8_t au8_errorState = 0;
   if(NULL != call_back_ptr)
   {
      udre_call_back = call_back_ptr;
      /* report success */
      au8_errorState = USART_CALL_BACK_SET_SUCCESS;
   }
   else
   {
      /* report fail */
      au8_errorState = INVALID_USART_INPUT_PARAMS;
   }
   return au8_errorState;
}

/*
*  Description : Set Rx call back.
*
*  @param void (*call_back_ptr)(void)
*
*  @return EnumUSARTError_t
*
*/
EnumUSARTError_t USART_SetRxCallBack(void (*call_back_ptr)(void))
{   
   uint8_t au8_errorState = 0;
   if(NULL != call_back_ptr)
   {
      rx_call_back = call_back_ptr;
      /* report success */
      au8_errorState = USART_CALL_BACK_SET_SUCCESS;
   }
   else
   {
      /* report fail */
      au8_errorState = INVALID_USART_INPUT_PARAMS;
   }
   return au8_errorState;
}
/*--------------------------------------------------- Interrupt handlers  -----------------------------------------*/
/*
* USART on successful character receive completion interrupt handler 
*/
ISR_USART_RX()
{
   /* on successful character reception : you can read the new character */  
   //gu8_RX_CompleteFlag = 1;
   rx_call_back();
}

/*
* USART when data register is empty
*/
ISR_USART_UDRE()
{
   /* if data register is empty : you can write a new character. */ 
   udre_call_back();           
}

ISR_USART_TX()
{
   /* if data register is empty : you can write a new character. */
   //gu8_TX_CompleteFlag = 1;
   tx_call_back();     
}