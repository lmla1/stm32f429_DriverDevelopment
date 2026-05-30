#include "stm32f429xx_timer_driver.h"

/**
 * @brief This function is used to initialize the timer base unit.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param TIM_BaseConf Structure that contains the configuration information of a specified TIMx.
 */
void TIM_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_Conf_t TIM_BaseConf) {
    /*Set the auto-reload preload*/
    TIMx->CR1 |= (TIM_BaseConf.AutoReloadPreload << TIM_CR1_ARPE); 
    /*Set the auto-relaod value*/
    TIMx->ARR = TIM_BaseConf.period;
    /*Set the prescaler value*/
    TIMx->PSC = TIM_BaseConf.Prescaler;
    /*Set timer counting mode*/
    TIMx->CR1 |= (TIM_BaseConf.CounterMode << TIM_CR1_DIR);
    /*Generate an update event to reload the period and presacaler value*/
    TIMx->EGR |= (0x01U << TIM_EGR_UG);
    /*Clear the update flag*/
    TIMx->SR &= ~(0x01U << TIM_SR_UIF);
}

/**
 * @brief This function starts the timer.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 */
void TIM_Base_Start(TIM_RegDef_t *TIMx) {
    /*Start the timer*/
    TIMx->CR1 |= (0x01U <<TIM_CR1_CEN);
}

/**
 * @brief This function stop the timer.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 */
void TIM_Base_Stop(TIM_RegDef_t *TIMx) {
    /*Stop the timer*/
    TIMx->CR1 &= ~(0x01U <<TIM_CR1_CEN);
}

/**
 * @brief This function initialized the timer base interrupt
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param Priority Interrupt priority to be set
 */
void TIM_Base_IT_Init(TIM_RegDef_t *TIMx, uint8_t Priority) {
    /*Set the interrupt priority for TIMx*/
    NVIC_SetPriority(TIMx_TO_IRQ(TIMx), Priority);
    /*Enable the IRQ of TIMx*/
    NVIC_EnableIRQ(TIMx_TO_IRQ(TIMx));
    /*Enable the TIMx update Interrupt*/
    TIMx->DIER |= (0x01U << TIM_DIER_UIE);
}

/**
 * @brief This function is used to initialize the output compare.
 * 
 * @param TIMx Pointer to the TIMx (e.g, TIM4, TIM6).
 * @param TIM_OCConf Structure that cointains the output compare configuration.
 * @param Channel Output compare channel to be configured.
 */
void TIM_OC_Init(TIM_RegDef_t *TIMx, TIM_OC_Conf_t TIM_OCConf, uint8_t Channel) {
    uint8_t CCMR_Reg_Index, CCMR_Bit_Selection;
    CCMR_Reg_Index = Channel / 2;
    CCMR_Bit_Selection = Channel % 2;

    /*set the output compare mode*/
    TIMx->CCMR[CCMR_Reg_Index] &= ~(TIM_OCConf.OCMode << (CCMR_Bit_Selection*8 + 4));
    TIMx->CCMR[CCMR_Reg_Index] |= (TIM_OCConf.OCMode << (CCMR_Bit_Selection*8 + 4));
    /*set output compare polarity*/
    TIMx->CCER |= (TIM_OCConf.OCPolarity << (Channel*4 + 1));
    /*set the pulse value*/
    TIMx->CCR[Channel] = TIM_OCConf.Pulse;
    /*enable the compare*/
    TIMx->CCER |= (ENABLE << Channel*4);
}