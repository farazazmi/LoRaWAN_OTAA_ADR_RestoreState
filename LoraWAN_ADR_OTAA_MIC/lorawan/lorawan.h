/*
 * loraWAN.h
 *
 *  Created on: 8 Dec 2022
 *      Author: Connor Bramwell
 */

#ifndef LORAWAN_LORAWAN_H_
#define LORAWAN_LORAWAN_H_
//#include <lorawan/sx1276_lora_csts1.h>
#include "lora/sx1276_lora_csts.h"
#include <msp430.h>
#include "spi/spi.h"
#include <lora/lora.h>
#include <lorawan/LoraCtx.h>
#include <lorawan/MacCommands.h>
#include <lorawan/MacCommandsHandler.h>
#include <lorawan/PhyPayloadParser.h>
#include <lorawan/MacPayloadParser.h>
#include <lorawan/MacPayloadFormat.h>
#include <lorawan/PhyPayloadFormat.h>
#include <lorawan/OTAAAcceptParser.h>

#define NUM_OF_16BYTES_BLOCKS(x) 1 + ((x - 1) >> 4)

#define OUTPUT_POWER  2 //(dBm) - Possible Input Range = [0 - 15] dBm
#define SF            7  //Possible Input Range [7 - 10]

#define LORA_BW_125_KHZ 0
#define LORA_BW_250_KHZ 1
#define LORA_BW_500_KHZ 2

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

#define NODE_ADDRESS  0x56u
#define BROADCAST_ADDRESS   0x00u
#define DIO0          ((P8IN & BIT0) != 0)


#define NODE_ADDRESS  0x56u


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#define POWER_LEVEL 17

//FCtrl (uplink)
#define FCTRL_ADR       0x80
#define FCTRL_ADRACKREQ 0x40
#define FCTRL_ACK       0x20
#define FCTRL_CLASS_B   0x10

//void ABP(u4_t netid, u4_t devAddr, u1_t nwkSKey, u1_t appSKey);//useless function as ABP does not have 'join' packet like OTAA does

//this function is used to build an unconfirmed LoRaWAN packet that should comply to LoRaWAN specs
uint8_t buildUnconfLoRaWANPacket(uint8_t *packet, uint32_t devAddr, uint32_t frameCounter, uint8_t FPort, uint8_t payload_size, uint8_t * payload, uint8_t * nwkskey, uint8_t * appskey);

//this function is used to build an OTAA join request LoRaWAN packet that should comply to LoRaWAN specs
uint8_t buildOTAALoRaWANPacket(uint8_t *packet, uint8_t *JoinEUI, uint8_t *DevEUI, uint16_t DevNonce, uint8_t *appKey);

//randomly select one of 3 frequencies
//void setJoinFreq(int select);//may have to be in LoRa as opposed to LoRaWAN file

//this function is to send the data using LoRaWAN
void loraWANSend(uint8_t *packet, uint8_t packetSize);


void encryptAESECB(uint8_t *key , uint8_t *plaintext, uint8_t *cyphertext);

void encryptAESCBC(uint8_t *key , uint8_t *plaintext, uint8_t *cyphertext);

void decryptAESECB(uint8_t *key , uint8_t *cyphertext , uint8_t *plaintext);

void loraWANreceive();

void initLoraCtx();
LORA_CTX_t* Lorawan_getLoraCtx();
uint8_t* Lorawan_getLoraPayload();

#endif /* LORAWAN_LORAWAN_H_ */
