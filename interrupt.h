#ifndef INTERRUPT_H_
#define INTERRUPT_H_
/*- FUNCTION-LIKE MACROS ---------------------------------------------------------------------------------------------------*/
#define sei() __asm__ __volatile__ ("sei" ::: "memory")
#define cli() __asm__ __volatile__ ("cli" ::: "memory")

/*ISR_TIMER0_OVF*/
#define ISR_TIMER0_OVF()\
void __vector_11(void) __attribute__ ((signal));\
void __vector_11(void)

/*ISR_TIMER0_COMP*/
#define ISR_TIMER0_COMP()\
void __vector_10(void) __attribute__ ((signal));\
void __vector_10(void)

/*ISR_TIMER1_OVF*/
#define ISR_TIMER1_OVF()\
void __vector_9(void) __attribute__ ((signal));\
void __vector_9(void)

/*ISR_TIMER2_OVF*/
#define ISR_TIMER2_OVF()\
void __vector_5(void) __attribute__ ((signal));\
void __vector_5(void)

/*ISR_INT2*/
#define ISR_INT2()\
void __vector_3(void) __attribute__ ((signal));\
void __vector_3(void)

/*ISR_SPI*/
#define ISR_SPI()\
void __vector_12(void) __attribute__ ((signal , used , externally_visible));\
void __vector_12(void)

/*ISR_USART_RX_COMPLETE*/
#define ISR_USART_RX()\
void __vector_13(void) __attribute__ ((signal , used , externally_visible));\
void __vector_13(void)

/*ISR_USART_DATA_REGISTER_EMPTY*/
#define ISR_USART_UDRE()\
void __vector_14(void) __attribute__ ((signal));\
void __vector_14(void)

/*ISR_USART_TX_COMPLETE*/
#define ISR_USART_TX()\
void __vector_15(void) __attribute__ ((signal , used , externally_visible));\
void __vector_15(void)

#endif /* INTERRUPT_H_ */
