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
    init_printf((void *) !(START_TX), printf_callback);
}
