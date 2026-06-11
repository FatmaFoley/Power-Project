/*
 * external_eeprom.h
 *
 *  Created on: Jun 7, 2026
 */

#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* Macros to indicate operation status */
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Function: EEPROM_writeByte
 * --------------------------
 * Writes a byte of data to the external EEPROM at the specified address.
 *
 *  u16addr: The 16-bit address in EEPROM to write the data byte.
 *  u8data: The data byte to be written.
 *
 *  returns: SUCCESS if the write operation was successful, ERROR otherwise.
 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/*
 * Function: EEPROM_readByte
 * -------------------------
 * Reads a byte of data from the external EEPROM at the specified address.
 *
 *  u16addr: The 16-bit address in EEPROM to read the data byte.
 *  u8data: Pointer to a variable where the read data will be stored.
 *
 *  returns: SUCCESS if the read operation was successful, ERROR otherwise.
 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
