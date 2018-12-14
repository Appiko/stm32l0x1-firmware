/* 
 * File:   main.c
 * Copyright (c) 2018 Appiko
 * Created on 13 December, 2018, 11:52 AM
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
#include "log.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_utils.h"

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
    LL_IOP_GRP1_EnableClock (LL_IOP_GRP1_PERIPH_GPIOA);
}

void EXTI0_1_IRQHandler (void)
{
//    log_printf ("%s\n", __func__);
    {
//        log_printf("Some Button is pressed..!!\n");
        if((LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
           (!LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)))
        {
            log_printf("Button1 is Pressed..!!\n");
            while((LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
           (!LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)));
        }
        if((!LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
           (LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)))
        {
            log_printf("Button2 is Pressed..!!\n");
            while((!LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
               (LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)));
        }
        if(!(LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
           !(LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)))
        {
            log_printf("Both buttons are Pressed..!!\n");
            while((!LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
               (!LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)));
        }
        
        if((LL_GPIO_IsInputPinSet (BUTTON_PORT, BUTTON_PIN)) & 
           (LL_GPIO_IsInputPinSet (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN)))
        {
            log_printf("Reset..!!\n");
            LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_0 );
            LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_1 );
        }
    }
}

void button_inits ()
{
    LL_GPIO_SetPinMode (BUTTON_PORT, BUTTON_PIN, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull (BUTTON_PORT, BUTTON_PIN, LL_GPIO_PULL_UP);
    LL_SYSCFG_SetEXTISource (LL_SYSCFG_EXTI_PORTA, LL_EXTI_LINE_1);
    LL_GPIO_SetPinMode (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull (LEDIR_PWR2_PORT, LEDIR_PWR2_PIN, LL_GPIO_PULL_UP);
    LL_SYSCFG_SetEXTISource (LL_SYSCFG_EXTI_PORTA, LL_EXTI_LINE_0);
    
    LL_EXTI_InitTypeDef exti_init = 
    {
        .Line_0_31 = LL_EXTI_LINE_0 | LL_EXTI_LINE_1,
        .LineCommand = ENABLE,
        .Mode = LL_EXTI_MODE_IT,
        .Trigger = LL_EXTI_TRIGGER_FALLING,
    };
    LL_EXTI_Init (&exti_init);

    NVIC_SetPriority (EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ (EXTI0_1_IRQn);
}

int main()
{
    
    sys_init ();
    log_init();
    log_printf("\nHello World..!!\n");
    button_inits ();
    while (1)
    {
        __WFI();
    }
}