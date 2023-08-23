/*
 * MacCommandsParser.h
 *
 *  Created on: 17 Jul 2023
 *      Author: Mohd Firdaus Hirman Ritom
 */

#ifndef LORAWAN_MACCOMMANDSHANDLER_H_
#define LORAWAN_MACCOMMANDSHANDLER_H_

#include <stdint.h>
#include <Lorawan/LoraCtx.h>

void parseMacCommands(LORA_CTX_t* loraCtx ,uint8_t* frmPayload);

#endif /* LORAWAN_MACCOMMANDSHANDLER_H_ */
