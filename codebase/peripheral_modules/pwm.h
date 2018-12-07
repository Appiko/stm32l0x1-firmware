/* 
 * File:   pwm.h
 * Copyright (c) 2018 Appiko
 * Created on 25 October, 2018, 5:54 PM
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


#ifndef PWM_H
#define PWM_H

#include "stm32l011xx.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_gpio.h"

typedef struct {
    /** GPIO Port for output pin */
    GPIO_TypeDef * port;
    /** GPIO Pin for output pin */
    uint32_t pin_num;
    /** GPIO Alternate function for output pin */
    uint32_t alternate;
    /** Timer used */
    TIM_TypeDef * tim;
    /** Channel used */
    uint32_t channel;
    /** Pre-scaler for Timer peripheral */
    uint32_t prescaler;
    /** Duty cycle for PWM */
    uint32_t duty_cycle;

}pwm_config_t;

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Function to set counter value for given timer.
 * @param tim Timer for which counter value is to be set.
 * @param autoreload Counter value which is to be set.
 */
void pwm_set_counter (TIM_TypeDef * tim, uint32_t autoreload);
/**
 * @brief Function to initiate PWM setup for given timer, pin and duty cycle. 
 * @param pwm_config Configuration required to setup PWM. See @ref pwm_config_t.
 * @note Before calling this function set counter by using @ref pwm_set_counter().
 */
void pwm_init (pwm_config_t * pwm_config);
/**
 * @breif Function to start the PWM at given channel and for given timer.
 * @param tim Timer for which PWM has to be enabled.
 * @param channel Channel at which PWM is supposed to be started.
 */
void pwm_start (TIM_TypeDef * tim, uint32_t channel);

/**
 * @breif Function to stop the PWM at given channel of given timer.
 * @param tim Timer for which PWM has to be disabled.
 * @param channel Channel for which PWM is supposed to be stopped.
 */
void pwm_stop (TIM_TypeDef * tim, uint32_t channel);
#ifdef __cplusplus
}
#endif

#endif /* PWM_H */

