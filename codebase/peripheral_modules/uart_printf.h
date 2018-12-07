/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uart_printf.h
 * Author: tejas_tj
 *
 * Created on 24 September, 2018, 3:58 PM
 */

#ifndef UART_PRINTF_H
#define UART_PRINTF_H
#include "tinyprintf.h"

void uart_print_init();

void print_char(uint8_t ch);

#endif /* UART_PRINTF_H */
