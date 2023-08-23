/*
 * RestoreState.h
 *
 *  Created on: 20 Jul 2023
 *      Author: Faraz
 */

#ifndef RESTORESTATE_RESTORESTATE_H_
#define RESTORESTATE_RESTORESTATE_H_

#include <stdint.h>


typedef struct RegDB
{
    uint8_t regAdd;
    uint8_t regVal;
} RegDB;

//extern int instructions;                           //Faraz - To keep track of total no. of registers saved

void Save(uint8_t reg, uint8_t val);

void Restore();

#endif /* RESTORESTATE_RESTORESTATE_H_ */
