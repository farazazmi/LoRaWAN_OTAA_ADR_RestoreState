/*
 * LoRaWAN.c
 *
 *  Created on: 8 Dec 2022
 *      Author: Connor Bramwell, Mohd Firdaus Hirman Ritom, Faraz Azmi (OTAA Accept Parser)
 */

#include <lorawan/Lorawan.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>


//static void lora_idle()
//{
//    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
//}

//void ABP(u4_t netid, u4_t devAddr, u1_t nwkSKey, u1_t appSKey){//not sure that his function is necessary, does ABP even need to send a packet to 'join' in the first place?
 //this function was made after misunderstanding about ABP, a 'join' packet is not required, a packet containing the correct stuff will be processed by the server regardless
//}

LORA_CTX_t g_lora_ctx;

void initLoraCtx()
{
    //Initialise with 0
    memset(&g_lora_ctx,0,sizeof(LORA_CTX_t));
}

LORA_CTX_t* Lorawan_getLoraCtx()
{
    return &g_lora_ctx;
}

uint8_t* Lorawan_getLoraPayload()
{
    return g_lora_ctx.loraPayload;
}

void printRegisters()
{
    printf("Registers: \n");
    printf("REG_LR_OPMODE = %02x \n",spi_rcv_data(REG_LR_OPMODE));
    printf("REG_LR_FRFMSB = %02x \n",spi_rcv_data(REG_LR_FRFMSB));
    printf("REG_LR_FRFMID = %02x \n",spi_rcv_data(REG_LR_FRFMID));
    printf("REG_LR_FRFLSB = %02x \n",spi_rcv_data(REG_LR_FRFLSB));
    printf("REG_LR_PACONFIG = %02x \n",spi_rcv_data(REG_LR_PACONFIG));
    printf("REG_LR_PARAMP = %02x \n",spi_rcv_data(REG_LR_PARAMP));
    printf("REG_LR_OCP = %02x \n",spi_rcv_data(REG_LR_OCP));
    printf("REG_LR_LNA = %02x \n",spi_rcv_data(REG_LR_LNA));
    printf("REG_LR_MODEMCONFIG1 = %02x \n",spi_rcv_data(REG_LR_MODEMCONFIG1));
    printf("REG_LR_MODEMCONFIG2 = %02x \n",spi_rcv_data(REG_LR_MODEMCONFIG2));
    printf("REG_LR_MODEMCONFIG3 = %02x \n",spi_rcv_data(REG_LR_MODEMCONFIG3));
    printf("REG_LR_PREAMBLEMSB = %02x \n",spi_rcv_data(REG_LR_PREAMBLEMSB));
    printf("REG_LR_PREAMBLELSB = %02x \n",spi_rcv_data(REG_LR_PREAMBLELSB));
    printf("REG_LR_PAYLOADLENGTH = %02x \n",spi_rcv_data(REG_LR_PAYLOADLENGTH));
    printf("REG_LR_FEIMSB = %02x \n",spi_rcv_data(REG_LR_FEIMSB));
    printf("REG_LR_FEIMID = %02x \n",spi_rcv_data(REG_LR_FEIMID));
    printf("REG_LR_FEILSB = %02x \n",spi_rcv_data(REG_LR_FEILSB));
    printf("REG_LR_RSSIWIDEBAND = %02x \n",spi_rcv_data(REG_LR_RSSIWIDEBAND));
    printf("REG_LR_DETECTOPTIMIZE = %02x \n",spi_rcv_data(REG_LR_DETECTOPTIMIZE));
    printf("REG_LR_INVERTIQ = %x \n",spi_rcv_data(REG_LR_INVERTIQ));
    printf("REG_LR_DETECTIONTHRESHOLD = %02x \n",spi_rcv_data(REG_LR_DETECTIONTHRESHOLD));
    printf("REG_LR_SYNCWORD = %02x \n",spi_rcv_data(REG_LR_SYNCWORD));
    printf("REG_LR_INVERTIQ2 = %02x \n",spi_rcv_data(REG_LR_INVERTIQ2));
    printf("REG_LR_DIOMAPPING1 = %02x \n",spi_rcv_data(REG_LR_DIOMAPPING1));
    printf("REG_LR_VERSION = %02x \n",spi_rcv_data(REG_LR_VERSION));
    printf("REG_LR_PADAC = %02x \n",spi_rcv_data(REG_LR_PADAC));

}

uint8_t buildUnconfLoRaWANPacket(uint8_t *packet, uint32_t devAddr, uint32_t frameCounter, uint8_t FPort, uint8_t payload_size, uint8_t * payload , uint8_t * nwkskey, uint8_t * appskey)
{
    LORA_CTX_t* loraCtx = Lorawan_getLoraCtx();

    memcpy(loraCtx->nwkSKey, nwkskey, NWKSKEY_SIZE);
    memcpy(loraCtx->appSKey, appskey, APPSKEY_SIZE);

    loraCtx->devAddr = devAddr;

    int i =0;//Defining iterator here as used many times
    uint8_t packetPointer = 0;//keeps track of where we are currently looking in the packet

    //MHDR stand for MAC header, it includes the packet type which is unconfirmed uplink packet
    uint8_t MHDR = 0x40;//want 01000000 so 0x40

    packet[packetPointer] = MHDR; // puts MHDR at start of packet
    packetPointer++;//moves pointer along to next element

    //next move on to frame header which starts with device address which is 4 bytes long
    //this code should spit the device address into individual bytes and then add them to the packet
    packet[packetPointer] = (devAddr >> 0 ) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (devAddr >> 8 ) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (devAddr >> 16 ) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (devAddr >> 24 ) & 0xFF;
    packetPointer++;//moves pointer along to next element

    //next part of frame header is FCtrl or frame control
    uint8_t FCtrl=0x00;//again work out in binary then do hexadecimal, want 00000000 in binary so 00 in hexadecimal, look at documentation from LoRa alliance to find out what each bit does
    FCtrl = FCTRL_ADR | FCTRL_ADRACKREQ;
    //FCtrl = FCTRL_ADR;//need this for ADR

    packet[packetPointer] = FCtrl; // puts FCtrl next in the packet
    packetPointer++;//moves pointer along to next element


    //need least significant two bytes for this, not too sure which ones they are
    //apparently first byte is least significant (although already accounted for I think)
    //packet[packetPointer] = (frameCounter >>  24) & 0xFF;
    //packetPointer++;//moves pointer along to next element
    //packet[packetPointer] = (frameCounter >>  16) & 0xFF;
    //packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (frameCounter >>  0) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (frameCounter >>  8) & 0xFF;
    packetPointer++;//moves pointer along to next element

    //next is the FOpts but I have set the length of this to 0 so there is nothing to be added here

    //next is the FPort which has to match with server/application which is 1 in our case but I will take is as a function argument for ease of using a different port

    packet[packetPointer] = FPort; // puts FPort next in the packet
    packetPointer++;//moves pointer along to next element

    /*
     * ENCRYPTION PROCESS OF FRMPAYLOAD
     */

    uint8_t encrypt_cycle = 0;

    uint8_t Si[16] = {0};

    uint8_t encrypted_frm_payload[FRM_PAYLOAD_SIZE_MAX] = {0};
    uint8_t final_index = 0;

    uint8_t num_blocks = NUM_OF_16BYTES_BLOCKS(payload_size);

    for(encrypt_cycle = 1; encrypt_cycle <= num_blocks; encrypt_cycle++)
    {
        //need to do this strange exclusive or thing beforehand, I think this is to act like an initialisation vector for the encryption
        uint8_t Ai[16]={0};//this bit of code looks strange but makes sense when you look in to the official documentation
        Ai[0]=0x01;
        Ai[5]=0x00;// dir is 0 for uplink, dir is 1 for downlink
        Ai[6]=(devAddr >> 0) & 0xFF;
        Ai[7]=(devAddr >> 8) & 0xFF;
        Ai[8]=(devAddr >> 16) & 0xFF;
        Ai[9]=(devAddr >> 24) & 0xFF;
        Ai[10]=(frameCounter >> 0) & 0xFF;
        Ai[11]=(frameCounter >> 8) & 0xFF;
        Ai[12]=(frameCounter >> 16) & 0xFF;
        Ai[13]=(frameCounter >> 24) & 0xFF;
        Ai[15]=encrypt_cycle;

        encryptAESECB(appskey, Ai, Si);

        //XOR
        for(i=0; i<16;i++)
        {
            encrypted_frm_payload[final_index] = payload[final_index] ^ Si[i];
            final_index++;
            if(final_index == payload_size)
            {
                break;
            }
        }
    }
/*
    printf("encrypted_frm_payload = ");
    for(i=0; i<FRM_PAYLOAD_SIZE_MAX;i++)
    {
        printf("%02x ", encrypted_frm_payload[i]);
    }
    printf("\n");
*/

    for(i=0; i < payload_size; i++)//payload size should always be 16 after AES encryption
    {
        packet[packetPointer]=encrypted_frm_payload[i];
        packetPointer++;
    }

    // make MIC
    //stuff here looks a bit strange look at official documentation and AES CMAC stuff for exact details


    //uint8_t cmacIn[64]={0};//need 4 blocks I think and this will also pad the end with zeros
    //may need 3 not 4 but not sure so may need 48 not 64
    uint8_t cmacIn[48]={0};
    uint8_t blockNum = sizeof(cmacIn)/16;

    //next fill cmacIn with appropriate values (staring with B0)

    cmacIn[0]=0x49;
    //1-4 are all 0 so don't need to add anything (5 also 0 for uplink)
    //next do device address
    cmacIn[6]=(devAddr >> 0) & 0xFF;
    cmacIn[7]=(devAddr >> 8) & 0xFF;
    cmacIn[8]=(devAddr >> 16) & 0xFF;
    cmacIn[9]=(devAddr >> 24) & 0xFF;
    //next do frame counter
    cmacIn[10]=(frameCounter >> 0) & 0xFF;
    cmacIn[11]=(frameCounter >> 8) & 0xFF;
    cmacIn[12]=(frameCounter >> 16) & 0xFF;
    cmacIn[13]=(frameCounter >> 24) & 0xFF;
    //14 is 0
    //next need length of message, this will vary depending on things normally but this code is made for a fixed size
    //size of 25 in this case
    cmacIn[15]=25;
    //next move on to msg
    //this is effectively just the first 25 bytes of the packet
    for(i=0; i<25; i++){
        cmacIn[16+i]=packet[i];
    }
    //then need to consider zero padding which is 1 followed by may zeros(already have zeros so just need the 1
    cmacIn[41]=0x80;//this should do the 1 at the start


    //this section sets cmacIn to a test value so I can check that my code works
    //6bc1bee2 2e409f96 e93d7e11 7393172a ae2d8a57 1e03ac9c 9eb76fac 45af8e51 30c81c46 a35ce411

    /*uint8_t cmacIn[48]={0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
            0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
            0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};*/
            //0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    /*uint8_t cmacIn[48]={0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
            0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
            0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//version with adjusted padding
    uint8_t blockNum = sizeof(cmacIn)/16;
    */
    //this should be all of the values loaded in to cmac in, now need to start doing the cmac stuff
    uint8_t encIntemp[16]={0};//this holds the value to be fed into encryption
    uint8_t cyphertexttemp[16]={0};//stores the cyphertext output, also works as initialisation vector

    //need to generate subkeys
    uint8_t zeros[16]={0};//full zeros, needed for a few things
    uint8_t L[16]={0};//needed for subkey generation
    uint8_t LShift[16]={0};//needed for subkey generation, stores after bit shift
    //uint8_t Rb[16]={0};//needed for subkey generation
    //Rb[15]=0x87;//only this bit of Rb used so probably dont need to bother with it

    encryptAESECB(nwkskey,zeros,L);

    //decided to try some of the test values from the documentation
    //2b7e1516 28aed2a6 abf71588 09cf4f3c
    //uint8_t testkey[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f,0x3c};
    //uint8_t testkey[16] = {0x2B,0x7E,0x15,0x16,0x28,0xAE,0xD2,0xA6,0xAB,0xF7,0x15,0x88,0x09,0xCF,0x4F,0x3C};//had typos here
    //uint8_t * testkeyptr=testkey;
    //uint8_t * zerosptr=zeros;
    //encryptAESECB(testkeyptr,zerosptr,L);
    //encryptAESECB(testkey,zeros,L);
    /*
    uint8_t testkey2[16]={0};
    for (i=0; i<16; i++){
        testkey2[i]=testkey[15 - i];//flips round in case this is the problem
    }
    encryptAESECB(testkey2,zeros,L);
    //confirmed that my encrypt function works as expected so don't need to flip anything around
    */

    //unfortunately need to bit shift these arrays
    //need to manually consider carry on each
    //start at end then move up
    //on documentation most significant bit is leftmost, so this will remove the MSB
    uint8_t carry=0; //flag for if I need to carry a bit between bytes
    for(i=0;i<16;i++){
        //LShift[15-i]=L[15-i]/2;//dividing by 2 should shift along by one place
        //LShift[i]=L[i]*2;//Multiplying by 2 should shift up by 1
        LShift[15-i]=L[15-i]*2;//Multiplying by 2 should shift up by 1
        if(carry==1){
            //LShift[15-i]=LShift[15-i] + 128;//adds most significant bit on if carried from previous
            //LShift[i]=LShift[i] + 1;//adds least significant bit if from previous
            LShift[15-i]=LShift[15-i] + 1;//adds least significant bit if from previous
        }
        //carry=L[15-i]%2;//will set to 1 if 1 in LSB so can find by if number is odd
        carry=0;
        //if(L[i]>=128){//will make carry 1 if MSB would be lost
        if(L[15-i]>=128){//will make carry 1 if MSB would be lost
            carry=1;
        }
    }
    //should have properly shifted L now
    uint8_t K1[16]={0};//first subkey (not used here as used when blocks match)
    for(i=0;i<16;i++){
        K1[i]=LShift[i];
    }
    /*
    if(L[15]<128){//does this exclusive or if MSB is 1
        K1[15]=K1[15]^0x87;
        //K1[0]=K1[0]^0x87;
    }
    */
    if(L[0]>=127){//does this exclusive or if MSB is 1
        K1[15]=K1[15]^0x87;
        //K1[0]=K1[0]^0x87;
    }
    uint8_t K2[16]={0};//second subkey
    //then do this shift thing again
    carry=0;
    for(i=0;i<16;i++){
        K2[15-i]=K1[15-i]*2;//Multiplying by 2 should shift up by 1
        if(carry==1){
             K2[15-i]=K2[15-i] + 1;//adds least significant bit if from previous
        }
        carry=0;
        if(K1[15-i]>=128){//will make carry 1 if MSB would be lost
            carry=1;
        }
    }

    if(K1[0]>=127){//does this exclusive or if MSB is 1
        K2[15]=K2[15]^0x87;
    }

    //K2[15]=K2[15]^0x87;

    //this next part is used for testing to see if I get the same values as examples
    /*
    //as only one block ignoring iv as ^0 has no effect
    uint8_t MICTestIn1[16]={0};
    MICTestIn1[0]=K2[0]^0x80;//should adjust for weird padding thing
    for(i=1;i<16;i++){
        MICTestIn1[i]=K2[i];
    }
    uint8_t MICTestOut1[16]={0};
    encryptAESECB(testkey,MICTestIn1,MICTestOut1);//this should mimic the example with null input

    //6bc1bee2 2e409f96 e93d7e11 7393172a
    uint8_t MICTestIn2[16]={0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
    uint8_t MICTestOut2[16]={0};
    for(i=0;i<16;i++){
        MICTestIn2[i]=MICTestIn2[i]^K1[i];//K1 as full block
    }
    encryptAESECB(testkey,MICTestIn2,MICTestOut2);//this should mimic the example with one full block
    */

    //does cmac in this loop
    int j;
    for(i=0; i<blockNum; i++){
        for(j=0;j<16;j++){
            encIntemp[j]=cmacIn[((i*16)+(j))]^cyphertexttemp[j];//this should apply initialisation vector (by doing xor) and load data

            if((i + 1) == blockNum){
                encIntemp[j]=encIntemp[j]^K2[j];
            }
        }

        encryptAESECB(nwkskey,encIntemp,cyphertexttemp);//this should do the encryption for one block
        //encryptAESECB(testkey,encIntemp,cyphertexttemp);//this should do the encryption for one block(test ver)
    }
    //need to put first 4 bytes of last cyphertext block into end of packet for MIC
    //need to check which way round "first 4" is, may be opposite to what I expect
    for(i=0;i<4;i++){
        packet[packetPointer]=cyphertexttemp[i];
        packetPointer++;
    }
    //the packet should be done now
    //packet[packetPointer]=1;//adds an extra 1 to the end of the packet that should not be send (for debug purposes to see where the packet ends as receiver displays 0 for both 0 and nothing received)

    return packetPointer;//returns the pointer for where in packet array the end of the packet is
}

uint8_t buildOTAALoRaWANPacket(uint8_t *packet, uint8_t *JoinEUI, uint8_t *DevEUI, uint16_t DevNonce, uint8_t *Key){
    uint8_t packetPointer = 0;//keeps track of where we are currently looking in the packet
    int i=0;

    //MHDR stand for MAC header, it includes the packet type which is unconfirmed uplink packet
    uint8_t MHDR = 0x00;//want all zeros for join request

    packet[packetPointer] = MHDR; // puts MHDR at start of packet
    packetPointer++;//moves pointer along to next element

    for(i=0;i<8;i++){
        packet[packetPointer] = JoinEUI[i]; // adds JoinEUI (assumed already flipped for endianess purposes, need least significant first)
        packetPointer++;//moves pointer along to next element
    }
    for(i=0;i<8;i++){
        packet[packetPointer] = DevEUI[i]; // adds devEUI (assumed already flipped for endianess purposes, need least significant first)
        packetPointer++;//moves pointer along to next element
    }
    /*
    packet[packetPointer] = (DevNonce >>  8) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (DevNonce >>  0) & 0xFF;
    packetPointer++;//moves pointer along to next element
    */
    //little endian DevNonce:

    packet[packetPointer] = (DevNonce >>  0) & 0xFF;
    packetPointer++;//moves pointer along to next element
    packet[packetPointer] = (DevNonce >>  8) & 0xFF;
    packetPointer++;//moves pointer along to next element


    // make MIC
       //stuff here looks a bit strange look at official documentation and AES CMAC stuff for exact details


       //uint8_t cmacIn[64]={0};//need 4 blocks I think and this will also pad the end with zeros
       //may need 3 not 4 but not sure so may need 48 not 64 (need 2 for OTAA join request so 32)(this code adapted from data uplink MIC)
       uint8_t cmacIn[32]={0};
       //uint8_t blockNum = sizeof(cmacIn)/16;//should end up as 2 here
       uint8_t blockNum = 2;

       //next fill cmacIn with appropriate values (staring with B0)

       cmacIn[0]=0x00;//MHDR of 00 as join request
       for(i=0;i<8;i++){
           cmacIn[1+i] = JoinEUI[i]; // adds JoinEUI to cmac (assumed already flipped for endianess purposes, need least significant first)
       }
       for(i=0;i<8;i++){
           cmacIn[9+i] = DevEUI[i]; // adds devEUI to cmac (assumed already flipped for endianess purposes, need least significant first)
       }
       cmacIn[17] = (DevNonce >>  0) & 0xFF;//adds DevNonce to cmac
       cmacIn[18] = (DevNonce >>  8) & 0xFF;
       //then need to consider zero padding which is 1 followed by may zeros(already have zeros so just need the 1
       cmacIn[19]=0x80;//this should do the 1 at the start


       //this should be all of the values loaded in to cmac in, now need to start doing the cmac stuff
       uint8_t encIntemp[16]={0};//this holds the value to be fed into encryption
       uint8_t cyphertexttemp[16]={0};//stores the cyphertext output, also works as initialisation vector

       //need to generate subkeys
       uint8_t zeros[16]={0};//full zeros, needed for a few things
       uint8_t L[16]={0};//needed for subkey generation
       uint8_t LShift[16]={0};//needed for subkey generation, stores after bit shift
       //uint8_t Rb[16]={0};//needed for subkey generation
       //Rb[15]=0x87;//only this bit of Rb used so probably dont need to bother with it

       encryptAESECB(Key,zeros,L);

       //unfortunately need to bit shift these arrays
       //need to manually consider carry on each
       //start at end then move up
       //on documentation most significant bit is leftmost, so this will remove the MSB
       uint8_t carry=0; //flag for if I need to carry a bit between bytes
       for(i=0;i<16;i++){
           LShift[15-i]=L[15-i]*2;//Multiplying by 2 should shift up by 1
           if(carry==1){
               LShift[15-i]=LShift[15-i] + 1;//adds least significant bit if from previous
           }
           carry=0;
           if(L[15-i]>=128){//will make carry 1 if MSB would be lost
               carry=1;
           }
       }
       //should have properly shifted L now
       uint8_t K1[16]={0};//first subkey (not used here as used when blocks match)
       for(i=0;i<16;i++){
           K1[i]=LShift[i];
       }
       if(L[0]>=127){//does this exclusive or if MSB is 1
       //if(L[0]>=0x80){
           K1[15]=K1[15]^0x87;
       }
       uint8_t K2[16]={0};//second subkey
       //then do this shift thing again
       carry=0;
       for(i=0;i<16;i++){
           K2[15-i]=K1[15-i]*2;//Multiplying by 2 should shift up by 1
           if(carry==1){
                K2[15-i]=K2[15-i] + 1;//adds least significant bit if from previous
           }
           carry=0;
           if(K1[15-i]>=128){//will make carry 1 if MSB would be lost
               carry=1;
           }
       }

       if(K1[0]>=127){//does this exclusive or if MSB is 1
           K2[15]=K2[15]^0x87;
       }

       //does cmac in this loop
       int j;
       for(i=0; i<blockNum; i++){
           for(j=0;j<16;j++){
               encIntemp[j]=cmacIn[((i*16)+(j))]^cyphertexttemp[j];//this should apply initialisation vector (by doing xor) and load data
               if((i + 1) == blockNum){
                   encIntemp[j]=encIntemp[j]^K2[j];
               }
           }

           encryptAESECB(Key,encIntemp,cyphertexttemp);//this should do the encryption for one block
       }
       //need to put first 4 bytes of last cyphertext block into end of packet for MIC
       //need to check which way round "first 4" is, may be opposite to what I expect
       for(i=0;i<4;i++){
           packet[packetPointer]=cyphertexttemp[i];
           packetPointer++;
       }
       //the packet should be done now


    //next part adds a hard coded MIC for test purposes
    /*
    packet[packetPointer-4]=0x8b;
    packet[packetPointer-3]=0xa8;
    packet[packetPointer-2]=0x5e;
    packet[packetPointer-1]=0xff;
    */


    //next part is to print the cmac algoritm output


    packet[packetPointer]=1;//adds an extra 1 to the end of the packet that should not be sent (for debug purposes to see where the packet ends as receiver displays 0 for both 0 and nothing received)
    printf("join request packet:   ");
        for(i=0;i<packetPointer;i++){
            printf("%02x ",packet[i]);
        }
        printf("\n");
    return packetPointer;
}

//sends the packet
void loraWANSend(uint8_t *packet, uint8_t packetSize)
{
    uint8_t i;
    //uint8_t debug=0;//Connor - used to help see what is going on, may not be able to read everything though

    // Go to standby
    //lora_idle();//I don't think that it should be able to do this line due to how files are set up
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);//sets to LoRa standby
    //tried some of these lines of code with numbers directly as suspicious that it could not find the names as number did not appear when hovering over
    //above one appears to be okay though
    //debug= spi_rcv_data(REG_OP_MODE);
    //clear radio IRQ flags
    spi_snd_data(0x12, 0xFF);      // Faraz - clearing all interrupt flags
    __delay_cycles(1000u);

    //not sure why some stuff has LR on it
    //may try some of there directly with the locations at some point

    spi_snd_data(REG_LR_FIFOADDRPTR, 0);//selects correct address on transceiver (need to check this)(I think sets pointer to 0 which I guess is right)
    //spi_snd_data(0x0D, 0);//selects correct address on transceiver (need to check this)(I think sets pointer to 0 which I guess is right)
    //debug= spi_rcv_data(REG_LR_FIFOADDRPTR);

    printf("Packet size = %d\n",packetSize);
    for (i = 0u; i <= packetSize; i++)
    {
        //need to check this, may actually be repeatedly writing to the same address, although apparently increments automatically
        printf("%02x ",packet[i]);
        spi_snd_data(REG_LR_FIFO, packet[i]);//loads the packet itself on to transmission register on transceiver
    }

    printf("\n");

    // Set the payload size
    //spi_snd_data(REG_LR_PAYLOADLENGTH, (packetSize+1));//tells transceiver the size of the packet
    //spi_snd_data(0x22, (packetSize+1));//tells transceiver the size of the packet
    spi_snd_data(0x22, (packetSize));//tells transceiver the size of the packet
    spi_snd_data( REG_LR_DIOMAPPING1,0x40);
    //spi_snd_data( 0x40,0x40);
    //debug= spi_rcv_data(REG_LR_DIOMAPPING1);

    //Print registers
    //printRegisters();
    //__delay_cycles(1000u);


    // Going to TX mode (so the transceiver will actually send the data in the register)
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);
    //spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | 0x02);//frequency synthesis Tx mode as opposed to normal Tx
    //debug= spi_rcv_data(REG_OP_MODE);
    //spi_snd_data(REG_OP_MODE, debug);//I think I need to do this for the debug to appear on the debug
    // Waiting for the end of the transmission

    volatile uint8_t r = spi_rcv_data(REG_LR_IRQFLAGS);
    while ( !DIO0 );
    spi_rcv_data(REG_LR_IRQFLAGS);
    spi_snd_data(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
}

static void start_lorawanRX_timer(void)
{
    // Intialize TIMER
    TB0CCTL0 = CCIE;                        // TACCR0 interrupt enabled
    TB0CCR0 = 31250; // 0.25s
    //TB0CCR0 = 46875; // 0.375s
    //TB0CCR0 = 62500; // 0.5s
    TB0CTL = ID | TASSEL__SMCLK | MC__UP;        // SMCLK, UP mode
}

void loraWANreceive()
{

    lora_singleRxWindowConfig();
    LORA_CTX_t* loraCtx = Lorawan_getLoraCtx();

    //Reset payload buffer and size
    memset(loraCtx->loraPayload,0,MAX_PAYLOAD_BUFFER);
    loraCtx->loraPayloadSize = 0;

    //Start RX timer interrupt
    start_lorawanRX_timer();
    P3OUT ^= BIT2;
}

static void doLoraWANreceive()
{
    int i = 0;
    LORA_CTX_t* loraCtx = Lorawan_getLoraCtx();
    lora_startRx();
    do
    {
        printf("Fir Start Receiving Mode\n");
        loraCtx->loraPayloadSize = lora_receive(loraCtx->loraPayload);
        printf("loraCtx->loraPayloadSize = %d\n", loraCtx->loraPayloadSize);
        if(loraCtx->loraPayloadSize > 0 )
        {
            //Print buf
            printf("loraCtx->loraPayload = ");
            for (i = 0u; i < MAX_PAYLOAD_BUFFER; i++)
            {
                printf("%02x ",loraCtx->loraPayload[i]);
            }
            printf("\n");

            /*
             * PROCESS LORA PHY PAYLOAD
             */
            parsePhyPayload(loraCtx);

            if(MIC_NOT_OK == loraCtx->isMICOk)
            {
                printf("MIC Not OK\n");
                break;
            }

            if (loraCtx->loraPayload[0] == 0x20)
                        {
                            printf("JOIN ACCEPT MESSAGE! \n");
                            parseOTAA_Accept(loraCtx->appKey);          // Faraz - Parse OTAA accept message to generate NwkSKey & AppSKey for future data uplinks
                        }

                        else
                        {
                        /*
                         * PROCESS MAC PAYLOAD
                         */
                        parseMacPayload(loraCtx);

                        if(loraCtx->framePayloadSize != 0)
                        {
                            /*
                             * DECRYPTION PROCESS OF FRMPAYLOAD
                             */
                            printf("Inside frame payload \n");

                            uint8_t decrypt_cycle = 0;

                            uint32_t devAddr = loraCtx->macFrameHeader.devAddr;
                            uint32_t frameCounter = loraCtx->macFrameHeader.fCnt;

                            uint8_t Si[16] = {0};

                            uint8_t decrypted_frm_payload[FRM_PAYLOAD_SIZE_MAX] = {0};
                            uint8_t final_index = 0;

                            uint8_t num_blocks = NUM_OF_16BYTES_BLOCKS(loraCtx->framePayloadSize);
                            printf("num_blocks = %d\n", num_blocks);
                            printf("devAddr = 0x%" PRIx32 "\n",devAddr);
                            printf("frameCounter = %d\n", frameCounter);

                            for(decrypt_cycle = 1; decrypt_cycle <= num_blocks; decrypt_cycle++)
                            {
                                //need to do this strange exclusive or thing beforehand, I think this is to act like an initialisation vector for the encryption
                                uint8_t Ai[16]={0};//this bit of code looks strange but makes sense when you look in to the official documentation
                                Ai[0]=0x01;
                                Ai[5]=0x01;// dir is 0 for uplink, dir is 1 for downlink
                                Ai[6]=(devAddr >> 0) & 0xFF;
                                Ai[7]=(devAddr >> 8) & 0xFF;
                                Ai[8]=(devAddr >> 16) & 0xFF;
                                Ai[9]=(devAddr >> 24) & 0xFF;
                                Ai[10]=(frameCounter >> 0) & 0xFF;
                                Ai[11]=(frameCounter >> 8) & 0xFF;
                                Ai[12]=(frameCounter >> 16) & 0xFF;
                                Ai[13]=(frameCounter >> 24) & 0xFF;
                                Ai[15]=decrypt_cycle;

                                encryptAESECB(loraCtx->nwkSKey, Ai, Si); //TODO: IF fport is non-zero, use the appSkey instead

                                //XOR
                                for(i=0; i<16;i++)
                                {
                                    decrypted_frm_payload[final_index] = loraCtx->framePayload[final_index] ^ Si[i];
                                    final_index++;
                                    if(final_index == loraCtx->framePayloadSize)
                                    {
                                        break;
                                    }
                                }
                            }

                            printf("decrypted_frm_payload = ");
                            for(i=0; i<FRM_PAYLOAD_SIZE_MAX;i++)
                            {
                                printf("%02x ", decrypted_frm_payload[i]);
                            }
                            printf("\n");

                            /*
                             * PROCESS MAC COMMANDS
                             */
                            parseMacCommands(loraCtx, &decrypted_frm_payload[0]);
                        }

                        } // Faraz : Placed by faraz as scope for else statement OTAA parsing
            }
    } while (0);

    printf("lora_ctx.spreadFactor = %02x\n", loraCtx->spreadFactor);
    printf("lora_ctx.txPower = %02x\n", loraCtx->txPower);
}

int g_TimerB0_cnt = 0;
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)//triggered every half second
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B0_VECTOR))) Timer0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if(g_TimerB0_cnt == 0) //This statement happens every 1 second!
    {
        //printf("g_counter1\n");
        g_TimerB0_cnt = 0;
        TB0CTL = MC__STOP;  // stop the timer
        P3OUT ^= BIT2;
        doLoraWANreceive();
        P3OUT ^= BIT2;
    }
    else
    {
        g_TimerB0_cnt++;
    }

    TB0CTL |= TBCLR;
}



void encryptAESECB(uint8_t *key , uint8_t *plaintext, uint8_t *cyphertext){//assuming block size of 128 bits always
    AESACTL0=AESSWRST;// reset AES just in case
    //following values are not setable like this despite seeming so in pseudo code
    //AESCMEN= 0x01;//I think this allows access to the registers
    //AESCMx= 0x00;//for ECB
    //AESOPx= 0x00;//sets to encryption
    //AESKLx= 0x00;//sets to 128 bit
    //the next line should implement above stuff
    //AESACTL0= 0x8000;//1000000000000000 should be 0x8000
    //I think the reset alone should be good enough here
    AESACTL0= 0b1000000000000100;
    AESACTL0= 0b1000000000000000;//this should be a strange way of resetting the key (probably better way)
    //AESACTL1=0x01;//1 due to number of blocks, this may be redundant (may need to be 0 instead of 1)
    //next part should load the key into the register

    int i =0;
    //uint8_t debug1=0;
    //uint8_t debug2=0;
    uint8_t * reg = &AESAKEY;//define pointer that points to the key register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        //debug1=*(key+i);//this is just so I can see the values being loaded in on the debugger
        //debug2=debug1;//need this for debug otherwise debug 1 gets optimised out
        //seems like can't see this really in debug mode, just have to guess if working properly
        *reg=*(key+i);//sets value in register to appropriate key value
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //next do same but load plaintext in

    reg = &AESADIN;//sets pointer to location of input register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *reg=*(plaintext+i);//sets value in register to plaintext
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //I think that it should start now that input is filled, should wait long enough due to while

    //need to put output in to cyphertext, again similar process

    reg = &AESADOUT;//sets pointer to location of output register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *(cyphertext+i)=*reg;//sets cyphertext based off of values in register
    }
    while(AESASTAT&AESBUSY);//should wait until done



    //not sure where below code needs to go
    //AESBLKCNTx= 0x01;//1 due to number of blocks, this may be redundant (may need to be 0 instead of 1)
    AESACTL1=0x01;//1 due to number of blocks, this may be redundant (may need to be 0 instead of 1)
    //not sure if this line even does anything, recommended in the documentation though


}

void encryptAESCBC(uint8_t *key , uint8_t *plaintext, uint8_t *cyphertext){//assuming block size of 128 bits always
    AESACTL0=AESSWRST;// reset AES just in case
    //following values are not setable like this despite seeming so in pseudo code
    //AESCMEN= 0x01;//I think this allows access to the registers
    //AESCMx= 0x00;//for ECB
    //AESOPx= 0x00;//sets to encryption
    //AESKLx= 0x00;//sets to 128 bit
    //the next line should implement above stuff
    //AESACTL0= 0x8000;//1000000000000000 should be 0x8000
    //I think the reset alone should be good enough here
    AESACTL0= 0b1000000000000100;
    AESACTL0= 0b1000000000000000;//this should be a strange way of resetting the key (probably better way)

    //next part should load the key into the register

    int i =0;

    printf("Key in Encrypt function: ");
    for(i=0; i<16; i++)
      {
        printf("%02x ",key[i]);
      }
    printf("\n");

    printf("Plaintext in Encrypt function: ");
    for(i=0; i<16; i++)
      {
        printf("%02x ",plaintext[i]);
      }
    printf("\n");
    //uint8_t debug1=0;
    //uint8_t debug2=0;
    uint8_t * reg = &AESAKEY;//define pointer that points to the key register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        //debug1=*(key+i);//this is just so I can see the values being loaded in on the debugger
        //debug2=debug1;//need this for debug otherwise debug 1 gets optimised out
        //seems like can't see this really in debug mode, just have to guess if working properly
        *reg=*(key+i);//sets value in register to appropriate key value
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //next do same but load plaintext in

    reg = &AESADIN;//sets pointer to location of input register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *reg=*(plaintext+i);//sets value in register to plaintext
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //I think that it should start now that input is filled, should wait long enough due to while

    //need to put output in to cyphertext, again similar process

    reg = &AESADOUT;//sets pointer to location of output register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *(cyphertext+i)=*reg;//sets cyphertext based off of values in register
    }
    while(AESASTAT&AESBUSY);//should wait until done

    printf("Cyphertext in Encrypt function: ");
    for(i=0; i<16; i++)
      {
        printf("%02x ",cyphertext[i]);
      }
    printf("\n");


    //not sure where below code needs to go
    //AESBLKCNTx= 0x01;//1 due to number of blocks, this may be redundant (may need to be 0 instead of 1)
    AESACTL1=0x01;//again implements above
    //not sure if this line even does anything, recommended in the documentation though


}

void decryptAESECB(uint8_t *key , uint8_t *cyphertext , uint8_t *plaintext){//assuming block size of 128 bits always
    AESACTL0=AESSWRST;// reset AES just in case
    //following values are not setable like this despite seeming so in pseudo code
    //AESCMEN= 0x01;//I think this allows access to the registers
    //AESCMx= 0x00;//for ECB
    //AESOPx= 0x01;//sets to decryption
    //AESKLx= 0x00;//sets to 128 bit
    //the next line should implement above stuff
    AESACTL0= 0x8001;//1000 0000 0000 0001 should be 0x8001 (note above stuff not in order, look at documentation for more
    AESACTL0= 0b1000000000000101;
    AESACTL0= 0b1000000000000001;//this should be a strange way of resetting the key (probably better way)


    //next part should load the key into the register
    //uint8_t debug1=0;
    int i =0;
    uint8_t * reg = &AESAKEY;//define pointer that points to the key register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        //debug1=*(key+i);//this is just so I can see the values being loaded in on the debugger
        *reg=*(key+i);//sets value in register to appropriate key value
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //next do same but load cyphertext in

    reg = &AESADIN;//sets pointer to location of input register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *reg=*(cyphertext+i);//sets value in register to plaintext
    }
    while(AESASTAT&AESBUSY);//should wait until done

    //I think that it should start now that input is filled, should wait long enough due to while

    //need to put output in to plaintext, again similar process

    reg = &AESADOUT;//sets pointer to location of output register
    for (i=0;i<16;i++){//loops 16 times, once for each byte
        *(plaintext+i)=*reg;//sets cyphertext based off of values in register
    }
    while(AESASTAT&AESBUSY);//should wait until done



    //not sure where below code needs to go
    //AESBLKCNTx= 0x01;//1 due to number of blocks, this may be redundant (may need to be 0 instead of 1)
    AESACTL1=0x01;//again implements above
    //not sure if this line even does anything, recommended in the documentation though
}


