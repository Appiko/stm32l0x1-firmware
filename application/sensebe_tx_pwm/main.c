
/* 
 * File:   main.c
 * Copyright (c) 2018 Appiko
 * Created on 5 December, 2018, 6:46 PM
 * Author:  Tejas Vasekar (https://github.com/tejas-tj)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE
 */

#include "boards.h"
#include "pwm.h"
#include "tinyprintf.h"
#include "uart_printf.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_exti.h"

typedef enum 
{
    SHORT,
    MEDIUM, 
    LONG,
    MAX_STATES
}dist;

static uint32_t dist_flag;

void random_delay (uint32_t delay_mul)
{
    for(uint32_t i = 0; i < delay_mul; i++)
        for(uint32_t j = 0; j < 256; j++)
        {
            __NOP ();
        }
}

void short_dist (void)
{
    tfp_printf("%s\n",__func__);
    LL_GPIO_SetOutputPin (LED_RED_PORT, LED_RED_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_RED_PORT, LED_RED_PIN);
    LL_GPIO_SetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
    LL_GPIO_ResetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
    LL_GPIO_ResetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);

//    LL_GPIO_ResetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);

}

void medium_dist (void)
{
    tfp_printf("%s\n",__func__);
    LL_GPIO_SetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    LL_GPIO_SetOutputPin (LED_RED_PORT, LED_RED_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_RED_PORT, LED_RED_PIN);
    LL_GPIO_ResetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    LL_GPIO_ResetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
    LL_GPIO_SetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
    LL_GPIO_ResetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);

//    LL_GPIO_ResetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);

}

void long_dist (void)
{
    tfp_printf("%s\n",__func__);
    LL_GPIO_SetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    LL_GPIO_SetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
    LL_GPIO_SetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
    LL_GPIO_SetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);

//    LL_GPIO_ResetOutputPin (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN);
//    LL_GPIO_ResetOutputPin (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN);


}
void sys_init (void)
{
    LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_PWR);

    /* System interrupt init*/
    /* SVC_IRQn interrupt configuration */
    NVIC_SetPriority (SVC_IRQn, 0);
    /* PendSV_IRQn interrupt configuration */
    NVIC_SetPriority (PendSV_IRQn, 0);
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority (SysTick_IRQn, 0);


    LL_FLASH_SetLatency (LL_FLASH_LATENCY_0);

    LL_PWR_SetRegulVoltageScaling (LL_PWR_REGU_VOLTAGE_SCALE3);

    LL_RCC_MSI_Enable ();

    /* Wait till MSI is ready */
    while (LL_RCC_MSI_IsReady () != 1)
    {

    }
    LL_RCC_MSI_SetRange (LL_RCC_MSIRANGE_6);

    LL_RCC_MSI_SetCalibTrimming (0);

    LL_RCC_SetAHBPrescaler (LL_RCC_SYSCLK_DIV_1);

    LL_RCC_SetAPB1Prescaler (LL_RCC_APB1_DIV_1);

    LL_RCC_SetAPB2Prescaler (LL_RCC_APB2_DIV_1);

    LL_RCC_SetSysClkSource (LL_RCC_SYS_CLKSOURCE_MSI);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource () != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
    {

    }
    LL_Init1msTick (4194000);

    LL_SYSTICK_SetClkSource (LL_SYSTICK_CLKSOURCE_HCLK);

    LL_SetSystemCoreClock (4194000);

    LL_RCC_SetUSARTClockSource (LL_RCC_USART2_CLKSOURCE_PCLK1);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority (SysTick_IRQn, 0);
    LL_IOP_GRP1_EnableClock (LL_IOP_GRP1_PERIPH_GPIOC);
    LL_IOP_GRP1_EnableClock (LL_IOP_GRP1_PERIPH_GPIOA);
    LL_IOP_GRP1_EnableClock (LL_IOP_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_TIM21);
    LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_TIM2);
}

void pin_init()
{
    LL_GPIO_SetPinMode (LED_GREEN_PORT, LED_GREEN_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode (LED_RED_PORT, LED_RED_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode (LEDIR_PWR1_PORT, LEDIR_PWR1_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetPinMode (LEDIR_PWR3_PORT, LEDIR_PWR3_PIN, LL_GPIO_MODE_OUTPUT);

    LL_GPIO_SetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_SetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_SetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    LL_GPIO_ResetOutputPin (LED_GREEN_PORT, LED_GREEN_PIN);
    random_delay (500);
    
}
void EXTI0_1_IRQHandler (void)
{
//    tfp_printf("%s\n",__func__);
    tfp_printf("Button Press detected..!!\n");
    while(LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN) == BUTTON_ACTIVE_STATE);

    dist_flag = (dist_flag+1)%MAX_STATES;
    switch ((dist)dist_flag)
    {
        case SHORT :
            short_dist ();
            break;
        case MEDIUM : 
            medium_dist ();
            break;
        case LONG : 
            long_dist ();
            break;
        case MAX_STATES :
            tfp_printf("Something went wrong\n");
    }
    LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_1);
    while(LL_EXTI_ReadFlag_0_31 (LL_EXTI_LINE_1));
    LL_PWR_EnterLowPowerRunMode ();
    LL_FLASH_DisableRunPowerDown ();
    LL_LPM_EnableSleepOnExit ();
}

void button_init ()
{
    LL_SYSCFG_SetEXTISource (LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);

    LL_GPIO_SetPinPull (BUTTON_PORT, BUTTON_PIN, LL_GPIO_PULL_UP);

    LL_GPIO_SetPinMode (BUTTON_PORT, BUTTON_PIN, LL_GPIO_MODE_INPUT);
    
    LL_EXTI_InitTypeDef EXTI_InitStruct ;
    
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_1;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_FALLING;
    LL_EXTI_Init (&EXTI_InitStruct);

    NVIC_SetPriority (EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ (EXTI0_1_IRQn);

    LL_EXTI_EnableIT_0_31 (LL_EXTI_LINE_1);
//    tfp_printf("Button Status : %ld\n", LL_EXTI_IsEnabledIT_0_31 (LL_EXTI_LINE_1));
}

int main ()
{
    sys_init ();
    uart_print_init ();

    tfp_printf ("Hello to new arch..!!\n");
    button_init ();
    pin_init ();


    dist_flag = SHORT;
    short_dist ();

    pwm_config_t pwm_tim21_init = {
        .tim = TIM21,
        .channel = LL_TIM_CHANNEL_CH1,
        .prescaler = 0,
        .duty_cycle = 10,
        .port = LEDIR_CTRL1_PORT,
        .pin_num = LEDIR_CTRL1_PIN,
        .alternate = LL_GPIO_AF_0,
    };
    pwm_set_counter (pwm_tim21_init.tim, 75);
    pwm_init (&pwm_tim21_init);
    pwm_start (pwm_tim21_init.tim, pwm_tim21_init.channel);
    
    pwm_config_t pwm_tim2_init = {
        .tim = TIM2,
        .channel = LL_TIM_CHANNEL_CH1,
        .prescaler = 19,
        .duty_cycle = 25,
        .port = LEDIR_CTRL2_PORT,
        .pin_num = LEDIR_CTRL2_PIN,
        .alternate = LL_GPIO_AF_5,
    };
    pwm_set_counter (pwm_tim2_init.tim, 4200);
    pwm_init (&pwm_tim2_init);
    pwm_start (pwm_tim2_init.tim, pwm_tim2_init.channel);

    LL_PWR_SetRegulModeLP (LL_PWR_REGU_LPMODES_LOW_POWER);
    LL_PWR_EnterLowPowerRunMode ();
    LL_LPM_EnableSleepOnExit ();
    LL_FLASH_DisableRunPowerDown ();

    while(1)
    {
        __WFI ();
    }
}