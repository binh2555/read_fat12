#ifndef    _HAL_H_
#define    _HAL_H_

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define SIZE_PER_SECTOR              (512U)
#define SIZE_OF_BOOT_SECTOR          SIZE_PER_SECTOR
#define INDEX_OF_BOOT_SECTOR         (0U)


int32_t HAL_ReadSector(uint32_t index, uint8_t *buff);
int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff);
int32_t HAL_ReadByte(uint32_t index, uint32_t numOfByte, uint8_t *buff);

#endif    /* _HAL_H_ */
