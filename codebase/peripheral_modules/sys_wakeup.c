/* 
 * 
#include "sys_wakeup.h"
#include "sys_wakeup.h"File:   sys_wakeup.c
 * Copyright (c) 2018 Appiko
 * Created on 26 October, 2018, 4:30 PM
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

#include "sys_wakeup.h"
#include "stm32l011xx.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_pwr.h"
#include "tinyprintf.h"
#include "stdint.h"
#include "log.h"

void (*handler_buffer_rtc) (void);
void (*handler_buffer_lptim) (void);

void RTC_IRQHandler (void)
{
//    log_printf("%s\n",__func__);
    LL_LPTIM_StartCounter (LPTIM1, LL_LPTIM_OPERATING_MODE_ONESHOT);
    LL_RTC_ClearFlag_WUT (RTC);
    LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_20);
    handler_buffer_rtc ();
}

void LPTIM1_IRQHandler (void)
{
//    log_printf("%s\n", __func__);
    handler_buffer_lptim ();
    LL_EXTI_ClearFlag_0_31 (LL_EXTI_LINE_29);
    LL_LPTIM_ClearFLAG_ARRM (LPTIM1);
    LL_PWR_EnableFastWakeUp ();
//    LL_RCC_EnableRTC ();

}

void wakeup_init(uint32_t sleep_duration_ms, void (* handler) (void))
{
    handler_buffer_rtc = handler;
    LL_RCC_DisableRTC ();
    LL_RTC_DisableWriteProtection (RTC);

    LL_RTC_WAKEUP_Disable (RTC);
    while(LL_RTC_WAKEUP_IsEnabled (RTC));

    LL_RTC_ClearFlag_WUT (RTC);

    LL_RTC_WAKEUP_SetClock (RTC, LL_RTC_WAKEUPCLOCK_DIV_16);
    LL_RTC_WAKEUP_SetAutoReload (RTC, sleep_duration_ms*2);
    LL_RTC_WAKEUP_Enable (RTC);
    LL_RTC_ClearFlag_WUT (RTC);
    LL_RTC_EnableIT_WUT (RTC);


    while(LL_RTC_WAKEUP_IsEnabled (RTC) == 0);
    LL_RTC_EnableWriteProtection (RTC);
    while(LL_RTC_IsActiveFlag_WUT (RTC));
    LL_RCC_EnableRTC ();
    NVIC_SetPriority (RTC_IRQn, 0);
    NVIC_EnableIRQ (RTC_IRQn);

    LL_EXTI_InitTypeDef rtc_exti_wkup_init =
    {
        .Line_0_31 = LL_EXTI_LINE_20,
        .LineCommand = ENABLE,
        .Mode = LL_EXTI_MODE_IT,
        .Trigger = LL_EXTI_TRIGGER_RISING,
    };
    LL_EXTI_Init (&rtc_exti_wkup_init);    
    /* RTC interrupt Init */

}

void sleep_init (uint32_t wake_duration_ms, void (*handler) (void))
{
    handler_buffer_lptim = handler;
    NVIC_SetPriority (LPTIM1_IRQn, 1);
    NVIC_EnableIRQ (LPTIM1_IRQn);
    LL_EXTI_InitTypeDef lptim_exti_wkup_init ={
        .Line_0_31 = LL_EXTI_LINE_29,
        .LineCommand = ENABLE,
        .Mode = LL_EXTI_MODE_IT,
        .Trigger = LL_EXTI_TRIGGER_RISING_FALLING,
    };
    LL_EXTI_Init (&lptim_exti_wkup_init);    

    LL_LPTIM_InitTypeDef lptim_init = 
    {
        .ClockSource = LL_LPTIM_CLK_SOURCE_INTERNAL,
        .Polarity = LL_LPTIM_CLK_POLARITY_RISING,
        .Prescaler = LL_LPTIM_PRESCALER_DIV16,
        .Waveform = LL_LPTIM_OUTPUT_WAVEFORM_SETONCE,
    };
    LL_LPTIM_ClearFLAG_ARRM (LPTIM1);
    LL_LPTIM_EnableIT_ARRM (LPTIM1);
    LL_LPTIM_Init (LPTIM1, &lptim_init);
    LL_LPTIM_Enable (LPTIM1);
    
    while(LL_LPTIM_IsActiveFlag_ARRM (LPTIM1));
    
    LL_LPTIM_SetAutoReload (LPTIM1, wake_duration_ms);

}