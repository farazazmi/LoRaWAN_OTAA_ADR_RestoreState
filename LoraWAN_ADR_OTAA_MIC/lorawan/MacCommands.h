/*
 * MACCommands.h
 *
 *  Created on: 17 Jul 2023
 *      Author: Mohd Firdaus Hirman Ritom
 */

#ifndef LORAWAN_MACCOMMANDS_H_
#define LORAWAN_MACCOMMANDS_H_

#include <stdint.h>

#define MAX_EIRP 16 //From Lorawan Regional Lorawan 1.0.3

#define CID_LEN 1

//LIST OF MAC COMMANDS CID:
#define LINK_CHECK_REQ      0x02
#define LINK_CHECK_ANS      0x02
#define LINK_ADR_REQ        0x03
#define LINK_ADR_ANS        0x03
#define DUTY_CYCLE_REQ      0x04
#define DUTY_CYCLE_ANS      0x04
#define RX_PARAM_SETUP_REQ  0x05
#define RX_PARAM_SETUP_ANS  0x05
#define DEV_STATUS_REQ      0x06
#define DEV_STATUS_ANS      0x06
#define NEW_CHANNEL_REQ     0x07
#define NEW_CHANNEL_ANS     0x07
#define RX_TIMING_SETUP_REQ 0x08
#define RX_TIMING_SETUP_ANS 0x08
#define TX_PARAM_SETUP_REQ  0x09
#define TX_PARAM_SETUP_ANS  0x09
#define DI_CHANNEL_REQ      0x0A
#define DI_CHANNEL_ANS      0x0A
#define DEV_TIME_REQ        0x0D
#define DEV_TIME_ANS        0x0D

//DATA LENGTH OF EACH MAC COMMANDS
#define LINK_CHECK_REQ_LEN      0
#define LINK_CHECK_ANS_LEN      2
#define LINK_ADR_REQ_LEN        4
#define LINK_ADR_ANS_LEN        1
#define DUTY_CYCLE_REQ_LEN      1
#define DUTY_CYCLE_ANS_LEN      0
#define RX_PARAM_SETUP_REQ_LEN  4
#define RX_PARAM_SETUP_ANS_LEN  1
#define DEV_STATUS_REQ_LEN      0
#define DEV_STATUS_ANS_LEN      2
#define NEW_CHANNEL_REQ_LEN     5
#define NEW_CHANNEL_ANS_LEN     1
#define RX_TIMING_SETUP_REQ_LEN 1
#define RX_TIMING_SETUP_ANS_LEN 0
#define TX_PARAM_SETUP_REQ_LEN  1
#define TX_PARAM_SETUP_ANS_LEN  0
#define DI_CHANNEL_REQ_LEN      4
#define DI_CHANNEL_ANS_LEN      1
#define DEV_TIME_REQ_LEN        0
#define DEV_TIME_ANS_LEN        5

//DataRate Table
#define DR0_SF12_BW125              0x00
#define DR1_SF11_BW125              0x01
#define DR2_SF10_BW125              0x02
#define DR3_SF9_BW125               0x03
#define DR4_SF8_BW125               0x04
#define DR5_SF7_BW125               0x05
#define DR6_SF7_BW250               0x06
#define DR7_FSK_50KBPS              0x07
#define DR8_LR_FHSS_CR_1_3_BW137    0x08
#define DR9_LR_FHSS_CR_2_3_BW137    0x09
#define DR10_LR_FHSS_CR_1_3_BW336   0x0A
#define DR11_LR_FHSS_CR_2_3_BW336   0x0B

//Link Check Commands Data (0x02)
//LinkCheckReq has no payload
typedef struct LinkCheckAns_t
{
    uint8_t margin;
    uint8_t gwCnt;
} LinkCheckAns_t;

//ADR Commands Data (0x03)
#define LINKADRREQ_NBTRANS_MASK         0x0F //[3:0] bits of redundancy field
#define LINKADRREQ_CH_MASK_CNTL_MASK    0x70 //[6:4] bits of redundancy field

#define LINKADRANS_STATUS_POWER_ACK         0x04 //bit 2 of Status field
#define LINKADRANS_STATUS_DATARATE_ACK      0x02 //bit 1 of Status field
#define LINKADRANS_STATUS_CHANNELMASK_ACK   0x01 //bit 0 of Status field

typedef struct LinkADRReq_t
{
    uint8_t dataRate_TXPower;
    uint16_t chMask;
    uint8_t redundancy;
} LinkADRReq_t;

typedef struct LinkADRAns_t
{
    uint8_t status;
} LinkADRAns_t;

//Link Check Commands Data (0x04)
//LinkCheckAns has no payload

typedef struct DutyCycleReq_t
{
    uint8_t dutyCyclePL;
} DutyCycleReq_t;

/*
 * RX Param Setup Commands Data (0x05)
 */
#define RXPARAMSETUPREQ_FREQUENCY_MASK      0xFFFFFF //3 bytes (LSB) of RXParamSetupReq payload
#define RXPARAMSETUPREQ_RX2DATARATE_MASK    0x0F //[3:0] bits of DLSettings field
#define RXPARAMSETUPREQ_RX1DROFFSET_MASK    0x70 //[6:4] bits of DLSettings field

#define RXPARAMSETUPANS_RX1DROFFSET_ACK     0x04 //bit 2 of Status field
#define RXPARAMSETUPANS_RX2DATARATE_ACK     0x02 //bit 1 of Status field
#define RXPARAMSETUPANS_CHANNEL_ACK         0x01 //bit 0 of Status field

typedef struct RxParamSetupReq_t
{
    uint8_t DLSettings;
    uint32_t frequency; //frequency only needs 3 bytes
} RxParamSetupReq_t;

typedef struct RxParamSetupAns_t
{
    uint8_t status;
} RxParamSetupAns_t;

/*
 * Device Status Commands Data (0x06)
 * Note:
 * DevStatusReq has no payload
 */
#define DEVSTATUSANS_SNR_MASK    0x3F //[5:0] bits of RadioStatus field

typedef struct DevStatusAns_t
{
    uint8_t battery;
    uint8_t radioStatus;
} DevStatusAns_t;

/*
 * New Channel Commands Data (0x07)
 */
#define NEWCHANNELANS_STATUS_DR_RANGE_OK    0x02 //bit 1 of Status field
#define NEWCHANNELANS_STATUS_CH_FREQ_OK     0x01 //bit 0 of Status field

typedef struct NewChannelReq_t
{
    uint8_t chIndex;
    uint32_t frequency;
    uint8_t dataRateRange;
} NewChannelReq_t;

typedef struct NewChannelAns_t
{
    uint8_t status;
} NewChannelAns_t;

/*
 * DI Channel Commands Data (0x0A)
 */
#define DICHANNELANS_STATUS_UPLINK_FREQ_EXISTS  0x02 //bit 1 of Status field
#define DICHANNELANS_STATUS_CH_FREQ_OK          0x01 //bit 0 of Status field

typedef struct DIChannelReq_t
{
    uint8_t chIndex;
    uint32_t frequency;
} DIChannelReq_t;

typedef struct DIChannelAns_t
{
    uint8_t status;
} DIChannelAns_t;

/*
 * RX Timing Setup Commands Data (0x08)
 * Note:
 * RXTimingSetupAns has no payload
 */
#define RXTIMINGSETTINGS_DEL_MASK  0x0F //Bits [3:0] of RXTimingSettings field

typedef struct RXTimingSetupReq_t
{
    uint8_t rxTimingSettings;
} RXTimingSetupReq_t;

//TX Param Setup Commands Data (0x09)
// TXParamSetupAns has no payload
#define TXPARAMSETUP_MAX_EIRP_MASK              0x0F //Bits [3:0] of EIRP_DwellTime field
#define TXPARAMSETUP_UPLINK_DWELL_TIME_MASK     0x10 //Bit 4 of EIRP_DwellTime field
#define TXPARAMSETUP_DOWNLINK_DWELL_TIME_MASK   0x20 //Bit 5 of EIRP_DwellTime field

typedef struct TXParamSetupReq_t
{
    uint8_t EIRP_DwellTime;
} TXParamSetupReq_t;

//End-Device Time Commands
// EndDeviceAns has no payload

typedef struct EndDeviceReq_t
{
    uint8_t fractional_seconds;
    uint32_t second_since_epoch;
} EndDeviceReq_t;

#endif /* LORAWAN_MACCOMMANDS_H_ */
