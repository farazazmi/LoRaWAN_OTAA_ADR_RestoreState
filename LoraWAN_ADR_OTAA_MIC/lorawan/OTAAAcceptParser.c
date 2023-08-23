/*
 * OTAAAcceptParser.c
 *
 *  Created on: 2 Aug 2023
 *      Author: Faraz Azmi
 */

#include <lorawan/lorawan.h>
#include "lorawan/aes256.h"

//void parseOTAA_Accept(uint8_t AcceptPayload, uint16_t AcceptPayloadSize, uint8_t appKey)
void parseOTAA_Accept(uint8_t *appKey)
{
    //                                   1            16-32            4
    // Initially the Accept Payload has MHDR | Encrypted MACPayload | MIC

    LORA_CTX_t* loraCtx = Lorawan_getLoraCtx();     // From LoraWAN.h


    //uint8_t MacPayload[32] = {0x93, 0xfc, 0x83, 0x11, 0xae, 0x77, 0x6a, 0xf8, 0x88, 0xe0, 0x0c, 0x7b, 0x64, 0x86, 0xf9, 0xce, 0x14, 0x04, 0x79, 0x86, 0xb1, 0x0d, 0x1a, 0x60, 0xcc, 0xde, 0xe9, 0xec, 0x00, 0x00, 0x00, 0x00};
    uint8_t MacPayload[32] = {0};
    uint8_t EncMacPayload[16] = {0};
    uint8_t EncMacPayload1[16] = {0};
    uint8_t EncMacPayload2[16] = {0};
    uint8_t DecMacPayload[16] = {0};
    uint8_t DecMacPayload1[16] = {0};
    uint8_t DecMacPayload2[16] = {0};
    uint8_t FinalMacPayload[32] = {0};
    int i;
    int j;
    int k;
    int l;
    //uint8_t Si[16] = {0};


    for(i=1, j=0 ; i < (loraCtx->loraPayloadSize-4) ; i++, j++ )
    {
        MacPayload[j] = loraCtx->loraPayload[i];        // To extract just the Encrypted part, the MAC payload
    }

    /*
    printf("Encryption Key in Parser: ");
    for(i=0; i<16; i++)
    {
        printf("%02x ",appKey[i]);
    }
    printf("\n");
    */


    /*
    printf("Encrypted MAC Payload: ");
    for(i=0; i<32; i++)
    {
        printf("%02x ",MacPayload[i]);
    }
    printf("\n");
    */

    uint8_t blockNum = NUM_OF_16BYTES_BLOCKS(loraCtx->loraPayloadSize-5);   // No. of blocks for AES decryption, subtracting 5 from total payload size to get only the size of the Encrypted part of the payload
    //printf("No. of Blocks = %d \n", blockNum);

    AES256_setCipherKey(AES256_BASE, appKey, AES256_KEYLENGTH_128BIT);



    for(i=0;i<16;i++)
    {
        EncMacPayload1[i] = MacPayload[i];
    }

    for(i=0;i<16;i++)
    {
        EncMacPayload2[i] = MacPayload[i+16];
    }
    /*
    printf("MacPayload 1 input: ");
    for(i=0; i<16; i++)
    {
        printf("%02x ",EncMacPayload1[i]);
    }
    printf("\n");
    */
    AES256_encryptData(AES256_BASE, EncMacPayload1, DecMacPayload1);        // Using TI Library
    /*
    printf("DecMacPayload 1 output: ");
    for(i=0; i<16; i++)
    {
        printf("%02x ",DecMacPayload1[i]);
    }
    printf("\n");
    printf("\n");
    */
    //__delay_cycles(10000u);
    /*
    printf("MacPayload 2 input: ");
        for(i=0; i<16; i++)
        {
            printf("%02x ",EncMacPayload2[i]);
        }
        printf("\n");
        */
    AES256_encryptData(AES256_BASE, EncMacPayload2, DecMacPayload2);        // Using TI Library
    /*
    printf("DecMacPayload 2 output: ");
        for(i=0; i<16; i++)
        {
            printf("%02x ",DecMacPayload2[i]);
        }
        printf("\n");
        printf("\n");
        */
/*
    encryptAESECB(appKey,EncMacPayload1,DecMacPayload1);
    __delay_cycles(10000u);
    encryptAESECB(appKey,EncMacPayload2,DecMacPayload2);
*/

/*
    encryptAESECB(appKey,EncMacPayload2,DecMacPayload2);
    __delay_cycles(10000u);
    encryptAESECB(appKey,EncMacPayload1,DecMacPayload1);
*/

    //AES256_reset(AES256_BASE)
   // AES256_encryptData(AES256_BASE, EncMacPayload2, DecMacPayload2);

/*
    for(i=0; i<blockNum; i++)
    {
        for(j=0; j<16; j++)
        {
            EncMacPayload[j] = MacPayload[((i*16)+(j))];
        }

        encryptAESECB(appKey,EncMacPayload,DecMacPayload);  // Decrypting the MAC payload
        //AES256_encryptData(AES256_BASE, EncMacPayload, DecMacPayload);
        for(k=0; k<16; k++)
        {
            FinalMacPayload[((i*16)+(k))] = DecMacPayload[k];
        }
        printf("EncMacPayload: ");
        for(l=0; l<16; l++)
        {
            printf("%02x ",EncMacPayload[l]);
        }
        printf("\n");

    }
*/

/*
    printf("EncMacPayload 1: ");
    for(l=0; l<16; l++)
    {
        printf("%02x ",EncMacPayload1[l]);
    }
    printf("\n");

    printf("EncMacPayload 2: ");
    for(l=0; l<16; l++)
    {
        printf("%02x ",EncMacPayload2[l]);
    }
    printf("\n");
*/
    for(k=0; k<16; k++)
            {
                FinalMacPayload[k] = DecMacPayload1[k];
            }

    for(k=0; k<16; k++)
            {
                FinalMacPayload[k+16] = DecMacPayload2[k];
            }
    /*
    printf("Accept Payload: ");
    for(i=0; i<32; i++)
    {
        printf("%02x ",FinalMacPayload[i]);
    }
    printf("\n");
    */

    // Store DevAddr *******************************************************************************************************************
    uint32_t D1 = ((uint32_t)FinalMacPayload[9] << 24);
    uint32_t D2 = ((uint32_t)FinalMacPayload[8] << 16);
    uint32_t D3 = ((uint32_t)FinalMacPayload[7] << 8);
    uint32_t D4 = ((uint32_t)FinalMacPayload[6]);

    uint32_t DevAd = D1 + D2 + D3 + D4; //(uint32_t)((FinalMacPayload[9]) + ( FinalMacPayload[8] << 8) + ( FinalMacPayload[7] << 16) + ( FinalMacPayload[6] << 24));

    //printf("DevAddr = %08lx \n", DevAd);

    loraCtx->devAddr = DevAd;           // Send the Device Address to Global context to be saved and used everywhere


    // Now to generate the NwkSKey

    uint8_t Input[16] = {0};     // To store the input to be given to the AES encrypt function to derive the NwkSKey & AppSKey
    uint8_t NwkSKey[16] = {0};      // To store the final NwkSKey

    Input[0] = 0x01;             // First byte is a fixed 0x01 for the input, refer documentation

    // next 6 bytes are the JoinNonce and NetID which is the first 6 bytes of the decrypted Accept payload

    for (i=0;i<6;i++)
    {
        Input[i+1] =  FinalMacPayload[i];
    }

    // Next 2 bytes are the DevNonce for the Join Request, for now 0x00 0x00

    Input[7] = 0x00;
    Input[8] = 0x00;

    /*
    printf("NwkSKey Generator: ");         // Print Input to verify
    for(i=0; i<16; i++)
    {
        printf("%02x ",Input[i]);
    }
    printf("\n");
    */

    encryptAESECB(appKey,Input,NwkSKey);    // Encrypt the input to generate the NwkSKey

    for(i=0;i<16;i++)
    {
        loraCtx->nwkSKey[i] = NwkSKey[i];   // Send Key to Global context to be saved and used anywhere
    }

    /*
    printf("NwkSKey: ");         // Print NwkSKey to verify
        for(i=0; i<16; i++)
        {
            printf("%02x ",loraCtx->nwkSKey[i]);
        }
        printf("\n");
    */

    // Now to generate the AppSKey

    uint8_t AppSKey[16] = {0};

    Input[0] = 0x02;            // Input generator remains largely the same except first byte is changed to 0x02

    /*
    printf("AppSKey Generator: ");         // Print Input to verify
    for(i=0; i<16; i++)
    {
        printf("%02x ",Input[i]);
    }
    printf("\n");
    */

    encryptAESECB(appKey,Input,AppSKey);    // Encrypt the input to generate the NwkSKey

    for(i=0;i<16;i++)
        {
            loraCtx->appSKey[i] = AppSKey[i];   // Send Key to Global context to be saved and used anywhere
        }

    /*
    printf("AppSKey: ");         // Print AppSKey to verify
    for(i=0; i<16; i++)
        {
            printf("%02x ",loraCtx->appSKey[i]);
        }
    printf("\n");
    */




}

