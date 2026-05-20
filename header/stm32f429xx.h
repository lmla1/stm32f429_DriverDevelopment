#ifndef STM32F429XX_H
#define STM32F429XX_H

#include <stdint.h>

/*GPIO register definition struct*/
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
}GPIO_RefDef_t;

/*RCC register definition struct*/
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
             uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
             uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
             uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
             uint32_t RESERVED3[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB3LPENR;
             uint32_t RESERVED4;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
             uint32_t RESERVED5[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
             uint32_t RESERVED6[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;

}RCC_RegDef_t;

/*SYSCFG register definition struct*/
typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t CMPCR;
}SYSCFG_RegDef_t;

/*EXTI register definition struct*/
typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
}EXTI_RegDef_t;

/*AHB1 base address*/
#define AHB1_BASEADDR (0x40020000UL)
/*APB2 base address*/
#define APB2_BASEADDR (0x40010000UL)
#define GPIOA ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x0000UL))   /*GPIOA base address*/
#define GPIOB ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x0400UL))   /*GPIOB base address*/
#define GPIOC ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x0800UL))   /*GPIOC base address*/
#define GPIOD ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x0C00UL))   /*GPIOD base address*/
#define GPIOE ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x1000UL))   /*GPIOE base address*/
#define GPIOF ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x1400UL))   /*GPIOF base address*/
#define GPIOG ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x1800UL))   /*GPIOG base address*/
#define GPIOH ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x1C00UL))   /*GPIOH base address*/
#define GPIOI ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x2000UL))   /*GPIOI base address*/
#define GPIOJ ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x2400UL))   /*GPIOJ base address*/
#define GPIOK ((GPIO_RefDef_t *) (AHB1_BASEADDR + 0x2800UL))   /*GPIOK base address*/

/*RCC base address*/
#define RCC ((RCC_RegDef_t *) (AHB1_BASEADDR + 0x3800UL))
/*SYSCONF base address*/
#define SYSCFG ((SYSCFG_RegDef_t *) (APB2_BASEADDR + 0x3800UL))
/*exti base address*/
#define EXTI ((EXTI_RegDef_t *) (APB2_BASEADDR + 0x3C00UL))
/*GPIO clock enable*/
#define GPIOA_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 0U))   /*GPIOA peripheral clock enable*/
#define GPIOB_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 1U))   /*GPIOB peripheral clock enable*/
#define GPIOC_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 2U))   /*GPIOC peripheral clock enable*/
#define GPIOD_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 3U))   /*GPIOD peripheral clock enable*/
#define GPIOE_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 4U))   /*GPIOE peripheral clock enable*/
#define GPIOF_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 5U))   /*GPIOF peripheral clock enable*/
#define GPIOG_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 6U))   /*GPIOG peripheral clock enable*/
#define GPIOH_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 7U))   /*GPIOH peripheral clock enable*/
#define GPIOI_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 8U))   /*GPIOI peripheral clock enable*/
#define GPIOJ_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 9U))   /*GPIOJ peripheral clock enable*/
#define GPIOK_CLK_ENB() (RCC->AHB1ENR |= (0x01 << 10U))  /*GPIOK peripheral clock enable*/
/*GPIO clock disable*/
#define GPIOA_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 0U))  /*GPIOA peripheral clock disable*/
#define GPIOB_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 1U))  /*GPIOB peripheral clock disable*/
#define GPIOC_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 2U))  /*GPIOC peripheral clock disable*/
#define GPIOD_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 3U))  /*GPIOD peripheral clock disable*/
#define GPIOE_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 4U))  /*GPIOE peripheral clock disable*/
#define GPIOF_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 5U))  /*GPIOF peripheral clock disable*/
#define GPIOG_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 6U))  /*GPIOG peripheral clock disable*/
#define GPIOH_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 7U))  /*GPIOH peripheral clock disable*/
#define GPIOI_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 8U))  /*GPIOI peripheral clock disable*/
#define GPIOJ_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 9U))  /*GPIOJ peripheral clock disable*/
#define GPIOK_CLK_DIS() (RCC->AHB1ENR &= ~(0x01 << 10U)) /*GPIOK peripheral clock disable*/

/*APB2 Peripheral clock enable*/
#define TIM1_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 1U))   /*TIM1 peripheral clock enable*/
#define TIM8_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 2U))   /*TIM8 peripheral clock enable*/
#define USART1_CLK_ENB() (RCC->APB2ENR |= (0x01 << 4U))   /*USART1 peripheral clock enable*/
#define USART6_CLK_ENB() (RCC->APB2ENR |= (0x01 << 5U))   /*USART6 peripheral clock enable*/
#define ADC1_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 8U))   /*ADC1 peripheral clock enable*/
#define ADC2_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 9U))   /*ADC2 peripheral clock enable*/
#define ADC3_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 10U))  /*ADC3 peripheral clock enable*/
#define SDIO_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 11U))  /*SDIO peripheral clock enable*/
#define SPI1_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 12U))  /*SPI1 peripheral clock enable*/
#define SYSCFG_CLK_ENB() (RCC->APB2ENR |= (0x01 << 14U))  /*SYSCFG peripheral clock enable*/
#define TIM9_CLK_ENB()   (RCC->APB2ENR |= (0x01 << 16U))  /*TIM9 peripheral clock enable*/
#define TIM10_CLK_ENB()  (RCC->APB2ENR |= (0x01 << 17U))  /*TIM10 peripheral clock enable*/
#define TIM11_CLK_ENB()  (RCC->APB2ENR |= (0x01 << 18U))  /*TIM11 peripheral clock enable*/

/*APB2 Peripheral clock disable*/
#define TIM1_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 1U))   /*TIM1 peripheral clock disable*/
#define TIM8_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 2U))   /*TIM8 peripheral clock disable*/
#define USART1_CLK_DIS() (RCC->APB2ENR &= ~(0x01 << 4U))   /*USART1 peripheral clock disable*/
#define USART6_CLK_DIS() (RCC->APB2ENR &= ~(0x01 << 5U))   /*USART6 peripheral clock disable*/
#define ADC1_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 8U))   /*ADC1 peripheral clock disable*/
#define ADC2_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 9U))   /*ADC2 peripheral clock disable*/
#define ADC3_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 10U))  /*ADC3 peripheral clock disable*/
#define SDIO_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 11U))  /*SDIO peripheral clock disable*/
#define SPI1_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 12U))  /*SPI1 peripheral clock disable*/
#define SYSCFG_CLK_DIS() (RCC->APB2ENR &= ~(0x01 << 14U))  /*SYSCFG peripheral clock disable*/
#define TIM9_CLK_DIS()   (RCC->APB2ENR &= ~(0x01 << 16U))  /*TIM9 peripheral clock disable*/
#define TIM10_CLK_DIS()  (RCC->APB2ENR &= ~(0x01 << 17U))  /*TIM10 peripheral clock disable*/
#define TIM11_CLK_DIS()  (RCC->APB2ENR &= ~(0x01 << 18U))  /*TIM11 peripheral clock disable*/

#include "stm32f429xx_gpio_driver.h"
#include "cortexM4.h"
#endif //STM32F429XX_H