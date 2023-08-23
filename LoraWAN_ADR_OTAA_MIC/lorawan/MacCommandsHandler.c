/*
 * MacCommandsParser.c
 *
 *  Created on: 17 Jul 2023
 *      Author: Mohd Firdaus Hirman Ritom
 */
#include <stdio.h>
#include "MacCommandsHandler.h"
#include "MacCommands.h"
#include <lorawan/Lorawan.h>

static void handleLinkCheckAns(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleLinkCheckAns\n");
}

static void handleLinkAdrReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleLinkAdrReq\n");
    uint8_t* dataPtr = frmPayload + CID_LEN;
    LinkADRReq_t adrReq = {0,0,0};
    uint8_t dataRate = 0; //Consist of both SF and BW
    uint8_t txPower = 0;

    //Extract data rate and txPower
    adrReq.dataRate_TXPower = *dataPtr;

    printf("handleLinkAdrReq: dataRate_TXPower = %02x\n", adrReq.dataRate_TXPower);

    dataRate = (adrReq.dataRate_TXPower >> 4) & 0x0F;
    loraCtx->txPower = adrReq.dataRate_TXPower & 0x0F;

    uint8_t r=0;
    uint8_t TxIn;
    if(loraCtx->txPower != 0x0f){           // f tells to keep as is
       r = spi_rcv_data(0x09);              //read output power register
       r = r & 0xf0;                        //should mask for previous settings
       TxIn=(0x0f-(loraCtx->txPower*2) + 1);    //look at equation in regional parameters and transceiver
       spi_snd_data(0x09,(r|TxIn));         //will load new Tx power
     }

    switch(dataRate)
    {
        case DR0_SF12_BW125:
        {
            lora_change_SpreadingFactor(12);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 12;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR1_SF11_BW125:
        {
            lora_change_SpreadingFactor(11);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 11;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR2_SF10_BW125:
        {
            lora_change_SpreadingFactor(10);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 10;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR3_SF9_BW125:
        {
            lora_change_SpreadingFactor(9);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 9;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR4_SF8_BW125:
        {
            lora_change_SpreadingFactor(8);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 8;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR5_SF7_BW125:
        {
            printf("handleLinkAdrReq: SUCCESS\n");
            lora_change_SpreadingFactor(7);
            lora_set_signal_bandwidth(LORA_BW_125_KHZ);

            loraCtx->spreadFactor = 7;
            loraCtx->bw = LORA_BW_125_KHZ;
        }
        break;
        case DR6_SF7_BW250:
        {
            lora_change_SpreadingFactor(7);
            lora_set_signal_bandwidth(LORA_BW_250_KHZ);

            loraCtx->spreadFactor = 7;
            loraCtx->bw = LORA_BW_250_KHZ;
        }
        break;
        default:
        {
            printf("handleLinkAdrReq: Error: Unhandled data rate!!\n");
        }
    }

    printf("handleLinkAdrReq: loraCtx->spreadFactor = %02x\n", loraCtx->spreadFactor);
    printf("handleLinkAdrReq: loraCtx->bw = %02x\n", loraCtx->bw);
    printf("handleLinkAdrReq: loraCtx->txPower = %02x\n", loraCtx->txPower);

    //Todo: Handle ChMask and Redundancy

}

static void handleDutyCycleReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleDutyCycleReq\n");
}


static void handleRxParamSetupReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleRxParamSetupReq\n");
}


static void handleDevStatusReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleDevStatusReq\n");
}


static void handleNewChannelReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleNewChannelReq\n");
}


static void handleRxTimingSetupReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleRxTimingSetupReq\n");
}


static void handleTxParamSetupReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleTxParamSetupReq\n");
}

static void handleDIChannelReq(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleDIChannelReq\n");
}

static void handleDevTimeAns(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    printf("handleDevTimeAns\n");
}



void parseMacCommands(LORA_CTX_t* loraCtx ,uint8_t* frmPayload)
{
    int i;
    printf("parseMacCommands : frmPayload = ");
    for(i=0; i<51;i++)
    {
        printf("%02x ", frmPayload[i]);
    }
    printf("\n");

    printf("parseMacCommands : (begin) loraCtx->spreadfactor = %02x, loraCtx->txPower = %02x\n",loraCtx->spreadFactor, loraCtx->txPower);

    uint8_t* framePayloadPtr = frmPayload;

    do
    {
        if(*framePayloadPtr == LINK_CHECK_ANS)
        {
            handleLinkCheckAns(loraCtx, framePayloadPtr);
            framePayloadPtr += LINK_CHECK_ANS_LEN + 1;
        }
        else if(*framePayloadPtr == LINK_ADR_REQ)
        {
            handleLinkAdrReq(loraCtx, framePayloadPtr);
            framePayloadPtr += LINK_ADR_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == DUTY_CYCLE_REQ)
        {
            handleDutyCycleReq(loraCtx, framePayloadPtr);
            framePayloadPtr += DUTY_CYCLE_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == RX_PARAM_SETUP_REQ)
        {
            handleRxParamSetupReq(loraCtx, framePayloadPtr);
            framePayloadPtr += RX_PARAM_SETUP_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == DEV_STATUS_REQ)
        {
            handleDevStatusReq(loraCtx, framePayloadPtr);
            framePayloadPtr += DEV_STATUS_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == NEW_CHANNEL_REQ)
        {
            handleNewChannelReq(loraCtx, framePayloadPtr);
            framePayloadPtr += NEW_CHANNEL_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == RX_TIMING_SETUP_REQ)
        {
            handleRxTimingSetupReq(loraCtx, framePayloadPtr);
            framePayloadPtr += RX_TIMING_SETUP_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == TX_PARAM_SETUP_REQ)
        {
            handleTxParamSetupReq(loraCtx, framePayloadPtr);
            framePayloadPtr += TX_PARAM_SETUP_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == DI_CHANNEL_REQ)
        {
            handleDIChannelReq(loraCtx, framePayloadPtr);
            framePayloadPtr += DI_CHANNEL_REQ_LEN + 1;
        }
        else if(*framePayloadPtr == DEV_TIME_ANS)
        {
            handleDevTimeAns(loraCtx, framePayloadPtr);
            framePayloadPtr += DEV_TIME_ANS_LEN + 1;
        }
        else
        {
            //Not a MAC Command. Break from do-while loop
            printf("Not a MAC Command\n");
            break;
        }
    } while(1);
}
