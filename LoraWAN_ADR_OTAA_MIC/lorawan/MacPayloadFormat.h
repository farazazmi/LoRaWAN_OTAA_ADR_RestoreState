/*
 * MacPayloadFormat.h
 *
 *  Created on: 18 Jul 2023
 *      Author: Guest-PC
 */

#ifndef LORAWAN_MACPAYLOADFORMAT_H_
#define LORAWAN_MACPAYLOADFORMAT_H_

#define MAC_DEVADDR_SIZE    4
#define MAC_FCTRL_SIZE      1
#define MAC_FCNT_SIZE       2

#define MAC_FOPTS_MAX_SIZE 15
#define MAC_FOPTS_MASK 0x0F

#define MAC_FPORT_MAX_SIZE 1

#include <stdint.h>

typedef struct MAC_FHDR_t
{
    uint32_t devAddr;
    uint8_t  fCtrl;
    uint32_t fCnt;      //Make it 4 bytes because Ai or Bo for frame payload and MIC calculation uses 4 bytes
    uint8_t  fopts[MAC_FOPTS_MAX_SIZE];
}MAC_FHDR_t;

#endif /* LORAWAN_MACPAYLOADFORMAT_H_ */
