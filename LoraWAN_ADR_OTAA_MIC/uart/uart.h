/*
 * uart.h
 *
 *  Created on: 9 Aug 2022
 *      Author: mirko
 */

#ifndef UART_UART_H_
#define UART_UART_H_

#include <msp430.h>
#include <stdint.h>


#define UART_TX_READY   (UCA0IFG & UCTXIFG)
#define UART_RX_READY   (UCA0IFG & UCRXIFG)
#define UART_TX_DONE    (UCA0IFG & UCTXCPTIFG)
#define UART_RESET_TX_DONE  (UCA0IFG &= ~UCTXCPTIFG)


void uart_send_char(char c);

void uart_send_string(uint8_t *str, int length);

void Init_UART(void);

#endif /* UART_UART_H_ */
