/*
 * MacPayloadParser.c
 *
 *  Created on: 18 Jul 2023
 *      Author: Mohd Firdaus Hirman Ritom, Faraz Azmi (added frame options parsing to parseMacPayload function)
 */
#include "MacPayloadParser.h"
#include "PhyPayloadFormat.h"

static void parseFHDR(LORA_CTX_t* loraCtx, uint8_t* macPayloadPtr)
{
    uint32_t devAddr = 0;
    uint32_t fcnt = 0;
    uint8_t fOptsLen = 0;

    //Get device address
    devAddr = (uint32_t)macPayloadPtr[0] | (uint32_t)macPayloadPtr[1] << 8 | (uint32_t)macPayloadPtr[2] << 16 | (uint32_t)macPayloadPtr[3] << 24; //little to big endian

    loraCtx->macFrameHeader.devAddr = devAddr;
    //Get frame control
    loraCtx->macFrameHeader.fCtrl = macPayloadPtr[4];

    //Get frame count
    //NOTE: ASSUMING NETWORK IS GIVING 2 BYTES OF FRAME COUNT!
    fcnt = (uint32_t)macPayloadPtr[5] | (uint32_t)macPayloadPtr[6] << 8;
    loraCtx->macFrameHeader.fCnt = fcnt;

    //Get frame fopts
    fOptsLen = loraCtx->macFrameHeader.fCtrl & MAC_FOPTS_MASK;

    if(fOptsLen != 0 )
    {
        memcpy(loraCtx->macFrameHeader.fopts,&macPayloadPtr[7],fOptsLen);
    }
}

void parseMacPayload(LORA_CTX_t* loraCtx)
{
    uint8_t fOptsLen = 0;
    uint8_t fhdrLen = 0;
    int i;

    if(!loraCtx)
        return;

    uint8_t* macPayload = loraCtx->macPayload;

    if(!macPayload)
        return;

    uint8_t* macPayloadPtr = macPayload;



    //FHDR
    parseFHDR(loraCtx,macPayloadPtr);

    //Fport
    fOptsLen = loraCtx->macFrameHeader.fCtrl & MAC_FOPTS_MASK;
    fhdrLen = MAC_DEVADDR_SIZE + MAC_FCTRL_SIZE + MAC_FCNT_SIZE + fOptsLen;

    if(fhdrLen == loraCtx->macPayloadSize)
    {
        //Faraz - There is no fPort, hence no frame payload , parsing frame options here for ADR MAC command for OTAA sessions
        printf("Frame Options: ");
        for(i=0;i<fhdrLen;i++)
        {
            printf("%02x ", loraCtx->macFrameHeader.fopts[i]);
        }
        printf("\n");
        parseMacCommands(loraCtx, &loraCtx->macFrameHeader.fopts[0]);
        return;
    }

    //Frame port
    macPayloadPtr += fhdrLen;
    loraCtx->macFramePort = *macPayloadPtr;

    //Frame Payload
    macPayloadPtr += MAC_FPORT_MAX_SIZE;
    loraCtx->framePayload = macPayloadPtr;
    loraCtx->framePayloadSize = loraCtx->loraPayloadSize - MHDR_SIZE - fhdrLen - MAC_FPORT_MAX_SIZE - MIC_SIZE;
}
