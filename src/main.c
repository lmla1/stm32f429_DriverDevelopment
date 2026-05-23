#include "stm32f429xx.h"
#include <string.h>

#define BLINK_DELAY    1000000
#define DEBOUNCE_DELAY 300000
#define BUTTON_DEBOUNCE_TIME 100U
#define RX_BUFFER_SIZE 8U
#define TX_BUFFER_SIZE 8U

volatile uint8_t ReceivedMess[RX_BUFFER_SIZE];
volatile uint8_t SendMessage[TX_BUFFER_SIZE] = "J\n";
volatile uint8_t TxMessSize    = 2U;
volatile uint8_t RxIndex       = 0U;
volatile uint8_t RxData        = 0U;
volatile uint8_t IsRxAvailable = FALSE;
volatile uint16_t Timer6DelayCounter = 0U;

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

TIM_Base_Conf_t TIM6_Conf = {
    .AutoReloadPreload = ENABLE,
    .period = 999,  /*1ms period*/
    .Prescaler = 15 /*counter clock is 1MHz (with 16MHz timer clock)*/
};

/**
 * @brief This function is used to start timer 6
 * 
 */
void TIM6_Start(void) {
    TIM_Base_Start(TIM6);
}

/**
 * @brief This function is used to stop timer 6
 * 
 */
void TIM6_Stop(void) {
    TIM_Base_Stop(TIM6);
}

/**
 * @brief This function is used to initialize timer 6 interrupt
 *        Priority is set to 1
 * 
 */
void TIM6_IT_Init(void) {
    uint8_t Priority = 1U;
    TIM_Base_IT_Init(TIM6, Priority);
}

int main() {
    //uint8_t ReceivedMess[4] = {0};
    //uint8_t ReceivedMessSize = 3U;

    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA, pushButton);
    GPIO_IT_Init(GPIOA, pushButton, 0U);
    
    GPIOG_CLK_ENB();
    GPIO_Init(GPIOG, greenLed);
    GPIO_Init(GPIOG, redLed);

    GPIOB_CLK_ENB();
    GPIO_Init(GPIOB, USART_Tx_Pin);
    GPIO_Init(GPIOB, USART_Rx_Pin);

    USART3_CLK_ENB();
    USART3_RXNEIE_ENB();                  /*enable receive not empty interrupt*/
    NVIC_SetPriority(IRQ_NO_USART3, 1U);  /*set USART3 interrupt priority*/
    NVIC_EnableIRQ(IRQ_NO_USART3);        /*enable USART3 IRQ*/
    USART_Init(USART3, USART3_Conf);

    TIM6_CLK_ENB();
    TIM_Base_Init(TIM6, TIM6_Conf);
    TIM6_Start();
    TIM6_IT_Init();

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
        //USART_Receive(USART3, (uint8_t *) &ReceivedMess, ReceivedMessSize);
        /*Echo back the received message*/
        //USART_Transmit(USART3, (uint8_t *) &ReceivedMess, ReceivedMessSize);
        /*Check if the received message is "ON_"*/
        // if (strcmp((const char *)ReceivedMess, "ON_") == 0) {
        //     /*Turn green LED ON*/
        //     GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
        // }
        /*Check is the received message is "OFF"*/
        // if (strcmp((const char *)ReceivedMess, "OFF") == 0) {
        //     /*Turn green LED OFF*/
        //     GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
        // }
        /*Is new data available*/
        if (IsRxAvailable) {
            /*Check overflow status and store data*/
            if (RxIndex < RX_BUFFER_SIZE) {
                /*store data to the received message*/
                ReceivedMess[RxIndex] = RxData;
                /*increase the index*/
                RxIndex++;
            } else {
                RxIndex = 0U; /*Overflow recovery*/
            }
            /*Reset the Rx data available flag to false*/
            IsRxAvailable = FALSE;

            /*Check if the message is fully received*/
            if (RxData == '\n') {
                /*Null-terminate the string/message*/
                ReceivedMess[RxIndex - 1] = '\0';
                /*check if the received message is "ON"*/
                if (strcmp((const char *)ReceivedMess, "ON") == 0) {
                    /*Turn green LED ON*/
                    GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
                }
                if (strcmp((const char *)ReceivedMess, "OFF") == 0) {
                    /*Turn green LED OFF*/
                    GPIO_WritePinBit(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
                }
                /*Reset the index*/
                RxIndex = 0U;
            }
        }
        // /*Check if update event generated*/
        // if (TIM6_UEV_STS() == BIT_SET) {
        //     /*Clear the update event status*/
        //     TIM6_UEV_STS_CRL();
        //     /*Increase the timer delay counter by 1*/
        //     Timer6DelayCounter++;
        //     /*Check if 1sec has elapsed*/
        //     if (Timer6DelayCounter == 1000) {
        //         /*Toggle the red LED*/
        //         GPIO_ToggleLed(GPIOG, GPIO_PIN_NUM_14);
        //         /*Reset timer 6 delay counter*/
        //         Timer6DelayCounter = 0U;
        //     }
        // }
    }
    return 0;
}

/**
 * @brief This is interrupt service routine for EXTI0
 * 
 */
void EXTI0_IRQHandler(void) {
    /*Start timer 6*/
    TIM6_Start();
    //delay(DEBOUNCE_DELAY);
    /*is the corresponding bit in the EXTI_PR register set?*/
    if ((EXTI->PR >> pushButton.GPIO_PinNumber) & 0x01U) {
        /*clear the pending bit by writing 1*/
        EXTI->PR |= (0x01U << pushButton.GPIO_PinNumber);   
    }
    // delay(DEBOUNCE_DELAY);
    // if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) { /*check state again*/
    //     GPIO_ToggleLed(GPIOG, GPIO_PIN_NUM_13);
    // }
    //USART_Transmit(USART3, (uint8_t *)SendMessage, TxMessSize);
}

/**
 * @brief This is interrupt service routine for USART3
 * 
 */
void USART3_IRQHandler(void) {
    /*check if the receive register is not empty*/
    if ((USART3->SR >> USART_SR_RXNE) & 0x01U) {
        /*read received data*/
        RxData = USART3->DR;
        /*set the Rx data available flag to TRUE*/
        IsRxAvailable = TRUE;
    }
}

/**
 * @brief This is interrupt service routine for Timer 6 and DAC
 * 
 */
void TIM6_DAC_IRQHandler(void) {
    /*Check if update event generated*/
    if (TIM6_UEV_STS() == BIT_SET) {
        /*Clear the update event status*/
        TIM6_UEV_STS_CRL();
        /*Increase the timer delay counter by 1*/
        Timer6DelayCounter++;
        /*Check if 1sec has elapsed*/
        if (Timer6DelayCounter == BUTTON_DEBOUNCE_TIME) {
            /*Transmi data*/
            USART_Transmit(USART3, (uint8_t *)SendMessage, TxMessSize);
            /*Reset timer 6 delay counter*/
            Timer6DelayCounter = 0U;
            /*Stop timer 6*/
            TIM6_Stop();
        }
    }
}