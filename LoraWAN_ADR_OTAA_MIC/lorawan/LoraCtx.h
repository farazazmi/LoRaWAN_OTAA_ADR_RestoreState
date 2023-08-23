/*
 * LoraCtx.h
 *
 *  Created on: 18 Jul 2023
 *      Author: Guest-PC
 */

#ifndef LORAWAN_LORACTX_H_
#define LORAWAN_LORACTX_H_

#include <stdio.h>

#include <Lorawan/MacPayloadFormat.h>

#define MAX_PAYLOAD_BUFFER 256 //The SX1276 or RFM95W limited to 256 bytes
#define FRM_PAYLOAD_SIZE_MAX 51

#define NWKSKEY_SIZE 16
#define APPSKEY_SIZE 16
#define APPKEY_SIZE 16

typedef struct LORA_CTX_t
{
    //Security
    uint8_t nwkSKey[NWKSKEY_SIZE];
    uint8_t appSKey[APPSKEY_SIZE];
    uint8_t appKey[APPKEY_SIZE];//connor - this was added for OTAA purposes
    uint16_t devNonce;          // Faraz - Added for OTAA request and accept purpose

    uint32_t devAddr;

    //The whole physical payload including MAC Header | MacPayload | MIC
    uint8_t loraPayload[MAX_PAYLOAD_BUFFER];
    uint16_t loraPayloadSize;

    uint8_t* macPayload; //This points the start of the macPayload along loraPayload
    uint8_t macPayloadSize;

    MAC_FHDR_t macFrameHeader;
    uint8_t macFramePort;

    uint8_t* framePayload; //This points the start of the frame payload along loraPayload
    uint8_t framePayloadSize;

    uint8_t isMICOk;

    //Tx Data Rate settings
    uint8_t spreadFactor;
    uint8_t bw;
    uint8_t txPower;
}LORA_CTX_t;



#endif /* LORAWAN_LORACTX_H_ */
