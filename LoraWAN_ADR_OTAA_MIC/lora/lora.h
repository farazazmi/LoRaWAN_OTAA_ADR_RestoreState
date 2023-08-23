/*
 * lora.h
 *
 *  Created on: 28 Jun 2022
 *      Author: mirko
 */

#ifndef LORA_LORA_H_
#define LORA_LORA_H_
#include <msp430.h>
#include "spi/spi.h"
#include "sx1276_lora_csts.h"

//#define OUTPUT_POWER  2 //(dBm) - Possible Input Range = [0 - 15] dBm
#define SF            7  //Possible Input Range [7 - 10]


// PA config
#define PA_BOOST                 0x80


// registers
#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_OCP                  0x0b
#define REG_LNA                  0x0c
#define REG_FIFO_ADDR_PTR        0x0d
#define REG_FIFO_TX_BASE_ADDR    0x0e
#define REG_FIFO_RX_BASE_ADDR    0x0f
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_PKT_SNR_VALUE        0x19
#define REG_PKT_RSSI_VALUE       0x1a
#define REG_RSSI_VALUE           0x1b
#define REG_MODEM_CONFIG_1       0x1d
#define REG_MODEM_CONFIG_2       0x1e
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MODEM_CONFIG_3       0x26
#define REG_FREQ_ERROR_MSB       0x28
#define REG_FREQ_ERROR_MID       0x29
#define REG_FREQ_ERROR_LSB       0x2a
#define REG_RSSI_WIDEBAND        0x2c
#define REG_DETECTION_OPTIMIZE   0x31
#define REG_INVERTIQ             0x33
#define REG_DETECTION_THRESHOLD  0x37
#define REG_SYNC_WORD            0x39
#define REG_INVERTIQ2            0x3b
#define REG_DIO_MAPPING_1        0x40
#define REG_VERSION              0x42
#define REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06


// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40
#define IRQ_RX_TIMEOUT_MASK        0x80

#define NODE_ADDRESS  0x56u
#define BROADCAST_ADDRESS   0x00u
#define DIO0          ((P8IN & BIT0) != 0)


#define NODE_ADDRESS  0x56u


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define POWER_LEVEL 17

//Signal Bandwidth RegModemConfig1(0x1D)
#define LORA_MODULE_BW_MASK        0xF0
#define LORA_MODULE_BW_7_8KHZ      0x00    //7.8 kHz
#define LORA_MODULE_BW_10_4KHZ     0x10    //10.4 kHz
#define LORA_MODULE_BW_15_6KHZ     0x20    //15.6 kHz
#define LORA_MODULE_BW_20_8KHZ     0x30    //20.8 kHz
#define LORA_MODULE_BW_31_25KHZ    0x40    //31.25 kHz
#define LORA_MODULE_BW_41_7KHZ     0x50    //41.7 kHz
#define LORA_MODULE_BW_62_5KHZ     0x60    //62.5 kHz
#define LORA_MODULE_BW_125KHZ      0x70    //125 kHz
#define LORA_MODULE_BW_250KHZ      0x80    //250 kHz
#define LORA_MODULE_BW_500KHZ      0x90    //500 kHz

/*
 * Initialize the LoRa RFM95 module
 */
void lora_begins();

void lora_begins1();//Connor- attempt at adapting transceiver initialisation from Arduino code

/*
 * Send a LoRa packet to a specific address
 */
void lora_send(uint8_t *data, uint8_t pkt_size);

/*
 *  Put LoRa module into Idle mode
 */
static void lora_idle();

/*
 * Put the LoRa module into Sleep mode
 */
void lora_sleep();

/*
 * Receive 8 bits of data
 */
uint8_t lora_receive(uint8_t *buffer);

/*
 * Change the Spreading Factor
 */
void lora_change_SpreadingFactor(uint8_t spreading_factor);

/*
 * Change the Spreading Factor
 */
uint8_t lora_get_SpreadingFactor();

/*
 * Set the LoRa module frequency (866mhz EU)
 */
static void lora_setFrequency(long frequency);

void lora_set_signal_bandwidth(uint8_t bw);


/*
 *  Generate a packet suitable for Lora 2 Lora transmission
 */
uint8_t packet_generator(uint8_t *packet, uint8_t header, uint8_t dev_id, uint8_t payload_size, uint8_t * payload);

//Connor- function to select one of the appropriate OTAA join frequencies
void setJoinFreq(int select);

/*
 * Configure RFM95W to prepare for single rx window
 */
void lora_singleRxWindowConfig();

void lora_startRx();

void lora_setTxPower(uint8_t txpower);

#endif /* LORA_LORA_H_ */
