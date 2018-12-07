/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uart_printf.c
 * Author: tejas_tj
 * 
 * Created on 24 September, 2018, 3:58 PM
 */

#include "stm32l011xx.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_bus.h"
#include "uart_printf.h"

void print_char(uint8_t ch)
{
    LL_USART_IsActiveFlag_TC(USART2);
    {
        LL_USART_TransmitData8(USART2, ch);
        while((USART2->ISR & USART_ISR_TXE_Msk) != USART_ISR_TXE);
    }
    USART2->ICR |= USART_ICR_TCCF;
}

static void printf_callback(void * str_end, char ch)
{
    if((uint32_t)str_end != START_TX)
    {
        print_char (ch);    
    }
}

void uart_print_init ()
{
    LL_USART_InitTypeDef USART_InitStruct;

    LL_GPIO_InitTypeDef GPIO_InitStruct;
    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock (LL_APB1_GRP1_PERIPH_USART2);

    /**USART2 GPIO Configuration  
    PA2   ------> USART2_TX 
     */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init (GPIOA, &GPIO_InitStruct);

    USART_InitStruct.BaudRate = 115200;
    USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
    USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
    USART_InitStruct.Parity = LL_USART_PARITY_NONE;
    USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
    USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
    LL_USART_Init (USART2, &USART_InitStruct);

    LL_USART_ConfigHalfDuplexMode (USART2);

    LL_USART_Enable (USART2);

    init_printf((void *) !(START_TX), printf_callback);
}
