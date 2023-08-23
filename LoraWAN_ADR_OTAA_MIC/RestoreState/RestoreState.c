/*
 * RestoreState.c
 *
 *  Created on: 20 Jul 2023
 *      Author: Faraz Azmi
 */

#include <stdint.h>
#include <msp430.h>
#include "RestoreState/RestoreState.h"
#include "spi/spi.h"


#ifdef __TI_COMPILER_VERSION__
#pragma PERSISTENT(instructions)
int instructions = 0;    //Faraz - Saves number of instructions in persistent memory
#endif

#ifdef __TI_COMPILER_VERSION__
#pragma PERSISTENT(regArray)
RegDB regArray[30] = { 0 }; // Faraz - Array for saving Transceiver's registers
#endif


//#pragma DATA_SECTION(regArray, ".reg_fram")
//RegDB regArray[100];                        //Faraz - Register database array structure

//#pragma DATA_SECTION(instructions, ".reg_fram")
//int instructions;                           //Faraz - To keep track of total no. of registers saved

void Save(uint8_t reg, uint8_t val)
{
    //FRCTL0 = FRCTLPW | WPROT_0;             //Faraz - Disable FRAM Write protection
    unsigned int i = 0;
    int exists = 0;
    while(!exists && (i < instructions))
    {
        if(regArray[i].regAdd == reg)       //Faraz - If register has already been saved before
        {
            exists = 1;
        }
        else
        {
            i++;                            //Faraz - Move to next index in Database
        }

    }

    if(exists == 1)
    {
        regArray[i].regVal = val;           //Faraz - Update value of saved register
    }
    else
    {
        regArray[i].regAdd = reg;           //Faraz - Add new register address
        regArray[i].regVal = val;           //        and its value.
        instructions++;                     //Faraz - Increment instructions counter to keep track of total instructions
    }
    //FRCTL0 = FRCTLPW | WPROT;               //Faraz - Enable FRAM write protection
}

void Restore()
{
    int j=0;
    while( j < instructions)
    {
        spi_snd_data(regArray[j].regAdd, regArray[j].regVal);
        j++;
    }
}

