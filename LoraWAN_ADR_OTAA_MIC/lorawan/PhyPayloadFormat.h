/*
 * PhyPayloadFormat.h
 *
 *  Created on: 18 Jul 2023
 *      Author: Guest-PC
 */

#ifndef LORAWAN_PHYPAYLOADFORMAT_H_
#define LORAWAN_PHYPAYLOADFORMAT_H_

#define MHDR_SIZE   1
#define MIC_SIZE    4

//FType
#define MHDR_FRAME_TYPE_MASK 0xE0 //Bits [7:5] of MHDR
#define MHDR_FRAME_TYPE_JOIN_REQUEST                0x00
#define MHDR_FRAME_TYPE_JOIN_ACCEPT                 0x01
#define MHDR_FRAME_TYPE_UNCONFIRMED_DATA_UPLINK     0x02
#define MHDR_FRAME_TYPE_UNCONFIRMED_DATA_DOWNLINK   0x03
#define MHDR_FRAME_TYPE_CONFIRMED_DATA_UPLINK       0x04
#define MHDR_FRAME_TYPE_CONFIRMED_DATA_DOWNLINK     0x05
#define MHDR_FRAME_TYPE_PROPRIETARY                 0x07

//Major
#define MHDR_MAJOR_MASK         0x03 //Bits [1:0] of MHDR
#define MHDR_MAJOR_LORAWAN_R1   0x00

//MIC
#define MIC_OK      1
#define MIC_NOT_OK  0

#endif /* LORAWAN_PHYPAYLOADFORMAT_H_ */
