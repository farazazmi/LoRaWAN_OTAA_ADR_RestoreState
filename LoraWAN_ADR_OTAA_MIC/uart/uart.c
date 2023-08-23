/*
 * uart.c
 *
 *  Created on: 9 Aug 2022
 *      Author: mirko
 */

#include "uart.h"


void uart_send_char(char c)
{
    while (!(UART_TX_READY));
    UCA0TXBUF = c;
    while (!(UART_TX_DONE));
    UART_RESET_TX_DONE;
}


void uart_send_string(uint8_t *str, int length)
{
   unsigned i;
   for(i=0; i < length;i++)
   {
       uart_send_char((char)str[i]);
   }
   uart_send_char('\n');
}

void Init_UART(void){
    P2SEL0 &= ~(BIT0 | BIT1);
    P2SEL1 |= BIT0 | BIT1; // USCI_A0 UART operation
    // Configure USCI_A0 for UART mode
    UCA0CTLW0 = UCSWRST; // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // CLK = SMCLK

    UCA0BRW = 52;
    UCA0MCTLW = 0x4911;
    UCA0CTLW0 &= ~UCSWRST; // release from reset
    UCA0IE |= UCRXIE; // Enable USCI_A0 RX interrupt
}



