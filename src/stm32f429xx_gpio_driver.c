#include "stm32f429xx.h"

/**
 * @brief This functioon initializes the GPIO peripheral to the specified settings.
 * 
 * @param GPIOx Pionter to the GPIO port to be configured. (e.g, GPIOA, GPIOB).
 * @param GPIOPinConf Structure that contains the configuration of a specified GPIO pin.
 * 
 * @return None
 */
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
            GPIOx->AFRH |= (GPIOPinConf.GPIO_AltFnc << (GPIOPinConf.GPIO_PinNumber - 8 ) * 4);
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

void GPIO_LockPinConf(GPIO_RefDef_t * GPIOx, uint8_t PinNumber) {
    uint32_t temp1 = (0x01U << 16U);
    uint32_t temp2 = (0x01U << PinNumber);

    temp1 |= temp2;
    /*WR LCKR[16] = ‘1’ + LCKR[15:0]*/
    GPIOx->LCKR = temp1;
    /*WR LCKR[16] = ‘0’ + LCKR[15:0]*/
    GPIOx->LCKR = temp2;
    /*WR LCKR[16] = ‘1’ + LCKR[15:0]*/
    GPIOx->LCKR = temp1;
    /*RD LCKR*/
    temp1 = GPIOx->LCKR;
}

/**
 * @brief Initializes the interrupt for the specified pin
 *        including EXTI and NVIC settings
 * 
 * @param GPIOx Pionter to the GPIO port to be configured. (e.g, GPIOA, GPIOB).
 * @param GPIOPinConf Structure that contains the configuration of a specified GPIO pin.
 * @param Priority Interrupt priority of the selected pin.
 */
void GPIO_IT_Init(GPIO_RefDef_t * GPIOx, GPIO_PinConf_t GPIOPinConf, uint8_t Priority) {
    uint8_t index, bitpos, portcode;
    /*configure the SYSCFG*/
    /*enable the SYSCFG clock*/
    SYSCFG_CLK_ENB();
    /*select the souce (GPIO pin) for the respective EXTI line*/
    /*specify the SYSCFG_EXTICR register index*/
    index = GPIOPinConf.GPIO_PinNumber / 4;
    /*specify the SYSCFG_EXTICR bit position*/
    bitpos = (GPIOPinConf.GPIO_PinNumber % 4) * 4;
    /*specify the GPIO port to be mapped in SYSCFG_EXTICR*/
    portcode = SYSCFG_EXTI_PORTCODE(GPIOx);
    SYSCFG->EXTICR[index] &= ~(0x0F << bitpos);
    SYSCFG->EXTICR[index] |= (portcode << bitpos); 
    /*configure the EXTI*/
    /*select the edge trigger for the interrupt*/
    switch (GPIOPinConf.GPIO_EdgeTrigger)
    {
        case GPIO_IT_EDGE_FT: {
            /*disable the rising edge trigger selection*/
            EXTI->RTSR &= ~(0x01U << GPIOPinConf.GPIO_PinNumber);
            /*enable the falling edge trigger selection*/
            EXTI->FTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }
        case GPIO_IT_EDGE_RT: {
            /*disable the falling edge trigger selection*/
            EXTI->FTSR &= ~(0x01U << GPIOPinConf.GPIO_PinNumber);
            /*enable the rising edge trigger selection*/
            EXTI->RTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }    
        default: {
            /*enable the falling edge trigger selection*/
            EXTI->FTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            /*enable the rising edge trigger selection*/
            EXTI->RTSR |= (0x01U << GPIOPinConf.GPIO_PinNumber);
            break;
        }
    }
    /*enable the interrupt mask for the respective EXTI line*/
    EXTI->IMR |= (0x01U << GPIOPinConf.GPIO_PinNumber);

    /*configure the NVIC*/
    /*select the interrupt priority*/
    NVIC_SetPriority(GPIO_PIN_TO_IRQ(GPIOPinConf.GPIO_PinNumber), Priority);
    /*enable interrupt request*/
    NVIC_EnableIRQ(GPIO_PIN_TO_IRQ(GPIOPinConf.GPIO_PinNumber));
}
