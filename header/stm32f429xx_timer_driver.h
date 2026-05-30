#ifndef STM32F429XX_TIMER_DRIVER_H
#define STM32F429XX_TIMER_DRIVER_H
#include "stm32f429xx.h"

/*Timer base unit configuration structure*/
typedef struct {
    uint32_t period;            /*Specifies the period value to be loaded into the active
                                Auto-Reload Register at the next update event.
                                This parameter can be a number between Min_Data = 0x00000000 and Max_Data = 0xFFFFFFFF*/
    uint16_t Prescaler;         /*Specifies the prescaler value of the timer base unit,
                                This parameter can be a number between Minval = 0x0000 and Maxval = 0xFFFF*/
    uint8_t AutoReloadPreload;  /*Specifies the auto reload Preload,
                                This parameter can be a value of @ref TIM_AutoReloadPreload*/
	uint8_t CounterMode;		/*Specifies the timer counting mode,
								This parameter can be a value of @ref TIM_CounterMode*/
}TIM_Base_Conf_t;

/*Output comapre configuration struct*/
typedef struct {
	uint32_t Pulse;     /*Specifies the pulse value to be loaded into the capture compare register
						This parameter can be a number between Min_Data = 0x00000000 and Max_Data = 0xFFFFFFFF*/
	uint8_t OCMode;     /*Specifies TIM mode
						This parameter can a value of @ref TIM_Output_Compare_and_PWM_modes*/
	uint8_t OCPolarity; /*Specifies the output polarity
						This parameter can be a value of @ref TIM_Output_Compare_Polarity*/
}TIM_OC_Conf_t;

/*TIM_AutoReloadPreload*/
#define TIM_AUTORELOAD_PRELAOD_DISABLE 0U /*TIMx_ARR register is not a buffer*/
#define TIM_AUTORELOAD_PRELAOD_ENABLE  1U /*TIMx_ARR register is a buffer*/

/*TIM_CounterMode*/
#define TIM_UPCOUNTING   0U /*TIMx upcounting mode selection*/
#define TIM_DOWNCOUNTING 1U /*TIMx downcounting mode selection*/

/*TIM_Output_Compare_and_PWM_modes*/
#define TIM_OCMODE_TIMING            0U  /*Frozen - The comparison between the output compare register  TIMx_CCRx_and the 
								         counter TIMx_CNT has no effect on the outputs.(this mode is used to generate a timing  base)*/
#define TIM_OCMODE_ACTIVE            1U  /*Set channel x to active level on match. OCxREF signal is forced high when the counter 
								         TIMx_CNT matches the capture/compare register x (TIMx_CCRx)*/
#define TIM_OCMODE_INACTIVE          2U  /*Set channel x to inactive level on match. OCx_REF signal is forced low when the 
								         counter TIMx_CNT matches the capture/compare register x (TIMx_CCRx)*/
#define TIM_OCMODE_TOGGLE            3U  /*Toggle - OCx_REF toggles when TIMx_CNT = TIMx_CCRx*/
#define TIM_OCMODE_FORCED_INACTIVE   4U  /*Force inactive level - OCxREF is forced low*/
#define TIM_OCMODE_FORCED_ACTIVE     5U  /*Force inactive level - OCxREF is forced high)*/
#define TIM_OCMODE_PWM1              6U  /*PWM mode 1 - In upcounting, channel x is active as long as TIMx_CNT < TIMxCCRx 
								         else inactive. In downcounting, channel x is inactive (OCx_REF = 0) as long as
										TIMx_CNT > TIMx_CCRx else active (OCx_REF = 1)*/
#define TIM_OCMODE_PWM2              7U  /*PWM mode 2 - In upcounting, channel x is inactive as long as TIMx_CNT < TIMxCCRx 
								         else active. In downcounting, channel x is active as long as TIMx_CNT > TIMx_CCRx 
										 else inactive*/

/*TIM_Output__Compare_Polarity*/
#define TIM_OCPOLARITY_HIGH 0U  /*OC ative high*/
#define TIM_OCPOLARITY_LOW 1U   /*OC ative low*/

#define TIM_OC_CHANNEL_4 3U /*Output comapre channel 4*/


/*TIMx CR1 register bits*/
#define TIM_CR1_ARPE 7U    /*ARPE: Auto-reload preload enable bit*/
#define TIM_CR1_CEN  0U    /*CEN: Counter enable bit*/
#define TIM_CR1_DIR  4    /*DIR: Direction*/

/*TIMx CR1 register bits*/
#define TIM_EGR_UG   0U    /*EGR: Update generation bit*/

/*TIMx SR register bits*/
#define TIM_SR_UIF   0U    /*UIF: Update interrupt flag bit*/

/*TIMx DIER register bit*/
#define TIM_DIER_UIE 0U    /*UIE: Enable update interrupt bit*/

/*Macros handle update event status*/
#define TIM6_UEV_STS()     ((TIM6->SR >> TIM_SR_UIF) & 0x01U)
#define TIM6_UEV_STS_CRL() (TIM6->SR &= ~(0x01U <<TIM_SR_UIF))

/*Macro handle output compare*/
#define TIM4_OC_PWM_SET_DUTY(channel, CCR_value) (TIM4->CCR[(channel)] = (CCR_value))

/*Macro to map IRQn to TIMx*/
#define TIMx_TO_IRQ(TIMx) \
        ((TIMx == TIM6) ? IRQ_NO_TIM6_DAC : \
        (TIMx == TIM7) ? IRQ_NO_TIM7 : IRQ_NO_TIM6_DAC)

void TIM_Base_Init(TIM_RegDef_t *TIMx, TIM_Base_Conf_t TIM_BaseConf);
void TIM_Base_Start(TIM_RegDef_t *TIMx);
void TIM_Base_Stop(TIM_RegDef_t *TIMx);
void TIM_Base_IT_Init(TIM_RegDef_t *TIMx, uint8_t Priority);
void TIM_OC_Init(TIM_RegDef_t *TIMx, TIM_OC_Conf_t TIM_OCConf, uint8_t Channel);

#endif //STM32F429XX_TIMER_DRIVER_H