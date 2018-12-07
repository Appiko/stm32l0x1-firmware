/* 
 * File:   pwm.c
 * Copyright (c) 2018 Appiko
 * Created on 25 October, 2018, 5:53 PM
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

#include "pwm.h"

#include "stm32l011xx.h"
#include "stm32l0xx.h"
#include "tinyprintf.h"
#include "stm32l0xx_ll_bus.h"

void pwm_set_counter (TIM_TypeDef * tim, uint32_t autoreload)
{
    LL_TIM_SetAutoReload (tim, autoreload);
//    tfp_printf("Auto Reload is set to : %ld\n", LL_TIM_GetAutoReload (tim));
}

void pwm_init (pwm_config_t * pwm_config)
{
    LL_TIM_InitTypeDef tim_init_strct =  {
        tim_init_strct.Prescaler = pwm_config->prescaler,
        tim_init_strct.CounterMode = LL_TIM_COUNTERMODE_UP,
        tim_init_strct.Autoreload = LL_TIM_GetAutoReload (pwm_config->tim),
        tim_init_strct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    };
    
    
    LL_TIM_OC_InitTypeDef tim_oc_init_strct = {
        tim_oc_init_strct.OCMode = LL_TIM_OCMODE_PWM1,
        tim_oc_init_strct.OCState = LL_TIM_OCSTATE_ENABLE,
        tim_oc_init_strct.CompareValue = (pwm_config->duty_cycle * tim_init_strct.Autoreload)/100,
        tim_oc_init_strct.OCPolarity = LL_TIM_OCPOLARITY_HIGH,
    };
//    tfp_printf("Autoreload and Pulse : %ld %ld\n", tim_init_strct.Autoreload, tim_oc_init_strct.CompareValue);

    LL_GPIO_InitTypeDef gpio_pin_cnf = {
        gpio_pin_cnf.Pin = pwm_config->pin_num,
        gpio_pin_cnf.Mode = LL_GPIO_MODE_ALTERNATE,
        gpio_pin_cnf.Speed = LL_GPIO_SPEED_FREQ_LOW,
        gpio_pin_cnf.OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        gpio_pin_cnf.Pull = LL_GPIO_PULL_NO,
        gpio_pin_cnf.Alternate = pwm_config->alternate,       
    };

    LL_GPIO_Init (pwm_config->port, &gpio_pin_cnf);

    LL_TIM_Init (pwm_config->tim, &tim_init_strct);
    LL_TIM_OC_EnablePreload(pwm_config->tim, pwm_config->channel);
    LL_TIM_OC_Init (pwm_config->tim, pwm_config->channel, &tim_oc_init_strct);

    LL_TIM_OC_DisableFast (pwm_config->tim, pwm_config->channel);

    LL_TIM_SetTriggerOutput (pwm_config->tim, LL_TIM_TRGO_RESET);

    LL_TIM_DisableMasterSlaveMode (pwm_config->tim);


}
void pwm_start (TIM_TypeDef * tim, uint32_t channel)
{
    if(tim == TIM2)
    {
        LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_TIM2);
    }
    else if(tim == TIM21)
    {
        LL_APB2_GRP1_EnableClock (LL_APB2_GRP1_PERIPH_TIM21);
    }

    LL_TIM_CC_EnableChannel (tim, channel);
    LL_TIM_EnableARRPreload (tim);
    LL_TIM_EnableCounter (tim);
//    tfp_printf("PWM Status : %ld\n", LL_TIM_IsEnabledCounter (tim));
}

void pwm_stop (TIM_TypeDef * tim, uint32_t channel)
{
    LL_TIM_CC_DisableChannel (tim, channel);
    LL_TIM_DisableCounter (tim);
    if(tim == TIM2)
    {
        LL_APB1_GRP1_DisableClock (LL_APB1_GRP1_PERIPH_TIM2);
    }
    else if(tim == TIM21)
    {
        LL_APB2_GRP1_DisableClock (LL_APB2_GRP1_PERIPH_TIM21);
    }
}