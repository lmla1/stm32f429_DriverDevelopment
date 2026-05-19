#include "stm32f429xx.h"

void GPIO_Init(GPIO_RefDef_t * GPIOx, GPIO_PinConf_t GPIOPinConf) {
    /*clear the current pin mode configuration*/
    GPIOx->MODER &= ~(0x03U << GPIOPinConf.GPIO_PinNumber * 2);
    /*set new pin mode configuration*/
    GPIOx->MODER |= (GPIOPinConf.GPIO_PinMode << GPIOPinConf.GPIO_PinNumber * 2);
    
    if (GPIOPinConf.GPIO_PinMode == GPIO_MODE_ALT || GPIOPinConf.GPIO_PinMode == GPIO_MODE_OUTPUT) {
        GPIOx->OTYPER &= ~(0x01U << GPIOPinConf.GPIO_PinNumber);
        GPIOx->OTYPER |= (GPIOPinConf.GPIO_OutType << GPIOPinConf.GPIO_PinNumber);
        GPIOx->OSPEEDR &= ~(0x03U << GPIOPinConf.GPIO_PinNumber * 2);
        GPIOx->OSPEEDR |= (GPIOPinConf.GPIO_OutSpeed << GPIOPinConf.GPIO_PinNumber * 2);
    }
    GPIOx->PUPDR &= ~(0x03U << GPIOPinConf.GPIO_PinNumber * 2);
    GPIOx->PUPDR |= (GPIOPinConf.GPIO_PUPD << GPIOPinConf.GPIO_PinNumber * 2);
    
    if (GPIOPinConf.GPIO_PinMode == GPIO_MODE_ALT) {
        if (GPIOPinConf.GPIO_PinNumber < 8) {
            GPIOx->AFRL &= ~(0x0FU << GPIOPinConf.GPIO_PinNumber * 4);
            GPIOx->AFRL |= (GPIOPinConf.GPIO_AltFnc << GPIOPinConf.GPIO_PinNumber * 4);
        } else {
            GPIOx->AFRH &= ~(0x0FU << GPIOPinConf.GPIO_PinNumber * 4);
            GPIOx->AFRH |= (GPIOPinConf.GPIO_AltFnc << GPIOPinConf.GPIO_PinNumber * 4);
        }
    }
}

void GPIO_WritePin(GPIO_RefDef_t * GPIOx, uint8_t PinNumber, GPIO_PinState_e PinState){
    if (PinState == GPIO_PIN_LOW) {
        GPIOx->ODR &= ~(0x01U << PinNumber);
    } else {
        GPIOx->ODR |= (0x01U << PinNumber);
    }
}

uint8_t GPIO_ReadPin(GPIO_RefDef_t * GPIOx, uint8_t PinNumber) {
    return (GPIOx->IDR >> PinNumber) & 0x01U;
}

void GPIO_ToggleLed(GPIO_RefDef_t * GPIOx, uint8_t PinNumber) {
    GPIOx->ODR ^= (0x01U << PinNumber);
}

void GPIO_WritePinBit(GPIO_RefDef_t * GPIOx, uint8_t PinNumber, GPIO_PinState_e PinState) {
    if (PinState == GPIO_PIN_LOW) {
        GPIOx->BSRR |= (0x01U << (PinNumber + 16U));
    } else {
        GPIOx->BSRR |= (0x01U << PinNumber);
    }
}