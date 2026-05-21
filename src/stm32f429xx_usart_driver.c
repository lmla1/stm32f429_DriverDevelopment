#include "stm32f429xx.h"

uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
uint16_t APB_PreScaler[4] = {2, 4, 8, 16};

/**
 * @brief This function gets PLL clock frequency.
 *        This is a dummy function and always returns 0.
 *        The implementation should be completed in when the PLL is used in the project.
 * 
 * @return uint32_t 
 */
uint32_t RCC_GetPLLOutputClock(void) {
    /*PLL  clock is not used in this application*/
    return 0;
}

/**
 * @brief This function is used to get APB1 clock frequency.
 *        Chage the value of HSI and HSE as respect to the MCU.
 * 
 * @return uint32_t 
 */
uint32_t RCC_GetPCLK1Val(void) {
    uint32_t P_Clk1, SYS_Clk;
    uint8_t Clk_Src, temp, AHB_Pre, APB1_Pre;

    /*Clock source in the MCU*/
    Clk_Src = (RCC->CFGR >> 2) & 0x03;

    switch (Clk_Src)
    {
        case 0: { /* HSI oscillator (16MHz) used as the system clock */
            SYS_Clk = 16000000;
            break;
        }
        case 1: { /* HSE oscillator (8MHz) used as the system clock */
            SYS_Clk = 8000000;
            break;
        }
        case 2: { /* PLL used as the system clock */
            SYS_Clk = RCC_GetPLLOutputClock();
            break;
        }
        default: { /* Not applicable */
            SYS_Clk = 0;
            break;
        }
    }

    /*get the AHB prescaler*/
    temp = (RCC->CFGR >> 4) & 0x0F;
    if (temp < 8) {
        /*System clock is not divided*/
        AHB_Pre = 1;
    } else {
        /*System clock is divided*/
        AHB_Pre = AHB_PreScaler[temp - 8];
    }

    /*get the APB1 prescaler*/
    temp = (RCC->CFGR >> 10) & 0x07;
    if (temp < 4) {
        /*System clock is not divided*/
        APB1_Pre = 1;
    } else {
        /*System clock is divided*/
        APB1_Pre = APB_PreScaler[temp - 4];
    }
    
    /*set peripheral clock*/
    P_Clk1 = (SYS_Clk / AHB_Pre) / APB1_Pre;
    return P_Clk1;
}

/**
 * @brief This function is used to get APB2 clock frequency.
 *        Chage the value of HSI and HSE as respect to the MCU.
 * 
 * @return uint32_t 
 */
uint32_t RCC_GetPCLK2Val(void) {
    uint32_t P_Clk2, SYS_Clk;
    uint8_t Clk_Src, temp, AHB_Pre, APB2_Pre;

    /*Clock source in the MCU*/
    Clk_Src = (RCC->CFGR >> 2) & 0x03;

    switch (Clk_Src)
    {
        case 0: { /* HSI oscillator (16MHz) used as the system clock */
            SYS_Clk = 16000000;
            break;
        }
        case 1: { /* HSE oscillator (8MHz) used as the system clock */
            SYS_Clk = 8000000;
            break;
        }
        case 2: { /* PLL used as the system clock */
            SYS_Clk = RCC_GetPLLOutputClock();
            break;
        }
        default: { /* Not applicable */
            SYS_Clk = 0;
            break;
        }
    }

    /*get the AHB prescaler*/
    temp = (RCC->CFGR >> 4) & 0x0F;
    if (temp < 8) {
        /*System clock is not divided*/
        AHB_Pre = 1;
    } else {
        /*System clock is divided*/
        AHB_Pre = AHB_PreScaler[temp - 8];
    }

    /*get the APB2 prescaler*/
    temp = (RCC->CFGR >> 13) & 0x07;
    if (temp < 4) {
        /*System clock is not divided*/
        APB2_Pre = 1;
    } else {
        /*System clock is divided*/
        APB2_Pre = APB_PreScaler[temp - 4];
    }
    
    /*set peripheral clock*/
    P_Clk2 = (SYS_Clk / AHB_Pre) / APB2_Pre;
    return P_Clk2;
}

/**
 * @brief This function initializes the USART peripheral according the the specified settings.
 * 
 * @param USARTx Pointer to the USART port to be configured (e.g, USART1, USART2).
 * @param USART_Conf Structer that contains the configuration information of the specified USART.
 */
void USART_Init(USART_RegDef_t *USARTx, USART_Conf_t USART_Conf) {
    uint32_t USARTDIV;
    uint32_t Mantissa, Fraction, Remainder, Scaling;
    uint32_t USARTx_Clk;
    /*set wordLength in CR1 register*/
    USARTx->CR1 |= (USART_Conf.WordLength << USART_CR1_M);
    /*set parity in CR1 register*/
    USARTx->CR1 &= ~(0x03U << USART_CR1_PS);
    USARTx->CR1 |= (USART_Conf.Parity << USART_CR1_PS);
    /*set USART mode in CR1 register*/
    USARTx->CR1 &= ~(0x03U << USART_CR1_RE);
    USARTx->CR1 |= (USART_Conf.Mode << USART_CR1_RE);
    /*set oversampling in CR1 register*/
    USARTx->CR1 |= (USART_Conf.OverSampling << USART_CR1_OVER8);
    /*set stop bit in CR2 register*/
    USARTx->CR2 &= ~(0x03U << USART_CR2_STOP);
    USARTx->CR2 |= (USART_Conf.StopBits << USART_CR2_STOP);
    /*set baudrate in BRR register*/
    if (USARTx == USART1 || USARTx == USART6) {
        USARTx_Clk = RCC_GetPCLK2Val();
    } else {
        USARTx_Clk = RCC_GetPCLK1Val();
    }
    Scaling = 8 * (2 - USART_Conf.OverSampling);
    USARTDIV = (USARTx_Clk / (Scaling * USART_Conf.BaudRate));
    Mantissa = USARTDIV;
    Remainder = (USARTx_Clk % (Scaling * USART_Conf.BaudRate));
    Fraction = ((Remainder) + (USART_Conf.BaudRate / 2)) / USART_Conf.BaudRate; /* Round(fraction) */
    USARTx->BRR &= ~(0xFFF0 << USART_DIV_MANTISSA);
    USARTx->BRR &= ~(0x000F << USART_DIV_FRACTION);
    USARTx->BRR |= (Mantissa << USART_DIV_MANTISSA);
    USARTx->BRR |= (Fraction << USART_DIV_FRACTION);
    /*enable the USART peripheral in CR1 register */
    USARTx->CR1 |= (ENABLE << USART_CR1_UE);
}

/**
 * @brief This function is used to transmit data via USART communication.
 * 
 * @note This implementation can only handle Asynchronous transmition.
 * 
 * @param USARTx Pointer to the USART port to be configured (e.g, USART1, USART2).
 * @param Mess Pointer to the message is going to be sent.
 * @param MessSize The size of the transmitted message.
 */
void USART_Transmit(USART_RegDef_t *USARTx, uint8_t *Mess, uint8_t MessSize) {
    uint8_t *pData8Bit;
    uint16_t *pData16Bit;
    uint8_t TxBufCounter;

    TxBufCounter = MessSize;
    /* Check the wordLength; If it is 9 bit wordLength,
    then use the 16bit data pointer to handle the transmission*/
    if ((((USARTx->CR1 >> USART_CR1_M) & 0x01) == USART_WORDLENGTH_9B)\
    && ((USARTx->CR1 >> USART_CR1_PS) & 0x01) == USART_PARITY_NONE) {
        /*9bit word length; None parity control*/
        pData8Bit = NULL;
        pData16Bit = (uint16_t *) Mess;
    } else {
        /*9bit word length; Parity control enabled*/
        /*8bit word length; Parity control enabled*/
        /*8bit word length; None parity control*/
        pData8Bit = Mess;
        pData16Bit = NULL;
    }
    /*loop through all characters in the message*/
    while (TxBufCounter > 0) {
        /*check if transmit data register empty*/
        if (((USARTx->SR >> USART_SR_TXE) & 0x01) == BIT_SET) {
            if (pData8Bit == NULL) {
                /*handle 9bit transmition*/
                USARTx->DR = (uint16_t) (*pData16Bit & 0x01FF);
                /*move the pointer to the next character*/
                pData16Bit++;
            } else {
                /*handle 8bit transmition*/
                USARTx->DR = (uint8_t) (*pData8Bit & 0x00FF);
                /*move the pointer to the next character*/
                pData8Bit++;
            }
            /*reduce the buffer counter by 1*/
            TxBufCounter--;
        }
    } 
}

/**
 * @brief This function is used to receive data from USART communication.
 * 
 * @note This implementation can only handle Asynchronous transmition.
 * 
 * @param USARTx Pointer to the USART port to be configured (e.g, USART1, USART2).
 * @param Mess Pointer to the message buffer that is used to store the received message.
 * @param MessSize The size of the received message.
 */
void USART_Receive(USART_RegDef_t *USARTx, uint8_t *Mess, uint8_t MessSize) {
    uint8_t *pData8Bit;
    uint16_t *pData16Bit;
    uint8_t RxBufCounter;

    RxBufCounter = MessSize;
    /* Check the wordLength; If it is 9 bit wordLength,
    then use the 16bit data pointer to handle the reception*/
    if ((((USARTx->CR1 >> USART_CR1_M) & 0x01) == USART_WORDLENGTH_9B)\
    && ((USARTx->CR1 >> USART_CR1_PS) & 0x01)== USART_PARITY_NONE) {
        /*9bit word length; None parity control*/
        pData8Bit = NULL;
        pData16Bit = (uint16_t *) Mess;
    } else {
        /*9bit word length; Parity control enabled*/
        /*8bit word length; Parity control enabled*/
        /*8bit word length; None parity control*/
        pData8Bit = Mess;
        pData16Bit = NULL;
    }
    /*loop through all characters in the message*/
    while (RxBufCounter > 0) {
        /*check if read data register empty*/
        if (((USARTx->SR >> USART_SR_RXNE) & 0x01) == BIT_SET) {
            if (pData8Bit == NULL) {
                /*handle 9bit reception*/
                *pData16Bit = (uint16_t) (USARTx->DR & 0x01FF);
                /*move the pointer to the next character*/
                pData16Bit++;
            } else {
                /*handle 8bit reception*/
                /*Is 9bit word length OR (8bit word lenght AND none parity)*/
                if (((USARTx->CR1 >> USART_CR1_M & 0x01) == USART_WORDLENGTH_9B)\
                || (((USARTx->CR1 >> USART_CR1_M & 0x01) == USART_WORDLENGTH_8B)
                && (((USARTx->CR1 >> USART_CR1_PS) & 0x01) == USART_PARITY_NONE))) {
                    /*handle 8bit reception*/
                    *pData8Bit = (uint8_t) (USARTx->DR & 0x00FF);
                    /*move the pointer to the next character*/
                    pData8Bit++;
                } else {
                    /*handle 8bit reception; Parity control enabled*/
                    *pData8Bit = (uint8_t) (USARTx->DR & 0x007F);
                    /*move the pointer to the next character*/
                    pData8Bit++;
                }
            }
            /*reduce the buffer counter by 1*/
            RxBufCounter--;
        }
    }
}