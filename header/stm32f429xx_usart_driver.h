#ifndef SMT32F429XX_USART_DRIVER_H
#define SMT32F429XX_USART_DRIVER_H
#include "stm32f429xx.h"

/*USART configuration structure*/
typedef struct {
    uint8_t Mode;         /*Specifies whether the Receive or Transmit mode is enabled or disabled.
                            This parameter could be a value of @ref USART_Mode*/

    uint16_t BaudRate;     /*This member configures the USART/UART comumnication baud rate.
                            This parameter can be a value of @ref USART_Common_BaudRate*/

    uint8_t WordLength;   /*Specifies the number of data bits transmitted or received in a frame.
                            This parameter can be a value of @ref USART_Word_Length*/

    uint8_t StopBits;     /*Specifies the number of stop bits transmitted.
                            This parameter can be a value of @ref USART_Stop_Bits*/

    uint8_t Parity;       /*Specifies the parity mode.
                            This parameter can be a value of @ref USART_Parity
                            @note When pararity is enabled, the computed parity is inserted
                            at the MSB position of the transmitted data (9th bit when
                            when the word length is set to 9 data bits; 8th bit when the
                            word length is set to 8 data bits).*/

    uint8_t OverSampling; /*Specifies whether Over sampling 8 is enabled or disabled, to achive higher speed (up to fPCLK/8).
                            This parameter can be a value of @ref USART_Over_Sampling*/
}USART_Conf_t;

/*USART_Mode*/
#define USART_MODE_RX           1U       /*Only CR_RE bit is enabled, receive mode only*/
#define USART_MODE_TX           2U       /*Only CR_TE bit is enabled, transmit mode only*/
#define USART_MODE_TX_RX        3U       /*CR_RE and CR_TE bits are enabled, both receive  and transmit mode*/

/*USART_Common_BaudRate*/
#define USART_BAUDRATE_115200   115200U  /*115200 bits per second*/
#define USART_BAUDRATE_57600    57600U   /*57600 bits per second*/
#define USART_BAUDRATE_38400    38400U   /*38400 bits per second*/
#define USART_BAUDRATE_19200    19200U   /*19200 bits per second*/
#define USART_BAUDRATE_9600     9600U     /*9600 bits per second*/
#define USART_BAUDRATE_4800     4800U    /*4800 bits per second*/

/*USART_Word_Length*/
#define USART_WORDLENGTH_8B     0U       /*8 bit word length, 1 Start bit, 8 Data bits, n Stop bits*/
#define USART_WORDLENGTH_9B     1U       /*9 bit word length, 1 Start bit, 9 Data bits, n Stop bits*/

/*USART_Stop_Bits*/
#define USART_STOPBITS_1        0U       /*1 Stop bit*/
#define USART_STOPBITS_0_5      1U       /*0.5 Stop bit*/
#define USART_STOPBITS_2        2U       /*2 Stop bit*/
#define USART_STOPBITS_1_5      3U       /*1.5 Stop bit*/

/*USART_Parity*/
#define USART_PARITY_NONE       0U       /*Parity control is disabled*/
#define USART_PARITY_EVEN       0U       /*Parity control is enabled, Even parity is selected*/
#define USART_PARITY_ODD        0U       /*Parity control is enabled, Odd parity is selected*/

/*USART_Over_Sampling*/
#define USART_OVERSAMPLING_16   0U       /*Oversampling by 16*/
#define USART_OVERSAMPLING_8    1U       /*Oversampling by 8*/

/*USART register bits --------------------------------------*/
/*USART CR1*/
#define USART_CR1_RE            2U /*RE bit*/
#define USART_CR1_TE            3U /*TE bit*/
#define USART_CR1_RXNEIE        5U /*RXNEIE bit*/
#define USART_CR1_PS            9U /*PS bit*/
#define USART_CR1_PCE           10U /*PCE bit*/
#define USART_CR1_M             12U /*M bit*/
#define USART_CR1_UE            13U /*UE bit*/
#define USART_CR1_OVER8         15U /*OVER8 bit*/

/*USART CR2*/
#define USART_CR2_STOP          12U /*STOP bit*/

/*USART BRR*/
#define USART_DIV_MANTISSA      4U /*Div_Mantissa*/
#define USART_DIV_FRACTION      0U /*Div_Fraction*/

#define USART_SR_TXE            7U /*TXE bit*/
#define USART_SR_RXNE           5U /*RXNE bit: read data register not empty */

/*Interrupt configuration*/
#define USART3_RXNEIE_ENB()     (USART3->CR1 |= (1U << USART_CR1_RXNEIE))    /*enable receive not empty interrupt*/
#define USART3_RXNEIE_DIS()     (USART3->CR1 &= ~(1U << USART_CR1_RXNEIE))   /*disable receive not empty interrupt*/

/*Function prototypes*/
void USART_Init(USART_RegDef_t *USARTx, USART_Conf_t USART_Conf);
void USART_Transmit(USART_RegDef_t *USARTx, uint8_t *Mess, uint8_t MessSize);
void USART_Receive(USART_RegDef_t *USARTx, uint8_t *Mess, uint8_t MessSize);

#endif //SMT32F429XX_USART_DRIVER_H