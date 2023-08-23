/*
 * LoraParser.c
 *
 *  Created on: 18 Jul 2023
 *      Author: Mohd Firdaus Hirman Ritom
 */
#include "PhyPayloadParser.h"
#include "PhyPayloadFormat.h"

static uint8_t checkMIC(uint8_t* phyPayload)
{
    if(!phyPayload)
        return MIC_NOT_OK;

    //TODO: Check MIC here
    //See section 4.4 of lorawan specification document

    return MIC_OK;
}

static void parseMHDR(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("parseMHDR\n");
}

void parsePhyPayload(LORA_CTX_t* loraCtx)
{
    if(!loraCtx)
        return;

    uint8_t* phyPayload = loraCtx->loraPayload;

    if(phyPayload)
    {
        uint8_t* payloadPtr = phyPayload;

        if(MIC_OK == checkMIC(phyPayload))
        {
            loraCtx->isMICOk = MIC_OK;

            //Parse Mac Header
            parseMHDR(loraCtx,phyPayload);

            //Make Lora Ctx know where is the mac payload located
            payloadPtr += MHDR_SIZE;
            loraCtx->macPayload = payloadPtr;
            loraCtx->macPayloadSize = loraCtx->loraPayloadSize - MHDR_SIZE - MIC_SIZE;
        }
        else
        {
            //Error: MIC not okay
            loraCtx->isMICOk = MIC_NOT_OK;
        }
    }
    else
    {
        //Error: Invalid phyPayload
    }
}
