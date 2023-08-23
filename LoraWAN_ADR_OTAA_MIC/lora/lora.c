/*
 * LoRa.c
 *
 *  Created on: 28 Jun 2022
 *      Author: mirko (some alterations by Connor)
 */

#include <lora/lora.h>
#include <lorawan/Lorawan.h>

#include <stdio.h>


/**
 * @brief Put the LoRa module into idle mode
 *
 */
static void lora_idle()
{
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
}

/**
 * @brief Put the LoRa module into Sleep mode
 *
 */
void lora_sleep()
{
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP);
}


/**
 * @brief Set the LoRa module frequency (866mhz EU)
 *
 * @param frequency Frequency in MHz
 *
 */
static void lora_setFrequency(long frequency)
{
    //frequency = 868100000;

    //set frequency
    uint64_t frf = ((uint64_t)frequency << 19) / 32000000;
    spi_snd_data(REG_FRF_MSB, (uint8_t)(frf >> 16));
    spi_snd_data(REG_FRF_MID, (uint8_t)(frf >> 8));
    spi_snd_data(REG_FRF_LSB, (uint8_t)(frf >> 0));
}

/**
 * @brief Initialize the LoRa RFM95 module
 *
 */
void lora_begins()
{
    //put in sleep mode
    lora_sleep();

    //line from Connor to set spreading factor for test purposes
    spi_snd_data(REG_LR_MODEMCONFIG2,0xC4);//SF7 (default)

    // Set EU frequency
    lora_setFrequency(866E6);//need to check this

    // set base addresses
    spi_snd_data(REG_FIFO_TX_BASE_ADDR, 0);
    spi_snd_data(REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost
    uint8_t r = spi_rcv_data(REG_LNA);
    spi_snd_data(REG_LNA, r | 0x03);

    // set auto AGC
    //spi_snd_data(REG_MODEM_CONFIG_3, 0x04);         // For low data rate optimisation disable, needed for SF10 and below
    spi_snd_data(REG_MODEM_CONFIG_3, 0x0C);       // For low data rate optimisation enable, needed for SF 11 & 12

    // set output power to 17 dBm
    spi_snd_data(REG_PA_DAC, 0x84);//Connor- as far as I can tell this just sets back to default, although I think the default is correct

    // enable overload current protection (100ma)
    uint8_t ocpTrim = 27;
    ocpTrim = (100 + 30) / 10;

    spi_snd_data(REG_OCP, 0x20 | (0x1F & ocpTrim));

    spi_snd_data(REG_PA_CONFIG, 0x8F);

    spi_snd_data(REG_LR_SYNCWORD,0x34);

    //spi_snd_data(REG_LR_IRQFLAGSMASK, (~0x08));
    spi_snd_data(REG_LR_IRQFLAGSMASK, 0x00);

    // put in standby mode
    lora_idle();
}

void lora_begins1()//Connor- adaptation of Arduino example
{
    lora_sleep();

    //SF not here in Arduino code but I want it
    spi_snd_data(REG_LR_MODEMCONFIG2,0x74);//SF7 (default)
    //spi_snd_data(0x1E,0xC0);//SF12
    //spi_snd_data(0x1E,0xC4);//SF12, this should match the Arduino

    // Set EU frequency
    //double freq=868100000.0;//may have to set as double type
    //double freq=866E6;//may have to set as double type
    //lora_setFrequency(freq);
    //lora_setFrequency(866E6);//need to check this
    //small section of stuff from Connor, just trying some alternate frequencies
    //decided to try some other frequencies too
    //lora_setFrequency(867700000);//Frequency Mirko added to TTN
    //lora_setFrequency(868100000);//one of the official frequencies
    lora_setFrequency(868300000);//one of the official frequencies
    //lora_setFrequency(868500000);//one of the official frequencies

    // set base addresses
    spi_snd_data(REG_FIFO_TX_BASE_ADDR, 0);
    spi_snd_data(REG_FIFO_RX_BASE_ADDR, 0);

    // set LNA boost
    uint8_t r = spi_rcv_data(REG_LNA);
    spi_snd_data(REG_LNA, (r | 0x03));

    // set auto AGC
    spi_snd_data(REG_MODEM_CONFIG_3, 0x04);
    //spi_snd_data(REG_MODEM_CONFIG_3, 0x0C);//this should match the Arduino

    //set power
    // set output power to 17 dBm
    //spi_snd_data(REG_PA_DAC, 0x84);//Connor- as far as I can tell this just sets back to default, although I think the default is correct
    spi_snd_data(REG_PA_DAC, 0x87);
    //spi_snd_data(REG_OCP, 0x20 | (0x1F & 140));//over current protection thing
    //also variant where it sets it to 0x87
    //code for power is complex in origional and has a bunch of if statemts, tried to get the important tuff
    //spi_snd_data(REG_PA_CONFIG, 0x70 | 17); //this seems to be the most important line from the set power code but this is very trimmed
    spi_snd_data(REG_PA_CONFIG, (0x70 | 15)); //this seems to be the most important line from the set power code but this is very trimmed
    spi_snd_data(REG_OCP, (0x20 | (0x1F & 11)));//11 as values are scaled before being set
    //may need OCP trim stuff here
    //writeRegister(REG_OCP, 0x20 | (0x1F & ocpTrim)); OCP trim line either set to 140 or 100

    //spi_snd_data(REG_OCP, 0x20 | (0x1F & 140));
    /*Mirko's set OCP, may be right
    // enable overload current protection (100ma)
    uint8_t ocpTrim = 27;
    ocpTrim = (100 + 30) / 10;
    spi_snd_data(REG_OCP, 0x20 | (0x1F & ocpTrim));//may want OCP before actually setting power level
    */

    //this is the end of the equivalent Arduino function but I think that there may be more settings somewhere


    //writeRegister(REG_MODEM_CONFIG_1, readRegister(REG_MODEM_CONFIG_1) & 0xfe);//explicit header mode
    r=spi_rcv_data(REG_LR_MODEMCONFIG1);
    spi_snd_data(REG_LR_MODEMCONFIG1,(r & 0xFE));

    spi_snd_data(REG_LR_PARAMP,0x08);

    //may need to be in standby instead of sleep here
    //lora_idle();
    //rest of this is stuff found from various other Arduino code functions:
    //this contradicts earier stuff but is later in the Arduino code (other used for Rx?)
    //this code was orgionally from a Tx function so may need to move to Tx function
    spi_snd_data(REG_PA_CONFIG, 140);
    r = spi_rcv_data(REG_PA_DAC);
    spi_snd_data(REG_PA_DAC, ((r & ~0x07) | 4));//not sure this register can be changed in this mode
    spi_snd_data(REG_OCP, (11 | 0x20));

    //not sure if I need following code
    //sync word
    //not sure if next two can be changed as not part of LoRa modem apparently (without going in to another mode)
    //spi_snd_data(0x39, 0x34);
    //IRQ mapping
    //spi_snd_data(0x40, (0x30|0x40|0x0C));
    //clear radio IRQ flags
    spi_snd_data(REG_LR_IRQFLAGS, 0xFF);
    //mask all IRQs other than TxDone
    spi_snd_data(REG_LR_IRQFLAGSMASK, (~0x08));

    //already do the pointer thing in other places as well as the payload length
    //last part is the load in which is done in the send function in this program

    //spi_snd_data(REG_LR_INVERTIQ,0x00);
    //spi_snd_data(REG_LR_SYNCWORD,0x34);

    lora_idle();
}

/**
 * @brief Sends a packet over LoRa protocol
 *
 * @param data Pointer to the packet to be sent
 * @param pkt_size Size of the packet
 *
 */
void lora_send(uint8_t *data, uint8_t pkt_size)
{
    uint8_t i;

    // Go to standby
    lora_idle();
    __delay_cycles(1000u);

    spi_snd_data(REG_LR_FIFOADDRPTR, 0);

    for (i = 0u; i < pkt_size; i++)
    {
        spi_snd_data(REG_LR_FIFO, data[i]);
    }

    // Set the payload size
    spi_snd_data(REG_LR_PAYLOADLENGTH, pkt_size);
    spi_snd_data( REG_LR_DIOMAPPING1,0x40);

    // Going to TX mode
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    // Waiting for the end of the transmission
    volatile uint8_t r = spi_rcv_data(REG_LR_IRQFLAGS);
    while ( !DIO0 );
    spi_rcv_data(REG_LR_IRQFLAGS);
    spi_snd_data(REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE);
}

/**
 * @brief Receives a packet over LoRa protocol
 *
 * @param buffer Pointer to the receiver's buffer
 *
 * @return Length of the received packet
 */
uint8_t lora_receive(uint8_t *buffer)
{

    uint8_t irq_flag = 0;
        uint8_t opmode = spi_rcv_data(REG_OP_MODE);
        uint8_t symboltimeout = spi_rcv_data(REG_LR_SYMBTIMEOUTLSB);
        uint8_t packetLength = 0;

        do
        {
            irq_flag = spi_rcv_data(REG_IRQ_FLAGS);

            printf("lora_receive: irq_flag = %02x, opmode =  %02x, symboltimeout = %02x\n",irq_flag,opmode,symboltimeout);
            if (irq_flag & IRQ_RX_DONE_MASK)
            {
                printf("lora_receive: rxDone\n");
                packetLength = spi_rcv_data(REG_RX_NB_BYTES);

                // set FIFO address to current RX address
                spi_snd_data(REG_FIFO_ADDR_PTR, spi_rcv_data(REG_FIFO_RX_CURRENT_ADDR));
                uint8_t i;
                for (i=0; i < packetLength; i++)
                {
                    buffer[i]=spi_rcv_data(REG_FIFO);
                }

                // clear IRQ's
                spi_snd_data(REG_IRQ_FLAGS, irq_flag);

                break;
            }
            else if(opmode != (MODE_LONG_RANGE_MODE | MODE_RX_SINGLE))
            {
                printf("lora_receive: reset FIFO address\n");
                // reset FIFO address
                spi_snd_data(REG_FIFO_ADDR_PTR, 0);

                // put in single RX mode
                spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
            }
            else
            {
                printf("lora_receive: waiting...\n");
            }

            // clear IRQ's
            spi_snd_data(REG_IRQ_FLAGS, irq_flag);

        } while((irq_flag & IRQ_RX_TIMEOUT_MASK) == 0);

        //Set to idle again
        lora_idle();

        return packetLength;
}

/**
 * @brief Retrieves Spreading Factor
 *
 * @return Integer value between 6 and 12
 */
uint8_t lora_get_SpreadingFactor()
{
    return spi_rcv_data(REG_MODEM_CONFIG_2) >> 4;
}

/**
 * @brief Sets bandwidth
 *
 */
void lora_set_signal_bandwidth(uint8_t bw)
{
    uint8_t setBwBits = 0;
    uint8_t reg = 0;

  switch (bw)
  {
    case LORA_BW_125_KHZ:
    {
        setBwBits = LORA_MODULE_BW_125KHZ;
    }
    break;
    case LORA_BW_250_KHZ:
    {
        setBwBits = LORA_MODULE_BW_250KHZ;
    }
    break;
    case LORA_BW_500_KHZ:
    {
        setBwBits = LORA_MODULE_BW_500KHZ;
    }
    break;
    default:
    {
        //Unsupported Lora bandwidth
        return;
    }
  }

  reg = spi_rcv_data(REG_MODEM_CONFIG_1);
  reg &= 0x0F;
  reg |= setBwBits;

  spi_snd_data(REG_MODEM_CONFIG_1,reg);
}

/**
 * @brief Retrieves bandwidth
 *
 * @return Signal bandwidth long
 */
long get_signal_bandwidth()
{
  uint8_t bw = (spi_rcv_data(REG_MODEM_CONFIG_1) >> 4);

  switch (bw) {
    case 0: return 7.8E3;
    case 1: return 10.4E3;
    case 2: return 15.6E3;
    case 3: return 20.8E3;
    case 4: return 31.25E3;
    case 5: return 41.7E3;
    case 6: return 62.5E3;
    case 7: return 125E3;
    case 8: return 250E3;
    case 9: return 500E3;
  }

  return -1;
}

void set_ldo_flag()
{
  // Section 4.1.1.5
  long symbolDuration = 1000 / ( get_signal_bandwidth() / (1L << lora_get_SpreadingFactor()) ) ;

  // Section 4.1.1.6
  uint8_t ldoOn = symbolDuration > 16;

  uint8_t config3 = spi_rcv_data(REG_MODEM_CONFIG_3);
  bitWrite(config3, 3, ldoOn);
  spi_snd_data(REG_MODEM_CONFIG_3, config3);
}

/**
 * @brief Changes the spreading factor
 *
 * @param spreading_factor SF value between 6 and 12ì
 */
void lora_change_SpreadingFactor(uint8_t spreading_factor)
{
    if( spreading_factor < 6)
    {
        spreading_factor = 6;
    }
    else if (spreading_factor > 12)
    {
        spreading_factor = 12;
    }

    if (spreading_factor == 6)
    {
        spi_snd_data(REG_DETECTION_OPTIMIZE, 0xc5);
        spi_snd_data(REG_DETECTION_THRESHOLD, 0x0c);
    }
    else
    {
        spi_snd_data(REG_DETECTION_OPTIMIZE, 0xc3);
        spi_snd_data(REG_DETECTION_THRESHOLD, 0x0a);
    }

    // writing 7-4 bits, need to preserve the value of first 0-3
    spi_snd_data(REG_MODEM_CONFIG_2, (spi_rcv_data(REG_MODEM_CONFIG_2) & 0x0f) | ((spreading_factor << 4) & 0xf0));
    set_ldo_flag();
}

/**
 * @brief Generate the packet to be sent over LoRa
 *
 * @param packet Pointer to the packet
 * @param header Header of the packet
 * @param dev_id Unique device address
 * @param payload_size Size of the payload
 * @param payload Pointer to the payload
 *
 * @return 1 if the packet is generated correctly, 0 otherwise
 */
uint8_t packet_generator(uint8_t *packet, uint8_t header, uint8_t dev_id, uint8_t payload_size, uint8_t * payload)
{
    uint8_t result = 1;

    if(payload_size > 16)
    {
        result =  0;
    }

    packet[0] = header; // Header, we use this to recognize our packets from other users

    packet[1] = dev_id; // Device ID

    packet[2] = payload_size; // Payload Size

    uint8_t i;
    for(i=0; i < payload_size; i++)
    {
        packet[3+i]=payload[i];
    }

    //calculate checksum
    uint8_t checksum=0;
    for(i=0; i < (3+payload_size); i++)
    {
        checksum+=packet[i];
    }

    // make complement
    packet[19]=~checksum;

    return result;
}

//Connor - copy of code from LoRaWAN as complained about not being able to find lora_setFrequency
void setJoinFreq(int select){//set the frequency for join request (supposed to be one of the below at random)
    //put in sleep mode
    lora_sleep();
    //uint8_t debug=0;//Connor - used to help see what is going on, may not be able to read everything though
    //debug= spi_rcv_data(REG_OP_MODE);

    if (select==0){
        lora_setFrequency(868100000);//one of the official frequencies
    }if (select==1){
        lora_setFrequency(868300000);//one of the official frequencies
    }if (select==2){
        lora_setFrequency(868500000);//one of the official frequencies
    }
    // put in standby mode
    //lora_idle();
    //debug= spi_rcv_data(REG_OP_MODE);
}

/**
 * @brief Configure RFM95W to prepare for single rx window
 *
 */
void lora_singleRxWindowConfig()
{
    //printf("lora_singleRxWindowConfig\n");
    //1) Set Low Noise Amplifier (LNA)
    /*
      RegLna (0x0C)
      LnaGain[7:5] = '001' - LNA gain set to the maximum value
      LnaBoostLf [4:3] = '00' - Low Frequency (RFI_LF) LNA current adjustment = Default LNA current
      res [2] = '0'
      LnaBoostHf[1:0] = '11' - High Frequency (RFI_HF) LNA current adjustment = LNA Boost enable, 150% LNA current
     */
    spi_snd_data(REG_LR_LNA, 0x23);

    //2) Set BW, Coding rate and Implicit Header Mode On
    /*
     * BW [7:4] = Same as Tx - '0111' for 125kHz / '1000' for 250kHz
     * CodingRate [3:1] = '001' -  4/5 coding rate
     * ImplicitHeaderModeOn [0] = '0' - Packets have up-front header
     */
    //Print REG_LR_MODEMCONFIG1 and check if equal to 0x72
    //printf("REG_LR_MODEMCONFIG1 = %02x\n", spi_rcv_data(REG_LR_MODEMCONFIG1));

    //3) Set SF, RxPayloadCrcOn and SymbolTimeout (MSB)
    /*
     * SpreadingFactor [7:4] = Same as Tx for 1st window, SF9 for 2nd window
     * RxPayloadCrcOn [2] = '1'
     * SymbTimeout[1:0] = '00'
     */
    //Print REG_LR_MODEMCONFIG1 and check if equal to the one set in lora_begin
    //Test: Try to set SymbTimeout = '11'
    uint8_t r = spi_rcv_data(REG_LR_MODEMCONFIG2);
    spi_snd_data(REG_LR_MODEMCONFIG2, r | 0x03); //Set bit 0 an 1
    //printf("REG_LR_MODEMCONFIG2 = %02x\n", spi_rcv_data(REG_LR_MODEMCONFIG2));

    //4) Set Rx Window length (SymbolTimeout (LSB))
    /*
     * SymbTimeout[7:0] = 0x05 or 0x08
     */
    /*
     *  Timeout = SymbTimeout x SymbolTime(Ts)
     *  Ts = 2^SF / BW
     */
    //Test: Try to set SymbTimeout(MSB) = '11' SymbTimeout(LSB) = '11111111' = 1023 x Ts
    //BW125, SF7 = 1.047552sec
    //BW125, SF8 = 2.095104sec
    //BW125, SF9 = 4.190208sec
    //BW125, SF10 = 8.380416sec
    //BW125, SF11 = 16.760832sec
    //BW125, SF12 = 33.521664sec
    spi_snd_data(REG_LR_SYMBTIMEOUTLSB, 0xFF);

    //5) Set Low Data Rate Optimize and AgcAutoOn
    /*
     * LowDataRateOptimize[3] = same as Tx
     * AgcAutoOn[2] = '1'
     */
    //printf("REG_LR_MODEMCONFIG3 = %02x\n", spi_rcv_data(REG_LR_MODEMCONFIG3));

    //6) Set Max Payload Length
    /*
     * PayloadMaxLength[7:0] = 0x40 (64bytes)
     */
    spi_snd_data(REG_LR_PAYLOADMAXLENGTH, 0x40);

    //7) Invert I and Q signals
    /*
     * InvertIQ[6] = '1'
     */
    r = spi_rcv_data(REG_LR_INVERTIQ);
    spi_snd_data(REG_LR_INVERTIQ, r | 0x40); //Set bit 6
    //printf("REG_LR_INVERTIQ = %02x\n", spi_rcv_data(REG_LR_INVERTIQ));

    //8) Change DIO mapping
    //DIO0 to RxDone
    spi_snd_data( REG_LR_DIOMAPPING1,0x00);

    // reset FIFO address
    spi_snd_data(REG_FIFO_ADDR_PTR, 0);

    // put in single RX mode
    //spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
}

void lora_startRx()
{
    // put in single RX mode
    spi_snd_data(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_SINGLE);
}

/**
 * @brief Configure RFM95W to change the tx power between [2dBm to 14dBm]
 * OutputPower = range[0x03 : 0x0f] -> Pout = range[2dBM : 14dBM]
 *
 */
void lora_setTxPower(uint8_t txpower)
{
    //uint8_t reg = 0x50; //Enable PaSelect 0 , Max Output Power = 14dBm <--- As per EU regulation
    uint8_t reg = 0x80; //Enable Pa boost

    //Index corresponds to desired output power
    //uint8_t txPowerTable[15] = {0x04,0x04,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x0F}; //RFO_PIN
    uint8_t txPowerTable[15] = {0x00,0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C}; //PA_BOOST

    if(txpower > 14)
    {
        txpower = 14;
    }

    if(txpower < 2)
    {
        txpower = 2;
    }

    printf("SUCCESS - txPower = %02x\n", txpower);

    reg |= txPowerTable[txpower];

    printf("SUCCESS - reg = %02x\n", reg);

    spi_snd_data(REG_PA_CONFIG, reg);
}
