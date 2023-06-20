#include "fat12_lib.h"

static uint16_t convertStringToIntLittleEdian();
static uint16_t convertHighEntry(const uint8_t* buff);
static uint16_t convertLowEntry(const uint8_t* buff);
void static printfNameOfFile(const uint8_t* str);
void static printfNameOfDirectory(const uint8_t* str);
static void copyDirectoryEntry(directoryEntrypointDisk tempEntryPoint, directoryEntrypointReal* directoryEntry);

bootSectorInfor getInformationBootSector(void)
{
    bootSectorInfor returnBootSector;
    structbootSector tempBootSector;

    if (0 == HAL_ReadSector(INDEX_OF_BOOT_SECTOR,tempBootSector.dataBootSector))
    {
        printf("\nread file faild!!!!");
    }
    else
    {
        strcopy(tempBootSector.feild.OEMName, returnBootSector.OEM, sizeof(returnBootSector.OEM));

        strcopy(tempBootSector.feild.Signature, returnBootSector.Signature, sizeof(returnBootSector.Signature));

        returnBootSector.bytePerSec = convertStringToIntLittleEdian(tempBootSector.feild.numOfBytePerSec,\
        sizeof(tempBootSector.feild.numOfBytePerSec));

        returnBootSector.secPerClus = convertStringToIntLittleEdian(tempBootSector.feild.numOfSecPerCluster,\
        sizeof(tempBootSector.feild.numOfSecPerCluster));

        returnBootSector.numOfReseredSec = convertStringToIntLittleEdian(tempBootSector.feild.numOfReservedSec,\
        sizeof(tempBootSector.feild.numOfReservedSec));

        returnBootSector.numOfFatTableCopies = convertStringToIntLittleEdian(tempBootSector.feild.numOfFATCopy,\
        sizeof(tempBootSector.feild.numOfFATCopy));

        returnBootSector.numOfRootDirectoryEntries = convertStringToIntLittleEdian(tempBootSector.feild.numOfRootDirectoryEntry,\
        sizeof(tempBootSector.feild.numOfRootDirectoryEntry));

        returnBootSector.numOfSecInFile = convertStringToIntLittleEdian(tempBootSector.feild.numOfSecInFilesystem,\
        sizeof(tempBootSector.feild.numOfSecInFilesystem));

        returnBootSector.numOfSecPerFAT = convertStringToIntLittleEdian(tempBootSector.feild.numOfSecPerFAT,\
        sizeof(tempBootSector.feild.numOfSecInFilesystem));

        returnBootSector.beginingOfFatTable = BOOT_SETOR_BEGIN + returnBootSector.numOfReseredSec;
        returnBootSector.beginingOfRootDirectory = (returnBootSector.numOfSecPerFAT * returnBootSector.numOfFatTableCopies) + 1;
        returnBootSector.beginingOfDataRegion = ((returnBootSector.numOfRootDirectoryEntries * BYTE_PER_ENTRY) / SIZE_PER_SECTOR)\
        + (returnBootSector.numOfFatTableCopies * returnBootSector.numOfSecPerFAT) + (returnBootSector.numOfReseredSec);

    }


    return returnBootSector;
}

static uint16_t convertStringToIntLittleEdian(const uint8_t* str, const uint32_t length)
{
    uint16_t result = 0;
    int32_t count = length-1;
    while(0 <= count)
    {
        result = result << EIGHT_BIT;
        result |= str[count];
        count -= 1;
    }

    return result;
}


void strcopy(const uint8_t* str, uint8_t* storeStr,uint32_t length)
{
    uint32_t i;
    for(i = 0; i < length; i++)
    {
        storeStr[i] = str[i];
    }
}

void strcopy2(const uint8_t* str, uint8_t* storeStr,uint32_t length, uint32_t position)
{
    uint32_t i;
    for(i = position; i < length + position; i++)
    {
        storeStr[i-position] = str[i];
    }
}

void printString(const uint8_t* str, uint32_t length)
{
    uint32_t i;
    for(i = 0; i < length; i++)
    {
    	printf("%1X ",str[i]);
	}
}

void createRootDirectory(directoryEntrypointReal* directoryEntry, uint32_t numOfEntryPoint, const bootSectorInfor* FatInfor)
{
    directoryEntrypointDisk tempEntryPoint;
    uint32_t startPosition = FatInfor->beginingOfRootDirectory;
    uint32_t i;

    for (i = 0; i < numOfEntryPoint; i++)
    {
        HAL_ReadByte(((startPosition * SIZE_PER_SECTOR) + (i * BYTE_PER_ENTRY)), BYTE_PER_ENTRY, tempEntryPoint.entryPointData);

        copyDirectoryEntry(tempEntryPoint,&directoryEntry[i]);

        // strcopy(tempEntryPoint.feild.nameOfFile, directoryEntry[i].nameOfFile,\
        // sizeof(tempEntryPoint.feild.nameOfFile));

        // directoryEntry[i].attributies = convertStringToIntLittleEdian(tempEntryPoint.feild.attributies,\
        // sizeof(tempEntryPoint.feild.attributies));

        // directoryEntry[i].resered = convertStringToIntLittleEdian(tempEntryPoint.feild.resered,\
        // sizeof(tempEntryPoint.feild.resered));

        // directoryEntry[i].fileCreationTime = convertStringToIntLittleEdian(tempEntryPoint.feild.fileCreationTime,\
        // sizeof(tempEntryPoint.feild.fileCreationTime));

        // directoryEntry[i].modifedTime = convertStringToIntLittleEdian(tempEntryPoint.feild.modifedTime,\
        // sizeof(tempEntryPoint.feild.modifedTime));

        // directoryEntry[i].modifiedDate = convertStringToIntLittleEdian(tempEntryPoint.feild.modifiedDate,\
        // sizeof(tempEntryPoint.feild.modifiedDate));

        // directoryEntry[i].startingCluster = convertStringToIntLittleEdian(tempEntryPoint.feild.startingCluster,\
        // sizeof(tempEntryPoint.feild.startingCluster));

        // directoryEntry[i].fileSize = convertStringToIntLittleEdian(tempEntryPoint.feild.fileSize,\
        // sizeof(tempEntryPoint.feild.fileSize));

    }
}

static void copyDirectoryEntry(directoryEntrypointDisk tempEntryPoint, directoryEntrypointReal* directoryEntry)
{
    strcopy(tempEntryPoint.feild.nameOfFile, directoryEntry->nameOfFile,\
    sizeof(tempEntryPoint.feild.nameOfFile));

    directoryEntry->attributies = convertStringToIntLittleEdian(tempEntryPoint.feild.attributies,\
    sizeof(tempEntryPoint.feild.attributies));

    directoryEntry->resered = convertStringToIntLittleEdian(tempEntryPoint.feild.resered,\
    sizeof(tempEntryPoint.feild.resered));

    directoryEntry->fileCreationTime = convertStringToIntLittleEdian(tempEntryPoint.feild.fileCreationTime,\
    sizeof(tempEntryPoint.feild.fileCreationTime));

    directoryEntry->modifedTime = convertStringToIntLittleEdian(tempEntryPoint.feild.modifedTime,\
    sizeof(tempEntryPoint.feild.modifedTime));

    directoryEntry->modifiedDate = convertStringToIntLittleEdian(tempEntryPoint.feild.modifiedDate,\
    sizeof(tempEntryPoint.feild.modifiedDate));

    directoryEntry->startingCluster = convertStringToIntLittleEdian(tempEntryPoint.feild.startingCluster,\
    sizeof(tempEntryPoint.feild.startingCluster));

    directoryEntry->fileSize = convertStringToIntLittleEdian(tempEntryPoint.feild.fileSize,\
    sizeof(tempEntryPoint.feild.fileSize));
}

void createFatTable(uint16_t* FatEntry, uint32_t numOfFatEntry, const bootSectorInfor* FatInfor)
{
    const uint32_t buffOfFatTableSize = FatInfor->numOfSecPerFAT * SIZE_PER_SECTOR;
    uint8_t buffOfFatTable[buffOfFatTableSize];
    uint32_t numReadByte = HAL_ReadMultiSector(FatInfor->beginingOfFatTable, FatInfor->numOfSecPerFAT, buffOfFatTable);
    uint32_t i = 0;
    uint8_t tempBuff[3];
    uint32_t buffPoint = 0;

    if (0 == numReadByte)
    {
        printf("\nRead file ERROR!!!!!\n");
    }
    else
    {
        for(i = 0; i < numOfFatEntry; i += 2)
        {
            strcopy2(buffOfFatTable, tempBuff, 3, buffPoint);
            buffPoint += 3;
            FatEntry[i] = convertHighEntry(tempBuff);
            FatEntry[i+1] = convertLowEntry(tempBuff);
        }
    }
}

static uint16_t convertHighEntry(const uint8_t* buff)
{
    uint16_t highEntry = 0;
    highEntry |= (buff[1] >> 4);
    highEntry = highEntry << 8;
    highEntry |= buff[0];

    return highEntry;
}

static uint16_t convertLowEntry(const uint8_t* buff)
{
    uint16_t lowEntry = 0;
    lowEntry |= buff[2];
    lowEntry = lowEntry << 4;
    lowEntry |= (buff[1] & 0x0f);

    return lowEntry;
}


void browserRootDirectory(directoryEntrypointReal* directoryEntry, uint32_t num)
{
    uint32_t i;
    for (i = 0; i < num; i++)
    {
        if(directoryEntry[i].nameOfFile[0] != 0x00 && directoryEntry[i].nameOfFile[0] != 0x5e && directoryEntry[i].attributies != LONG_NAME_FILE)
        {
            if (directoryEntry[i].attributies == 0x00)
            {
                printfNameOfFile(directoryEntry[i].nameOfFile);
            }
            else if ((directoryEntry[i].attributies & MASK_DIRECTORY) != 0)
            {
                printfNameOfDirectory(directoryEntry[i].nameOfFile);
            }
            printf("\n");
        }
    }
}

void static printfNameOfFile(const uint8_t* str)
{
    uint32_t i;
    for(i = 0; i < 11; i++)
    {
        if(' ' != str[i])
        {
            printf("%c",str[i]);
        }
        if (7 == i)
        {
            printf(".");
        }
    }
}

void static printfNameOfDirectory(const uint8_t* str)
{
    uint32_t i = 0;
    for(i = 0; i < 11; i++)
    {
        if ((65 <= str[i] && 90 >= str[i]) || (48 <= str[i] && 57 >= str[i]))
        {
            printf("%c",str[i]);
        }
    }
}

void browserSubdirectoy(directoryEntrypointReal* directoryEntry,const uint16_t* fatEntry, const bootSectorInfor* bootSectorInformation)
{
    uint8_t stopFlag = IS_RESUM;
    uint32_t index = directoryEntry->startingCluster;
    uint32_t i = 0;
    uint32_t numOfDirectoryEntry = (bootSectorInformation->secPerClus * bootSectorInformation->bytePerSec) / (BYTE_PER_ENTRY);
    directoryEntrypointDisk tempDirectoryDisk;
    directoryEntrypointReal tempDirectoryReal;
    uint32_t readPoint = 0;

    while(IS_RESUM == stopFlag)
    {
        for (i = 0; i < numOfDirectoryEntry; i ++)
        {
            readPoint = (bootSectorInformation->beginingOfDataRegion + ((index - 2) * bootSectorInformation->secPerClus)) * bootSectorInformation->bytePerSec;
            HAL_ReadByte( readPoint + (i * BYTE_PER_ENTRY), BYTE_PER_ENTRY, tempDirectoryDisk.entryPointData);


        }
    }
}
