#include <msp430.h>
#include "spi.h"
#include "RestoreState/RestoreState.h"

#define BIT_SPI_CS	BIT3
#define BIT_RESET	BIT1
#define BIT_DIO_0	BIT0

void init_spi(void) {
//                   MSP430FR6047
//                 -----------------
//                |                 |
//                |             P7.0|-> Data Out (UCA2SIMO)
//                |                 |
//                |             P7.1|<- Data In (UCA2SOMI)
//                |                 |
//                |             P7.2|-> Serial Clock Out (UCA2CLK)
//                |                 |
//                |             P7.3|-> CS
//                |                 |
//                |             P8.1|-> RESET
//                |                 |
//                |             P8.0|<-> DIO_0
//                |                 |

    P7OUT |= BIT3;
    P7DIR |= BIT3;

    P7SEL0 |= BIT0 | BIT1 | BIT2;
    P7SEL1 &= ~(BIT0 | BIT1 | BIT2);

    //Clock Polarity: The inactive state is high
    //MSB First, 8-bit, Master, 3-pin mode, Synchronous

    UCA2CTLW0 = UCSWRST;   // **Put state machine in reset**
    UCA2CTLW0 |= UCCKPL | UCMSB | UCSYNC | UCMST | UCSSEL__SMCLK;

    UCA2BRW = 10;
    //UCB0MCTLW = 0;
    UCA2CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
    //UCA2IE |= UCRXIE;                          // Enable USCI0 RX interrupt
}

/*
 * Set a register over the SPI interface
 */
void spi_snd_data(uint8_t reg_add, uint8_t data)
{
    Save(reg_add, data);            // Faraz - added function to save the registers being sent to the LoRa Transceiver
	P7OUT &= ~BIT_SPI_CS;
	__delay_cycles(20);
	UCA2TXBUF = 0x80u | reg_add;
	while (UCA2STATW & UCBUSY);
	UCA2TXBUF = data;
	while (UCA2STATW & UCBUSY);
	P7OUT |= BIT_SPI_CS;
}

/*
 * Read a register value over the SPI interface
 */
uint8_t spi_rcv_data(uint8_t reg_add)
{

	P7OUT &= ~BIT_SPI_CS;
	__delay_cycles(20);

	UCA2TXBUF = reg_add;
	while (UCA2STATW & UCBUSY);

	UCA2TXBUF = 0x00;
	while (UCA2STATW & UCBUSY);


	P7OUT |= BIT_SPI_CS;
	//return r; //SH
	return UCA2RXBUF;
}
