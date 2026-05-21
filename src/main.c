#include "stm32f429xx.h"
#include <string.h>

#define BLINK_DELAY    1000000
#define DEBOUNCE_DELAY 300000

/**
 * @brief Simple delay function. It is uses a for loop to execute a delay.
 *        Change the amount to increase or decrease the delay. 
 * 
 * @param amount Represents a number value that will decrease until it reaches zero.
 */
void delay(uint32_t amount) {
    volatile uint32_t cnt = amount;
    while (cnt--);
}

GPIO_PinConf_t greenLed = {
    .GPIO_PinNumber = GPIO_PIN_NUM_13,
    .GPIO_PinMode   = GPIO_MODE_OUTPUT,
    .GPIO_OutType   = GPIO_OUTPUT_PP,
    .GPIO_OutSpeed  = GPIO_SPEED_LOW,
    .GPIO_PUPD      = GPIO_NO_PUPD
};

GPIO_PinConf_t redLed = {
    .GPIO_PinNumber = GPIO_PIN_NUM_14,
    .GPIO_PinMode   = GPIO_MODE_OUTPUT,
    .GPIO_OutType   = GPIO_OUTPUT_PP,
    .GPIO_OutSpeed  = GPIO_SPEED_LOW,
    .GPIO_PUPD      = GPIO_NO_PUPD
};

GPIO_PinConf_t pushButton = {
    .GPIO_PinNumber   = GPIO_PIN_NUM_0,
    .GPIO_PinMode     = GPIO_MODE_INPUT,
    .GPIO_PUPD        = GPIO_NO_PUPD,
    .GPIO_EdgeTrigger = GPIO_IT_EDGE_RT
};

/*GPIO - USART pin configuration
configure GPIO (GPIOB pin10/Tx pin11/Rx) pin to use in alternate function mode (USART3)*/
GPIO_PinConf_t USART_Tx_Pin = {
    .GPIO_PinNumber = GPIO_PIN_NUM_10, /*Tx pin*/
    .GPIO_PinMode   = GPIO_MODE_ALT,
    .GPIO_OutType   = GPIO_OUTPUT_PP,
    .GPIO_OutSpeed  = GPIO_SPEED_VERY_HIGH,
    .GPIO_PUPD      = GPIO_NO_PUPD,
    .GPIO_AltFnc    = GPIO_ALT_AF7
};
GPIO_PinConf_t USART_Rx_Pin = {
    .GPIO_PinNumber = GPIO_PIN_NUM_11, /*Rx pin*/
    .GPIO_PinMode   = GPIO_MODE_ALT,
    .GPIO_OutType   = GPIO_OUTPUT_PP,
    .GPIO_OutSpeed  = GPIO_SPEED_VERY_HIGH,
    .GPIO_PUPD      = GPIO_NO_PUPD,
    .GPIO_AltFnc    = GPIO_ALT_AF7
};

USART_Conf_t USART3_Conf = {
    .Mode           = USART_MODE_TX_RX,       /*Transmit and receive mode*/
    .Parity         = USART_PARITY_NONE,      /*None parity control*/
    .StopBits       = USART_STOPBITS_1,       /*1 stop bit*/
    .WordLength     = USART_WORDLENGTH_8B,    /*8bit word length*/
    .OverSampling   = USART_OVERSAMPLING_16,  /*Oversampling by 16*/
    .BaudRate       = USART_BAUDRATE_9600,    /*None parity control*/
};

int main() {
    uint8_t ReceivedMess[4] = {0};
    uint8_t ReceivedMessSize = 3U;
    //GPIOA_CLK_ENB();
    //GPIO_Init(GPIOA, pushButton);
    //GPIO_IT_Init(GPIOA, pushButton, 1U);
    
    GPIOG_CLK_ENB();
    GPIO_Init(GPIOG, greenLed);
    //GPIO_Init(GPIOG, redLed);

    GPIOB_CLK_ENB();
    GPIO_Init(GPIOB, USART_Tx_Pin);
    GPIO_Init(GPIOB, USART_Rx_Pin);

    USART3_CLK_ENB();
    USART_Init(USART3, USART3_Conf);

    /*lock green led pin configuration*/
    //GPIO_LockPinConf(GPIOG, GPIO_PIN_NUM_13);
    /*turn on the green led*/
    //GPIO_WritePin(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
    while (1) {
        /*if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_14, GPIO_PIN_LOW);
            delay(BLINK_DELAY);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_14, GPIO_PIN_HIGH);
            delay(BLINK_DELAY);
        }*/

        /*if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
            delay(DEBOUNCE_DELAY);
            if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
                GPIO_ToggleLed(GPIOG, GPIO_PIN_NUM_13);
            }
            
        }*/
        /*if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
                GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
        } else {
                GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
        }*/

        /*if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
                delay(DEBOUNCE_DELAY);
                if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
                    greenLed.GPIO_PinMode = GPIO_MODE_INPUT;
                    GPIO_Init(GPIOG, greenLed);
                }
                
            }*/
        /*Receive message*/
        USART_Receive(USART3, (uint8_t *) &ReceivedMess, ReceivedMessSize);
        /*Echo back the received message*/
        USART_Transmit(USART3, (uint8_t *) &ReceivedMess, ReceivedMessSize);
        /*Check if the received message is "ON_"*/
        if (strcmp((const char *)ReceivedMess, "ON_") == 0) {
            /*Turn green LED ON*/
            GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
        }
        /*Check is the received message is "OFF"*/
        if (strcmp((const char *)ReceivedMess, "OFF") == 0) {
            /*Turn green LED OFF*/
            GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
        }
    }

    return 0;
}

// void EXTI0_IRQHandler(void) {
//     /*is the corresponding bit in the EXTI_PR register set?*/
//     if ((EXTI->PR >> pushButton.GPIO_PinNumber) & 0x01U) {
//         /*clear the bit by writing 1*/
//         EXTI->PR |= (0x01U << pushButton.GPIO_PinNumber);
//     }
//     delay(DEBOUNCE_DELAY);
//     if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) { /*check state again*/
//         GPIO_ToggleLed(GPIOG, GPIO_PIN_NUM_13);
//     }
// }