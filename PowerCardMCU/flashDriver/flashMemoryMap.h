/*
 * flashMemoryMap.h
 *
 *  Created on: Feb 1, 2021
 *      Author: Leo Clouet
 *
 *  The memory map of the flash. This show where are placed in FLASH the memory.
 *  Be careful of where you place the memory. Remember that the code is also placed in FLASH.
 *  You do not want to erase and program a code sector. For more information of where
 *  the code is placed, please refer to the linker configuration.
 */

#ifndef FLASHMEMORYMAP_H_
#define FLASHMEMORYMAP_H_

#define FLASH_SECTOR_SIZE 0x1000
#define FLASH_PAGE_SIZE 256

#define FLASH_BASE_ADDRESS 0x60000000

#define FLASH_PARAMETERS_A_ADDRESS 0x60090000
#define FLASH_PARAMETERS_A_SIZE (4*1024)

#define FLASH_PARAMETERS_B_ADDRESS 0x60113000
#define FLASH_PARAMETERS_B_SIZE (4*1024)


#endif /* FLASHMEMORYMAP_H_ */
