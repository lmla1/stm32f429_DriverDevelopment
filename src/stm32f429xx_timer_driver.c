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
 * @brief 
 * 
 * @param TIMx 
 */
void TIM_Base_Stop(TIM_RegDef_t *TIMx) {
    /*Stop the timer*/
    TIMx->CR1 &= ~(0x01U <<TIM_CR1_CEN);
}