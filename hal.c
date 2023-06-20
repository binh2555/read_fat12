#include "hal.h"

int32_t HAL_ReadSector(uint32_t index, uint8_t *buff)
{
	int32_t numberOfReadByte = 0;
	uint32_t fseekFlag = 0;
	FILE* fp = NULL;
	fp = fopen("floppy.img","rb");
	if (NULL == fp)
	{
		printf("\nERR,can not open file!!!!");
	}
	else
	{
		fseekFlag = fseek(fp, index * SIZE_PER_SECTOR, SEEK_SET);
		if (0 == fseekFlag)
		{
			numberOfReadByte = fread(buff,sizeof(uint8_t),SIZE_PER_SECTOR,fp);
		}
	}
	fclose(fp);
	return numberOfReadByte;
}

int32_t HAL_ReadMultiSector(uint32_t index, uint32_t num, uint8_t *buff)
{
	int32_t numberOfReadByte = 0;
	uint32_t fseekFlag = 0;
	FILE* fp = NULL;
	fp = fopen("floppy.img","rb");
	if (NULL == fp)
	{
		printf("\nERR,can not open file!!!!");
	}
	else
	{
		fseekFlag = fseek(fp, index * SIZE_PER_SECTOR, SEEK_SET);
		if (0 == fseekFlag)
		{
			numberOfReadByte = fread(buff,sizeof(uint8_t),num * SIZE_PER_SECTOR,fp);
		}
	}
	fclose(fp);
	return numberOfReadByte;
}

int32_t HAL_ReadByte(uint32_t index, uint32_t numOfByte, uint8_t *buff)
{
    int32_t numberOfReadByte = 0;
	uint32_t fseekFlag = 0;
	FILE* fp = NULL;
	fp = fopen("floppy.img","rb");
	if (NULL == fp)
	{
		printf("\nERR,can not open file!!!!");
	}
	else
	{
		fseekFlag = fseek(fp, index, SEEK_SET);
		if (0 == fseekFlag)
		{
			numberOfReadByte = fread(buff,sizeof(uint8_t),numOfByte,fp);
		}
		else
        {
            printf("\nFseek ERR!!!");
        }
	}
	fclose(fp);
	return numberOfReadByte;
}


