#include <stdint.h>
#include "stm32f429xx.h"

#define DELAY 1000000

void delay(uint32_t amount) {
    volatile uint32_t cnt = amount;
    while (cnt--);
}

int main() {
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
        .GPIO_PinNumber = GPIO_PIN_NUM_0,
        .GPIO_PinMode   = GPIO_MODE_INPUT,
        .GPIO_PUPD      = GPIO_NO_PUPD
    };

    GPIOA_CLK_ENB();
    GPIO_Init(GPIOA, pushButton);
    
    GPIOG_CLK_ENB();
    GPIO_Init(GPIOG, greenLed);
    GPIO_Init(GPIOG, redLed);

    while (1) {
        if (GPIO_ReadPin(GPIOA, GPIO_PIN_NUM_0)) {
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_HIGH);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_14, GPIO_PIN_LOW);
            delay(DELAY);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_13, GPIO_PIN_LOW);
            GPIO_WritePin(GPIOG, GPIO_PIN_NUM_14, GPIO_PIN_HIGH);
            delay(DELAY);
        }
    }

    return 0;
}